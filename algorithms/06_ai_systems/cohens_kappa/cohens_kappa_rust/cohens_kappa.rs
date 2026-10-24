//! Cohen's kappa for two raters assigning binary class labels.

/// Return chance-corrected agreement; reject mathematically undefined p_e=1.
pub fn cohens_kappa(first:&[u8],second:&[u8])
    ->Result<f64,&'static str>{
    let n=first.len();
    if n==0 || second.len()!=n ||
       first.iter().chain(second.iter()).any(|&v|v>1) {
        return Err("invalid aligned binary labels");
    }
    let (mut zero_a,mut zero_b,mut matches)=(0_usize,0_usize,0_usize);
    for (&a,&b) in first.iter().zip(second.iter()){
        if a==0 {zero_a+=1;}
        if b==0 {zero_b+=1;}
        if a==b {matches+=1;}
    }
    let pa=zero_a as f64/n as f64;
    let pb=zero_b as f64/n as f64;
    let observed=matches as f64/n as f64;
    let expected=pa*pb+(1.0-pa)*(1.0-pb);
    let denominator=1.0-expected;
    if !expected.is_finite() || !observed.is_finite() || denominator<=0.0 {
        return Err("undefined chance-corrected agreement");
    }
    let value=(observed-expected)/denominator;
    if !value.is_finite() || !(-1.0-1e-12..=1.0+1e-12).contains(&value) {
        return Err("invalid kappa value");
    }
    Ok(value.clamp(-1.0,1.0))
}

#[cfg(test)]
mod tests {
    use super::cohens_kappa as kappa;
    #[test]
    fn agreements(){
        assert!((kappa(&[0,0,1,1],&[0,0,0,1]).unwrap()-0.5).abs()<1e-12);
        assert_eq!(kappa(&[0,1],&[1,0]),Ok(-1.0));
        assert_eq!(kappa(&[0,1],&[0,1]),Ok(1.0));
        assert_eq!(kappa(&[0,0,1,1],&[0,1,0,1]),Ok(0.0));
        assert_eq!(kappa(&[0,0],&[1,1]),Ok(0.0));
    }
    #[test]
    fn invalid(){
        assert!(kappa(&[],&[]).is_err());
        assert!(kappa(&[0],&[]).is_err());
        assert!(kappa(&[0,0],&[0,0]).is_err());
        assert!(kappa(&[1,1],&[1,1]).is_err());
        assert!(kappa(&[0,2],&[0,1]).is_err());
    }
}
