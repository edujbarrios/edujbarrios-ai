//! Equal-width confidence-binned binary Expected Calibration Error.

/// ECE from class-one probabilities, labels, and 1..100 confidence bins.
pub fn expected_calibration_error(probabilities:&[f64],labels:&[u8],bins:usize)
    ->Result<f64,&'static str>{
    let n=probabilities.len();
    if n==0 || labels.len()!=n || !(1..=100).contains(&bins) {
        return Err("invalid vector sizes or bin count");
    }
    let mut counts=vec![0_usize;bins];
    let mut confidences=vec![0.0_f64;bins];
    let mut correct=vec![0.0_f64;bins];
    for (&p,&y) in probabilities.iter().zip(labels.iter()) {
        if !p.is_finite() || !(0.0..=1.0).contains(&p) || y>1 {
            return Err("invalid binary labels or probabilities");
        }
        let predicted=u8::from(p>=0.5);
        let confidence=p.max(1.0-p);
        let bin=((confidence*bins as f64).floor() as usize).min(bins-1);
        counts[bin]+=1;
        confidences[bin]+=confidence;
        correct[bin]+=f64::from(predicted==y);
        if !confidences[bin].is_finite() || !correct[bin].is_finite(){
            return Err("ECE bin overflow");
        }
    }
    let mut ece=0.0_f64;
    for b in 0..bins {
        if counts[b]==0 {continue;}
        let count=counts[b] as f64;
        ece+=(count/n as f64)*(correct[b]/count-confidences[b]/count).abs();
    }
    if !ece.is_finite() || !(0.0..=1.0+1e-12).contains(&ece) {
        return Err("invalid ECE");
    }
    Ok(ece.min(1.0))
}

#[cfg(test)]
mod tests {
    use super::expected_calibration_error as ece;
    #[test]
    fn values_and_boundaries() {
        assert!((ece(&[0.9,0.8,0.2,0.6],&[1,0,0,1],2).unwrap()-0.025).abs()<1e-12);
        assert!((ece(&[0.9,0.8,0.2,0.6],&[1,0,0,1],4).unwrap()-0.225).abs()<1e-12);
        assert_eq!(ece(&[1.0,0.0],&[1,0],10),Ok(0.0));
        assert_eq!(ece(&[0.5,0.5],&[0,1],1),Ok(0.0));
        assert_eq!(ece(&[1.0],&[0],100),Ok(1.0));
    }
    #[test]
    fn invalid() {
        assert!(ece(&[],&[],2).is_err());
        assert!(ece(&[0.5],&[],2).is_err());
        assert!(ece(&[0.5],&[1],0).is_err());
        assert!(ece(&[0.5],&[1],101).is_err());
        assert!(ece(&[1.1],&[1],2).is_err());
        assert!(ece(&[f64::NAN],&[1],2).is_err());
        assert!(ece(&[0.5],&[2],2).is_err());
    }
}
