//! Training-mode dense batch normalization with population variance.

/// Row-major X[n*d]; gamma/beta per feature. Return row-major result.
pub fn batch_normalization(x:&[f64],n:usize,d:usize,
    gamma:&[f64],beta:&[f64])->Result<Vec<f64>,&'static str>{
    if !(1..=512).contains(&n) || !(1..=32).contains(&d) ||
       n.checked_mul(d).ok_or("dimension overflow")?!=x.len() ||
       gamma.len()!=d || beta.len()!=d ||
       x.iter().chain(gamma.iter()).chain(beta.iter())
         .any(|v|!v.is_finite() || v.abs()>1e6) {
        return Err("invalid bounded feature matrix or parameters");
    }
    let mut mean=vec![0.0_f64;d];
    let mut variance=vec![0.0_f64;d];
    let mut scales=vec![0.0_f64;d];
    for j in 0..d {
        for i in 0..n {mean[j]+=x[i*d+j]/n as f64;}
        for i in 0..n {
            let delta=x[i*d+j]-mean[j];
            variance[j]+=delta*delta/n as f64;
        }
        let divisor=(variance[j]+1e-5).sqrt();
        scales[j]=gamma[j]/divisor;
        if !mean[j].is_finite() || !variance[j].is_finite() ||
           !divisor.is_finite() || divisor<=0.0 || !scales[j].is_finite() {
            return Err("invalid feature moments");
        }
    }
    let mut out=Vec::with_capacity(n*d);
    for i in 0..n {
        for j in 0..d {
            let value=(x[i*d+j]-mean[j])*scales[j]+beta[j];
            if !value.is_finite(){return Err("normalized output overflow");}
            out.push(value);
        }
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::batch_normalization as bn;
    #[test]
    fn fixtures() {
        let output=bn(&[1.0,10.0,3.0,10.0],2,2,&[1.0,2.0],&[0.0,-1.0]).unwrap();
        let s=1.0/(1.0_f64+1e-5).sqrt();
        assert!((output[0]+s).abs()<1e-12);
        assert_eq!(output[1],-1.0);
        assert!((output[2]-s).abs()<1e-12);
        assert_eq!(output[3],-1.0);
        assert_eq!(bn(&[7.0,4.0],1,2,&[1.0,2.0],&[0.0,-1.0]),Ok(vec![0.0,-1.0]));
        let negative=bn(&[1.0,10.0,3.0,10.0],2,2,&[-1.0,0.0],&[0.0,-1.0]).unwrap();
        assert!((negative[0]-s).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        assert!(bn(&[],0,1,&[1.0],&[0.0]).is_err());
        assert!(bn(&[1.0,2.0],1,2,&[1.0],&[0.0]).is_err());
        assert!(bn(&[1.0,2.0],2,2,&[1.0,1.0],&[0.0,0.0]).is_err());
        assert!(bn(&[f64::NAN],1,1,&[1.0],&[0.0]).is_err());
        assert!(bn(&[1000001.0],1,1,&[1.0],&[0.0]).is_err());
    }
}
