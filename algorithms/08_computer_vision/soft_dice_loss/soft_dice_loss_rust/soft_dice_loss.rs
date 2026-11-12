//! Binary segmentation soft Dice objective with fixed additive smoothing.

/// Return smoothed Dice loss, or None for invalid lengths/probabilities/targets.
pub fn soft_dice_loss(predictions: &[f64], targets: &[u8]) -> Option<f64> {
    if !(1..=512).contains(&predictions.len()) ||
        predictions.len() != targets.len() ||
        predictions.iter().any(|x| !x.is_finite() || !(0.0..=1.0).contains(x)) ||
        targets.iter().any(|&t| t > 1) {
        return None;
    }
    let mut overlap=0.0;
    let mut total_p=0.0;
    let mut total_t=0.0;
    for (&p,&t) in predictions.iter().zip(targets) {
        overlap += p * f64::from(t);
        total_p += p;
        total_t += f64::from(t);
    }
    Some(1.0 - (2.0*overlap+1.0)/(total_p+total_t+1.0))
}

#[cfg(test)]
mod tests {
    use super::soft_dice_loss as loss;

    #[test]
    fn examples() {
        assert_eq!(loss(&[0.0,0.0],&[0,0]),Some(0.0));
        assert_eq!(loss(&[1.0,0.0],&[1,0]),Some(0.0));
        assert!((loss(&[0.0,1.0],&[1,0]).unwrap()-2.0/3.0).abs()<1e-12);
        assert!((loss(&[0.5,0.5],&[1,0]).unwrap()-1.0/3.0).abs()<1e-12);
        assert!((loss(&[1.0,1.0],&[0,0]).unwrap()-2.0/3.0).abs()<1e-12);
        assert_eq!(loss(&[0.0,0.0],&[1,0]),Some(0.5));
        assert_eq!(loss(&[1.0],&[1]),Some(0.0));
        assert_eq!(loss(&vec![0.0;512],&vec![0;512]),Some(0.0));
        assert_eq!(loss(&vec![1.0;512],&vec![1;512]),Some(0.0));
    }
    #[test]
    fn invalid() {
        assert!(loss(&[],&[]).is_none());
        assert!(loss(&[0.0],&[0,1]).is_none());
        assert!(loss(&vec![0.0;513],&vec![0;513]).is_none());
        assert!(loss(&[f64::NAN],&[0]).is_none());
        assert!(loss(&[f64::INFINITY],&[0]).is_none());
        assert!(loss(&[-0.1],&[0]).is_none());
        assert!(loss(&[1.1],&[1]).is_none());
        assert!(loss(&[0.0],&[2]).is_none());
    }
}
