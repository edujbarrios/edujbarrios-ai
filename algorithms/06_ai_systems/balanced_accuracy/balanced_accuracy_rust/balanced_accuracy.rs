//! Binary balanced accuracy (mean of sensitivity and specificity).

pub fn balanced_accuracy(truth: &[u8], predictions: &[u8]) -> Result<f64, &'static str> {
    if truth.is_empty() || truth.len() != predictions.len() {
        return Err("nonempty aligned labels required");
    }
    let (mut positives, mut negatives, mut tp, mut tn) = (0_usize, 0_usize, 0_usize, 0_usize);
    for (&y, &p) in truth.iter().zip(predictions) {
        if y > 1 || p > 1 { return Err("binary labels required"); }
        if y == 1 {
            positives += 1;
            if p == 1 { tp += 1; }
        } else {
            negatives += 1;
            if p == 0 { tn += 1; }
        }
    }
    if positives == 0 || negatives == 0 { return Err("both truth classes required"); }
    Ok(0.5 * (tp as f64 / positives as f64 + tn as f64 / negatives as f64))
}

#[cfg(test)]
mod tests {
    use super::balanced_accuracy as score;

    #[test]
    fn scores() {
        assert!((score(&[0,0,0,1,1], &[0,0,1,1,0]).unwrap() - 7.0/12.0).abs() < 1e-12);
        assert_eq!(score(&[0,1], &[0,1]), Ok(1.0));
        assert_eq!(score(&[0,1], &[1,0]), Ok(0.0));
        assert_eq!(score(&[0,0,1], &[0,0,0]), Ok(0.5));
    }

    #[test]
    fn invalid() {
        assert!(score(&[], &[]).is_err());
        assert!(score(&[0], &[]).is_err());
        assert!(score(&[0,0], &[0,1]).is_err());
        assert!(score(&[1,1], &[1,1]).is_err());
        assert!(score(&[0,1], &[0,2]).is_err());
    }
}
