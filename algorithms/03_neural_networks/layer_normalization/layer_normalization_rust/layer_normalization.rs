//! Forward LayerNorm with fixed, caller-supplied affine parameters.

/// Normalize one feature vector using population variance and epsilon.
pub fn layer_normalization(
    input:&[f64], gamma:&[f64], beta:&[f64], epsilon:f64
) -> Result<Vec<f64>,&'static str> {
    let n=input.len();
    if n==0 || gamma.len()!=n || beta.len()!=n ||
       !epsilon.is_finite() || epsilon<=0.0 {
        return Err("invalid dimensions or epsilon");
    }
    if !input.iter().chain(gamma.iter()).chain(beta.iter())
         .all(|v|v.is_finite()) {
        return Err("non-finite feature or affine parameters");
    }
    let mut mean=0.0_f64;
    for (i,&value) in input.iter().enumerate() {
        let delta=value-mean;
        if !delta.is_finite(){return Err("mean overflow");}
        mean+=delta/(i+1) as f64;
        if !mean.is_finite(){return Err("mean overflow");}
    }
    let mut squared_sum=0.0_f64;
    for &value in input {
        let delta=value-mean;
        let square=delta*delta;
        if !delta.is_finite() || !square.is_finite(){
            return Err("variance overflow");
        }
        squared_sum+=square;
        if !squared_sum.is_finite(){return Err("variance accumulation overflow");}
    }
    let variance=squared_sum/n as f64;
    let divisor=(variance+epsilon).sqrt();
    if !variance.is_finite() || !divisor.is_finite() || divisor<=0.0 {
        return Err("normalizer overflow");
    }
    let mut output=Vec::with_capacity(n);
    for i in 0..n {
        let centered=input[i]-mean;
        let normalized=centered/divisor;
        let weighted=gamma[i]*normalized;
        let value=weighted+beta[i];
        if !centered.is_finite() || !normalized.is_finite() ||
           !weighted.is_finite() || !value.is_finite() {
            return Err("affine output overflow");
        }
        output.push(value);
    }
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::layer_normalization as normalize;
    #[test]
    fn standard_and_affine() {
        let inv=1.0/(2.0_f64/3.0+1e-5).sqrt();
        let out=normalize(&[1.0,2.0,3.0],&[1.0,1.0,1.0],
                          &[0.0,0.0,0.0],1e-5).unwrap();
        assert!((out[0]+inv).abs()<1e-12);
        assert!(out[1].abs()<1e-12);
        assert!((out[2]-inv).abs()<1e-12);
        let out=normalize(&[1.0,2.0,3.0],&[2.0,1.0,0.5],
                          &[1.0,2.0,3.0],1e-5).unwrap();
        assert!((out[0]-(1.0-2.0*inv)).abs()<1e-12);
        assert!((out[2]-(3.0+0.5*inv)).abs()<1e-12);
    }
    #[test]
    fn constant_and_invalid() {
        assert_eq!(normalize(&[7.0,7.0],&[1.0,1.0],&[4.0,5.0],1e-5),
                   Ok(vec![4.0,5.0]));
        assert_eq!(normalize(&[10.0],&[1.0],&[4.0],1e-5),Ok(vec![4.0]));
        assert!(normalize(&[],&[],&[],1e-5).is_err());
        assert!(normalize(&[1.0],&[],&[0.0],1e-5).is_err());
        assert!(normalize(&[1.0],&[1.0],&[0.0],0.0).is_err());
        assert!(normalize(&[1.0,f64::NAN],&[1.0,1.0],&[0.0,0.0],1e-5).is_err());
        assert!(normalize(&[1.7e308,-1.7e308],&[1.0,1.0],
                          &[0.0,0.0],1e-5).is_err());
    }
}
