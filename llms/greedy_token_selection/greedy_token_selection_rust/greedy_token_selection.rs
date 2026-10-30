//! Zero-based greedy next-token selection with first-index tie breaking.

/// Return the first maximum-logit index or error for empty/nonfinite logits.
pub fn greedy_token_selection(logits: &[f64]) -> Result<usize, &'static str> {
    if logits.is_empty() {
        return Err("logits must be nonempty");
    }
    let mut maximum = f64::NEG_INFINITY;
    let mut selected = 0;
    for (i, &score) in logits.iter().enumerate() {
        if !score.is_finite() {
            return Err("logits must be finite");
        }
        if score > maximum {
            maximum = score;
            selected = i;
        }
    }
    Ok(selected)
}

#[cfg(test)]
mod tests {
    use super::greedy_token_selection as greedy;

    #[test]
    fn ties_and_negatives() {
        assert_eq!(greedy(&[2.0, 5.0, 5.0, -1.0]), Ok(1));
        assert_eq!(greedy(&[-4.0, -2.0, -3.0]), Ok(1));
        assert_eq!(greedy(&[1.0, 1.0, 1.0]), Ok(0));
    }

    #[test]
    fn extremes_and_invalid() {
        assert_eq!(greedy(&[-1e308]), Ok(0));
        assert_eq!(greedy(&[-1e308, 1e308]), Ok(1));
        assert!(greedy(&[]).is_err());
        assert!(greedy(&[1.0, f64::NAN]).is_err());
        assert!(greedy(&[f64::INFINITY]).is_err());
    }
}
