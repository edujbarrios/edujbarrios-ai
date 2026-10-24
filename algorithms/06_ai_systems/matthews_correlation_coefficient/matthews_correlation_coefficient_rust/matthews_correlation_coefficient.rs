//! Matthews correlation coefficient for binary hard-label predictions.

/// Return correlation in [-1,1], with undefined denominator treated as 0.
pub fn matthews_correlation_coefficient(truth:&[u8],predictions:&[u8])
    ->Result<f64,&'static str> {
    if truth.is_empty() || truth.len()!=predictions.len() ||
       truth.iter().chain(predictions.iter()).any(|&x|x>1) {
        return Err("nonempty aligned binary predictions required");
    }
    let (mut tp,mut tn,mut fp,mut fn_)=(0.0_f64,0.0_f64,0.0_f64,0.0_f64);
    for (&y,&p) in truth.iter().zip(predictions.iter()) {
        match (y,p) {
            (1,1)=>tp+=1.0,
            (0,0)=>tn+=1.0,
            (0,1)=>fp+=1.0,
            (1,0)=>fn_+=1.0,
            _=>return Err("invalid label"),
        }
    }
    let n=truth.len() as f64;
    tp/=n;tn/=n;fp/=n;fn_/=n;
    let denom=(tp+fp)*(tp+fn_)*(tn+fp)*(tn+fn_);
    if !denom.is_finite() || denom<0.0{return Err("MCC denominator overflow");}
    let score=if denom==0.0 {0.0} else {(tp*tn-fp*fn_)/denom.sqrt()};
    if !score.is_finite() || !(-1.0-1e-12..=1.0+1e-12).contains(&score){
        return Err("invalid MCC result");
    }
    Ok(score.clamp(-1.0,1.0))
}

#[cfg(test)]
mod tests {
    use super::matthews_correlation_coefficient as mcc;
    #[test]
    fn scores() {
        let y=[1,1,0,0];
        assert_eq!(mcc(&y,&y),Ok(1.0));
        assert_eq!(mcc(&y,&[0,0,1,1]),Ok(-1.0));
        assert_eq!(mcc(&y,&[1,0,1,0]),Ok(0.0));
        assert_eq!(mcc(&y,&[0,0,0,0]),Ok(0.0));
        assert!((mcc(&[1,1,1,0,0],&[1,1,0,1,0]).unwrap()
            -(2.0_f64/15.0)/(36.0_f64/625.0).sqrt()).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        assert!(mcc(&[],&[]).is_err());
        assert!(mcc(&[0],&[]).is_err());
        assert!(mcc(&[0,2],&[0,1]).is_err());
        assert!(mcc(&[0,1],&[0,3]).is_err());
    }
}
