//! Intersection over Union of aligned binary segmentation masks.

/// Returns 1 for two all-background masks and errors on invalid masks.
pub fn binary_iou(prediction: &[u8], target: &[u8]) -> Result<f64, &'static str> {
    if prediction.is_empty() || prediction.len() != target.len() {
        return Err("masks must be nonempty and have equal length");
    }
    let mut intersection = 0usize;
    let mut union = 0usize;
    for (&a, &b) in prediction.iter().zip(target.iter()) {
        if a > 1 || b > 1 {
            return Err("mask values must be zero or one");
        }
        if a == 1 && b == 1 {
            intersection += 1;
        }
        if a == 1 || b == 1 {
            union += 1;
        }
    }
    if union == 0 {
        Ok(1.0)
    } else {
        Ok(intersection as f64 / union as f64)
    }
}

#[cfg(test)]
mod tests {
    use super::binary_iou;

    #[test]
    fn overlap() {
        let score = binary_iou(&[1, 1, 0, 0], &[1, 0, 1, 0]).unwrap();
        assert!((score - 1.0 / 3.0).abs() < 1e-12);
    }

    #[test]
    fn boundaries() {
        assert_eq!(binary_iou(&[1, 0], &[1, 0]), Ok(1.0));
        assert_eq!(binary_iou(&[0, 0], &[0, 0]), Ok(1.0));
        assert_eq!(binary_iou(&[1, 0], &[0, 1]), Ok(0.0));
    }

    #[test]
    fn invalid() {
        assert!(binary_iou(&[], &[]).is_err());
        assert!(binary_iou(&[0], &[0, 1]).is_err());
        assert!(binary_iou(&[2], &[0]).is_err());
    }
}
