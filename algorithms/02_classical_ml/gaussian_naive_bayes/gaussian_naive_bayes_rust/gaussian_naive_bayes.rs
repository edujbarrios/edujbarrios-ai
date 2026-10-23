//! Binary Gaussian naive Bayes with diagonal class variances.

/// Fit from row-major n*d samples and predict one d-dimensional query.
/// Returns (predicted 0/1 class, probability of class 1).
pub fn gaussian_naive_bayes(
    samples: &[f64], labels: &[u8], n: usize, d: usize, query: &[f64]
) -> Result<(u8,f64), &'static str> {
    if n < 2 || d == 0 || n.checked_mul(d).ok_or("shape overflow")? != samples.len()
       || labels.len()!=n || query.len()!=d {
        return Err("invalid dimensions");
    }
    if !samples.iter().chain(query).all(|v| v.is_finite()) ||
       labels.iter().any(|&v| v>1) {
        return Err("invalid data or label");
    }
    let (mut means,mut variances)=(vec![0.0_f64;2*d],vec![0.0_f64;2*d]);
    let mut counts=[0_usize;2];
    for i in 0..n {
        let class=labels[i] as usize;
        counts[class]+=1;
        for j in 0..d {
            let idx=class*d+j;
            let delta=samples[i*d+j]-means[idx];
            if !delta.is_finite() {return Err("mean overflow");}
            means[idx]+=delta/counts[class] as f64;
            if !means[idx].is_finite(){return Err("mean overflow");}
        }
    }
    if counts[0]==0 || counts[1]==0 {return Err("both classes required");}
    for i in 0..n {
        let class=labels[i] as usize;
        for j in 0..d {
            let idx=class*d+j;
            let delta=samples[i*d+j]-means[idx];
            let square=delta*delta;
            if !delta.is_finite() || !square.is_finite(){return Err("variance overflow");}
            variances[idx]+=square;
            if !variances[idx].is_finite(){return Err("variance overflow");}
        }
    }
    let mut scores=[0.0_f64;2];
    for class in 0..2 {
        scores[class]=(counts[class] as f64/n as f64).ln();
        for j in 0..d {
            let idx=class*d+j;
            let var=(variances[idx]/counts[class] as f64).max(1e-9);
            let delta=query[j]-means[idx];
            let square=delta*delta;
            let term=(std::f64::consts::TAU*var).ln()+square/var;
            if !var.is_finite() || !delta.is_finite() ||
               !square.is_finite() || !term.is_finite() {return Err("likelihood overflow");}
            scores[class]-=0.5*term;
            if !scores[class].is_finite(){return Err("likelihood overflow");}
        }
    }
    let log_odds=scores[1]-scores[0];
    if !log_odds.is_finite(){return Err("log odds overflow");}
    let e=(-log_odds.abs()).exp();
    let p1=if log_odds>=0.0 {1.0/(1.0+e)} else {e/(1.0+e)};
    Ok((if scores[1]>scores[0]{1}else{0},p1))
}

#[cfg(test)]
mod tests {
    use super::gaussian_naive_bayes as classify;
    #[test]
    fn classes_and_tie() {
        let x=[0.0,0.0,0.0,1.0,5.0,5.0,5.0,6.0];
        let y=[0,0,1,1];
        let (label,p)=classify(&x,&y,4,2,&[5.0,5.0]).unwrap();
        assert_eq!(label,1);assert!(p>0.99);
        let (label,p)=classify(&x,&y,4,2,&[0.0,0.0]).unwrap();
        assert_eq!(label,0);assert!(p<0.01);
        assert_eq!(classify(&[0.0,2.0],&[0,1],2,1,&[1.0]),Ok((0,0.5)));
    }
    #[test]
    fn invalid() {
        assert!(classify(&[],&[],0,1,&[0.0]).is_err());
        assert!(classify(&[0.0,1.0],&[0,0],2,1,&[0.0]).is_err());
        assert!(classify(&[0.0,1.0],&[0,2],2,1,&[0.0]).is_err());
        assert!(classify(&[f64::NAN,1.0],&[0,1],2,1,&[0.0]).is_err());
        assert!(classify(&[0.0,1.0],&[0,1],2,1,&[f64::INFINITY]).is_err());
        assert!(classify(&[0.0,1.0],&[0,1],2,2,&[0.0,0.0]).is_err());
    }
}
