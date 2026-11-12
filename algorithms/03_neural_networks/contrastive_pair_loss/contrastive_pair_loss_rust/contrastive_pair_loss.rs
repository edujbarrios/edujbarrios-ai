//! Euclidean contrastive loss for one binary-labeled embedding pair.

/// One-pair contrastive loss, or None if dimensions, coordinates or margin are invalid.
pub fn contrastive_pair_loss(first: &[f64], second: &[f64],
                             label: u8, margin: f64) -> Option<f64> {
    if !(1..=512).contains(&first.len()) || first.len() != second.len() ||
        label > 1 || !margin.is_finite() || !(0.0..=1000.0).contains(&margin) ||
        first.iter().chain(second.iter()).any(|x| !x.is_finite() || x.abs() > 1000.0) {
        return None;
    }
    let squared: f64 = first.iter().zip(second).map(|(a,b)| (a-b).powi(2)).sum();
    let gap = (margin - squared.sqrt()).max(0.0);
    Some(0.5 * if label == 1 { squared } else { gap*gap })
}

#[cfg(test)]
mod tests {
    use super::contrastive_pair_loss as loss;

    #[test]
    fn examples() {
        assert_eq!(loss(&[0.0,0.0],&[3.0,4.0],1,6.0),Some(12.5));
        assert_eq!(loss(&[0.0,0.0],&[3.0,4.0],0,6.0),Some(0.5));
        assert_eq!(loss(&[0.0,0.0],&[3.0,4.0],0,3.0),Some(0.0));
        assert_eq!(loss(&[1.0,2.0],&[1.0,2.0],0,2.0),Some(2.0));
        assert_eq!(loss(&[1.0,2.0],&[1.0,2.0],1,2.0),Some(0.0));
        assert_eq!(loss(&[0.0,0.0],&[3.0,4.0],0,0.0),Some(0.0));
        assert_eq!(loss(&vec![0.0;512],&vec![1.0;512],1,0.0),Some(256.0));
        assert_eq!(loss(&vec![0.0;512],&vec![1.0;512],0,1.0),Some(0.0));
    }

    #[test]
    fn invalid() {
        assert!(loss(&[],&[],1,1.0).is_none());
        assert!(loss(&[0.0],&[0.0,1.0],1,1.0).is_none());
        assert!(loss(&vec![0.0;513],&vec![0.0;513],1,1.0).is_none());
        assert!(loss(&[0.0],&[0.0],2,1.0).is_none());
        assert!(loss(&[0.0],&[0.0],1,-1.0).is_none());
        assert!(loss(&[0.0],&[0.0],1,1001.0).is_none());
        assert!(loss(&[0.0],&[0.0],1,f64::INFINITY).is_none());
        assert!(loss(&[f64::NAN],&[0.0],1,1.0).is_none());
        assert!(loss(&[0.0],&[1001.0],1,1.0).is_none());
    }
}
