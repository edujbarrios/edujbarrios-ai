//! Row-major z-score feature scaling with Welford population moments.

/// Returns (means[d], population standard deviations[d], normalized[n*d]).
pub fn z_score_standardization(data:&[f64],n:usize,d:usize)
    -> Result<(Vec<f64>,Vec<f64>,Vec<f64>),&'static str> {
    if n==0 || d==0 || n.checked_mul(d).ok_or("dimension overflow")?!=data.len() ||
       !data.iter().all(|v|v.is_finite()) {
        return Err("invalid dimensions or values");
    }
    let mut means=vec![0.0_f64;d];
    let mut m2=vec![0.0_f64;d];
    for i in 0..n {
        for j in 0..d {
            let value=data[i*d+j];
            let delta=value-means[j];
            if !delta.is_finite(){return Err("mean delta overflow");}
            means[j]+=delta/(i+1) as f64;
            let delta2=value-means[j];
            let term=delta*delta2;
            if !means[j].is_finite() || !delta2.is_finite() || !term.is_finite(){
                return Err("variance update overflow");
            }
            m2[j]+=term;
            if !m2[j].is_finite(){return Err("variance sum overflow");}
        }
    }
    let mut stds=Vec::with_capacity(d);
    for value in m2 {
        let variance=value/n as f64;
        if !variance.is_finite() || variance<0.0 {
            return Err("invalid variance");
        }
        stds.push(variance.sqrt());
    }
    let mut normalized=Vec::with_capacity(data.len());
    for i in 0..n {
        for j in 0..d {
            if stds[j]==0.0 {
                normalized.push(0.0);
            } else {
                let delta=data[i*d+j]-means[j];
                let value=delta/stds[j];
                if !delta.is_finite() || !value.is_finite() {
                    return Err("normalized output overflow");
                }
                normalized.push(value);
            }
        }
    }
    Ok((means,stds,normalized))
}

#[cfg(test)]
mod tests {
    use super::z_score_standardization as scale;
    #[test]
    fn basic_and_constant() {
        let (means,stds,z)=scale(&[1.0,10.0,2.0,10.0,3.0,10.0],3,2).unwrap();
        assert_eq!(means,vec![2.0,10.0]);
        assert!((stds[0]-(2.0_f64/3.0).sqrt()).abs()<1e-12);
        assert_eq!(stds[1],0.0);
        assert!((z[0]+1.5_f64.sqrt()).abs()<1e-12);
        assert_eq!(z[1],0.0);
        assert_eq!(&z[2..4],&[0.0,0.0]);
        assert!((z[4]-1.5_f64.sqrt()).abs()<1e-12);
        assert_eq!(scale(&[5.0,2.0],1,2),
                   Ok((vec![5.0,2.0],vec![0.0,0.0],vec![0.0,0.0])));
    }
    #[test]
    fn invalid() {
        assert!(scale(&[],0,1).is_err());
        assert!(scale(&[1.0],1,0).is_err());
        assert!(scale(&[1.0],1,2).is_err());
        assert!(scale(&[1.0,f64::NAN],1,2).is_err());
        assert!(scale(&[-1e308,1e308],2,1).is_err());
    }
}
