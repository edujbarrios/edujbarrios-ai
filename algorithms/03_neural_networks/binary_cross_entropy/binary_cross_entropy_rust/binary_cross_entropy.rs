//! Mean binary cross-entropy for validated probabilities and binary labels.

/// Return the finite mean BCE, rejecting impossible events and invalid values.
pub fn binary_cross_entropy(labels: &[f64], probabilities: &[f64]) -> Result<f64, &'static str> {
    if labels.is_empty() || labels.len() != probabilities.len() {
        return Err("expected equal, nonempty arrays");
    }

    let mut total = 0.0_f64;
    for (&label, &probability) in labels.iter().zip(probabilities) {
        if !label.is_finite()
            || !probability.is_finite()
            || (label != 0.0 && label != 1.0)
            || !(0.0..=1.0).contains(&probability)
        {
            return Err("invalid label or probability");
        }

        // ln_1p(-p) remains accurate for probabilities close to zero.
        let term = if label == 1.0 {
            -probability.ln()
        } else {
            -(-probability).ln_1p()
        };
        if !term.is_finite() {
            return Err("infinite loss");
        }
        total += term;
        if !total.is_finite() {
            return Err("loss overflow");
        }
    }

    Ok(total / labels.len() as f64)
}

#[cfg(test)]
mod tests {
    use super::binary_cross_entropy as bce;

    #[test]
    fn known_probabilities() {
        assert_eq!(bce(&[1.0, 0.0], &[1.0, 0.0]), Ok(0.0));
        assert!((bce(&[1.0, 0.0], &[0.5, 0.5]).unwrap() - 2.0_f64.ln()).abs() < 1e-12);
        assert!(bce(&[0.0], &[1e-12]).unwrap() > 0.0);
    }

    #[test]
    fn invalid_inputs() {
        assert!(bce(&[], &[]).is_err());
        assert!(bce(&[1.0], &[0.0]).is_err());
        assert!(bce(&[0.0], &[1.0]).is_err());
        assert!(bce(&[2.0], &[0.5]).is_err());
        assert!(bce(&[f64::NAN], &[0.5]).is_err());
    }
}
