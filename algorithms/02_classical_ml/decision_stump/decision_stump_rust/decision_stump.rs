//! Exhaustive single-feature binary decision stump training.

/// Training result: observed threshold, positive-on-right orientation,
/// and count of wrong training predictions.
pub fn decision_stump(x: &[f64], labels: &[u8])
    -> Result<(f64,bool,usize), &'static str> {
    if x.is_empty() || x.len()!=labels.len() ||
       !x.iter().all(|v|v.is_finite()) ||
       labels.iter().any(|&v|v>1) {
        return Err("invalid finite features or binary labels");
    }
    let mut best=(x[0],true,x.len());
    for &threshold in x {
        for right in [true,false] {
            let errors=x.iter().zip(labels).filter(|&(value,label)| {
                let prediction=if right {*value>threshold} else {*value<=threshold};
                prediction!=(*label==1)
            }).count();
            let old=(best.2,best.0,if best.1{0}else{1});
            let candidate=(errors,threshold,if right{0}else{1});
            // f64 comparison is defined because inputs are finite.
            if errors < old.0 ||
               (errors==old.0 &&
                (threshold<old.1 ||
                 (threshold==old.1 && candidate.2<old.2))) {
                best=(threshold,right,errors);
            }
        }
    }
    Ok(best)
}

/// Apply a fitted stump to a finite observation.
pub fn decision_stump_predict(
    feature: f64, threshold: f64, positive_right: bool
) -> Result<u8,&'static str> {
    if !feature.is_finite() || !threshold.is_finite() {
        return Err("finite feature/threshold required");
    }
    Ok(if (feature>threshold)==positive_right {1}else{0})
}

#[cfg(test)]
mod tests {
    use super::{decision_stump as train,decision_stump_predict as predict};
    #[test]
    fn orientations_and_order() {
        assert_eq!(train(&[0.0,1.0,2.0,3.0],&[0,0,1,1]),Ok((1.0,true,0)));
        assert_eq!(train(&[0.0,1.0,2.0,3.0],&[1,1,0,0]),Ok((1.0,false,0)));
        assert_eq!(train(&[3.0,0.0,2.0,1.0],&[1,0,1,0]),Ok((1.0,true,0)));
        assert_eq!(predict(2.0,1.0,true),Ok(1));
        assert_eq!(predict(2.0,1.0,false),Ok(0));
    }
    #[test]
    fn constant_and_invalid() {
        assert_eq!(train(&[4.0,4.0],&[1,1]),Ok((4.0,false,0)));
        assert_eq!(train(&[4.0,4.0],&[0,0]),Ok((4.0,true,0)));
        assert_eq!(train(&[4.0,4.0],&[0,1]),Ok((4.0,true,1)));
        assert!(train(&[],&[]).is_err());
        assert!(train(&[0.0],&[]).is_err());
        assert!(train(&[0.0],&[2]).is_err());
        assert!(train(&[f64::NAN],&[0]).is_err());
        assert!(predict(f64::INFINITY,1.0,true).is_err());
    }
}
