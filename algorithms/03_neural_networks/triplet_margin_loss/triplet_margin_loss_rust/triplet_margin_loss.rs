//! Unreduced Euclidean triplet margin ranking loss.

/// Return a nonnegative loss, or None if vectors or margin are invalid.
pub fn triplet_margin_loss(
    anchor: &[f64], positive: &[f64], negative: &[f64], margin: f64,
) -> Option<f64> {
    let d = anchor.len();
    if !(1..=512).contains(&d) || positive.len() != d || negative.len() != d ||
        !margin.is_finite() || !(0.0..=1000.0).contains(&margin) ||
        anchor.iter().chain(positive).chain(negative)
            .any(|x| !x.is_finite() || x.abs() > 1_000_000.0) {
        return None;
    }
    let mut pos_sq = 0.0;
    let mut neg_sq = 0.0;
    for i in 0..d {
        pos_sq += (anchor[i] - positive[i]).powi(2);
        neg_sq += (anchor[i] - negative[i]).powi(2);
    }
    Some((pos_sq.sqrt() - neg_sq.sqrt() + margin).max(0.0))
}

#[cfg(test)]
mod tests {
    use super::triplet_margin_loss as loss;

    #[test]
    fn examples() {
        assert_eq!(loss(&[0.0,0.0], &[1.0,0.0], &[3.0,0.0],1.0), Some(0.0));
        assert_eq!(loss(&[0.0,0.0], &[1.0,0.0], &[3.0,0.0],3.0), Some(1.0));
        assert_eq!(loss(&[0.0,0.0], &[3.0,4.0], &[0.0,0.0],1.0), Some(6.0));
        assert_eq!(loss(&[0.0,0.0], &[0.0,0.0], &[0.0,0.0],0.5), Some(0.5));
        assert_eq!(loss(&[-2.0,-2.0], &[-2.0,-2.0], &[-2.0,-2.0],0.0), Some(0.0));
        assert_eq!(loss(&vec![0.0;512], &vec![0.0;512], &vec![1.0;512],0.0), Some(0.0));
        assert!((loss(&vec![0.0;512], &vec![1.0;512], &vec![0.0;512],0.0).unwrap()
                 - (512.0_f64).sqrt()).abs() < 1e-9);
        assert_eq!(loss(&[1.0], &[2.0], &[-2.0],0.0), Some(0.0));
    }

    #[test]
    fn invalid() {
        assert!(loss(&[], &[], &[], 1.0).is_none());
        assert!(loss(&[0.0], &[], &[1.0], 1.0).is_none());
        assert!(loss(&[0.0], &[1.0], &[1.0,2.0], 1.0).is_none());
        assert!(loss(&vec![0.0;513], &vec![0.0;513], &vec![0.0;513], 1.0).is_none());
        assert!(loss(&[0.0], &[0.0], &[1.0], -1.0).is_none());
        assert!(loss(&[0.0], &[0.0], &[1.0], 1001.0).is_none());
        assert!(loss(&[0.0], &[0.0], &[1.0], f64::INFINITY).is_none());
        assert!(loss(&[0.0], &[f64::NAN], &[1.0], 1.0).is_none());
        assert!(loss(&[0.0], &[1_000_001.0], &[1.0], 1.0).is_none());
    }
}
