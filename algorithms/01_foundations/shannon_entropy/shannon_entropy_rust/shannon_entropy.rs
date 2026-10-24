//! Base-2 Shannon entropy for normalized finite categorical probabilities.

/// Return entropy in bits; zero probability terms contribute zero.
pub fn shannon_entropy(probabilities:&[f64])->Result<f64,&'static str>{
    if !(1..=4096).contains(&probabilities.len()) ||
       probabilities.iter().any(|p|!p.is_finite() || !(0.0..=1.0).contains(p)) {
        return Err("invalid categorical probability vector");
    }
    let total: f64=probabilities.iter().sum();
    if !total.is_finite() || (total-1.0).abs()>1e-9 {
        return Err("probability vector must sum to one");
    }
    let mut entropy=0.0_f64;
    for &p in probabilities {
        if p>0.0 {entropy-=p*p.log2();}
        if !entropy.is_finite(){return Err("entropy overflow");}
    }
    if entropy< -1e-12 {return Err("negative entropy");}
    Ok(entropy.max(0.0))
}

#[cfg(test)]
mod tests {
    use super::shannon_entropy as entropy;
    #[test]
    fn values() {
        assert_eq!(entropy(&[1.0,0.0]),Ok(0.0));
        assert!((entropy(&[0.5,0.5]).unwrap()-1.0).abs()<1e-12);
        assert!((entropy(&[0.25;4]).unwrap()-2.0).abs()<1e-12);
        assert!((entropy(&[0.75,0.25]).unwrap()-0.8112781244591328).abs()<1e-12);
        assert_eq!(entropy(&[1.0]),Ok(0.0));
    }
    #[test]
    fn errors() {
        assert!(entropy(&[]).is_err());
        assert!(entropy(&[0.2,0.2]).is_err());
        assert!(entropy(&[1.1,-0.1]).is_err());
        assert!(entropy(&[f64::NAN,1.0]).is_err());
        assert!(entropy(&[f64::INFINITY,0.0]).is_err());
    }
}
