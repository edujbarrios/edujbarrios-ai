//! Jensen–Shannon divergence of aligned categorical distributions (base 2).

/// Return finite bounded symmetric information divergence, zero terms omitted.
pub fn jensen_shannon_divergence(p:&[f64],q:&[f64])->Result<f64,&'static str>{
    let n=p.len();
    if !(1..=4096).contains(&n) || q.len()!=n ||
       p.iter().chain(q.iter()).any(|x|!x.is_finite() || !(0.0..=1.0).contains(x)){
        return Err("invalid probability entries or shapes");
    }
    let sum_p:f64=p.iter().sum();
    let sum_q:f64=q.iter().sum();
    if !sum_p.is_finite() || !sum_q.is_finite() ||
       (sum_p-1.0).abs()>1e-9 || (sum_q-1.0).abs()>1e-9 {
        return Err("probabilities must sum to one");
    }
    let mut result=0.0_f64;
    for (&a,&b) in p.iter().zip(q.iter()) {
        let mix=0.5*a+0.5*b;
        if a>0.0 {result+=0.5*a*(a/mix).log2();}
        if b>0.0 {result+=0.5*b*(b/mix).log2();}
        if !result.is_finite(){return Err("JSD arithmetic overflow");}
    }
    if !(-1e-12..=1.0+1e-12).contains(&result) {
        return Err("invalid JSD");
    }
    Ok(result.clamp(0.0,1.0))
}

#[cfg(test)]
mod tests {
    use super::jensen_shannon_divergence as jsd;
    #[test]
    fn examples() {
        assert!((jsd(&[1.0,0.0],&[0.0,1.0]).unwrap()-1.0).abs()<1e-12);
        assert_eq!(jsd(&[0.5,0.5],&[0.5,0.5]),Ok(0.0));
        assert!((jsd(&[1.0,0.0],&[0.5,0.5]).unwrap()-0.31127812445913283).abs()<1e-12);
        assert_eq!(jsd(&[1.0],&[1.0]),Ok(0.0));
    }
    #[test]
    fn invalid() {
        assert!(jsd(&[],&[]).is_err());
        assert!(jsd(&[1.0],&[0.5]).is_err());
        assert!(jsd(&[1.0,0.0],&[1.0]).is_err());
        assert!(jsd(&[1.0,0.0],&[-0.1,1.1]).is_err());
        assert!(jsd(&[1.0,0.0],&[f64::NAN,1.0]).is_err());
        assert!(jsd(&[1.0,0.0],&[0.2,0.2]).is_err());
    }
}
