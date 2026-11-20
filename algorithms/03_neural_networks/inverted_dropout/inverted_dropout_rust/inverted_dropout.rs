//! Deterministic inverted dropout forward pass using a caller-provided mask.

/// Scale retained values by 1/(1-p), zero masked values.
/// Reject invalid masks, probabilities and nonfinite resulting outputs.
pub fn inverted_dropout(
    activations: &[f64], mask: &[u8], probability: f64,
) -> Result<Vec<f64>, &'static str> {
    if activations.len() != mask.len() || !probability.is_finite() ||
        !(0.0..1.0).contains(&probability) ||
        activations.iter().any(|x| !x.is_finite()) ||
        mask.iter().any(|&bit| bit > 1) {
        return Err("aligned finite activations, binary mask and p in [0,1) required");
    }
    let keep = 1.0 - probability;
    let mut result = Vec::with_capacity(activations.len());
    for (&value, &bit) in activations.iter().zip(mask.iter()) {
        let scaled = if bit == 1 { value / keep } else { 0.0 };
        if !scaled.is_finite() {
            return Err("retained activation scaling overflow");
        }
        result.push(scaled);
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::inverted_dropout as dropout;

    #[test]
    fn forward() {
        assert_eq!(dropout(&[2.0,-4.0,0.0], &[1,0,1], 0.5),
                   Ok(vec![4.0,0.0,0.0]));
        assert_eq!(dropout(&[3.0,-2.0], &[1,1], 0.0), Ok(vec![3.0,-2.0]));
        assert_eq!(dropout(&[1.0e308], &[0], 0.99), Ok(vec![0.0]));
        assert_eq!(dropout(&[], &[], 0.5), Ok(vec![]));
    }

    #[test]
    fn invalid() {
        assert!(dropout(&[1.0], &[], 0.5).is_err());
        assert!(dropout(&[1.0], &[2], 0.5).is_err());
        assert!(dropout(&[f64::NAN], &[0], 0.5).is_err());
        assert!(dropout(&[f64::INFINITY], &[1], 0.5).is_err());
        assert!(dropout(&[1.0], &[1], -0.1).is_err());
        assert!(dropout(&[1.0], &[1], 1.0).is_err());
        assert!(dropout(&[1.0], &[1], f64::NAN).is_err());
        assert!(dropout(&[1.0e308], &[1], 0.99).is_err());
    }
}
