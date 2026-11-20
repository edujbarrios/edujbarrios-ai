//! Exact binary precision–recall curve, grouped at distinct probability thresholds.

/// Return descending (threshold, precision, recall) points.
/// Scores must be finite probabilities and labels binary, with at least one positive.
pub fn precision_recall_curve(
    scores: &[f64],
    labels: &[u8],
) -> Result<Vec<(f64, f64, f64)>, &'static str> {
    if scores.is_empty() || scores.len() != labels.len() ||
        scores.iter().any(|s| !s.is_finite() || !(0.0..=1.0).contains(s)) ||
        labels.iter().any(|&y| y > 1) {
        return Err("nonempty aligned finite probabilities and binary labels required");
    }
    let positives = labels.iter().filter(|&&y| y == 1).count();
    if positives == 0 {
        return Err("at least one positive label required");
    }
    let mut pairs: Vec<(f64, u8)> =
        scores.iter().copied().zip(labels.iter().copied()).collect();
    pairs.sort_by(|a, b| b.0.total_cmp(&a.0));
    let mut points = Vec::new();
    let mut end = 0;
    let mut tp = 0usize;
    while end < pairs.len() {
        let score = pairs[end].0;
        while end < pairs.len() && pairs[end].0 == score {
            tp += pairs[end].1 as usize;
            end += 1;
        }
        points.push((score, tp as f64 / end as f64, tp as f64 / positives as f64));
    }
    Ok(points)
}

#[cfg(test)]
mod tests {
    use super::precision_recall_curve as curve;

    #[test]
    fn distinct_thresholds_and_ties() {
        let points = curve(&[0.8, 0.8, 0.5, 0.2], &[1, 0, 1, 0]).unwrap();
        assert_eq!(points.len(), 3);
        assert_eq!(points[0], (0.8, 0.5, 0.5));
        assert_eq!(points[1].0, 0.5);
        assert!((points[1].1 - 2.0 / 3.0).abs() < 1e-12);
        assert_eq!(points[1].2, 1.0);
        assert_eq!(points[2], (0.2, 0.5, 1.0));
        assert_eq!(curve(&[1.0], &[1]), Ok(vec![(1.0, 1.0, 1.0)]));
        assert_eq!(curve(&[0.4, 0.4], &[1, 1]), Ok(vec![(0.4, 1.0, 1.0)]));
        assert_eq!(curve(&[0.0, 1.0], &[1, 0]),
                   Ok(vec![(1.0, 0.0, 0.0), (0.0, 0.5, 1.0)]));
    }

    #[test]
    fn invalid() {
        assert!(curve(&[], &[]).is_err());
        assert!(curve(&[0.5], &[]).is_err());
        assert!(curve(&[0.5], &[0]).is_err());
        assert!(curve(&[-0.1], &[1]).is_err());
        assert!(curve(&[1.1], &[1]).is_err());
        assert!(curve(&[f64::NAN], &[1]).is_err());
        assert!(curve(&[0.5], &[2]).is_err());
    }
}
