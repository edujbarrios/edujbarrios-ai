//! Sparsemax projection of a nonempty finite vector onto the probability simplex.

/// Returns sparse probabilities, or an error for empty or non-finite input.
pub fn sparsemax(logits: &[f64]) -> Result<Vec<f64>, &'static str> {
    if logits.is_empty() || !logits.iter().all(|x| x.is_finite()) {
        return Err("logits must be nonempty and finite");
    }
    let maximum = logits.iter().copied().fold(f64::NEG_INFINITY, f64::max);
    let shifted: Vec<f64> = logits.iter().map(|&x| x - maximum).collect();
    let mut sorted = shifted.clone();
    sorted.sort_by(|a, b| b.total_cmp(a));

    let mut prefix = 0.0;
    let mut threshold = 0.0;
    for (index, &value) in sorted.iter().enumerate() {
        let k = (index + 1) as f64;
        prefix += value;
        if 1.0 + k * value > prefix {
            threshold = (prefix - 1.0) / k;
        }
    }
    Ok(shifted.iter().map(|&x| (x - threshold).max(0.0)).collect())
}

#[cfg(test)]
mod tests {
    use super::sparsemax;

    #[test]
    fn sparse_and_uniform() {
        assert_eq!(sparsemax(&[0.0, 0.0, -1.0]), Ok(vec![0.5, 0.5, 0.0]));
        assert_eq!(sparsemax(&[3.0, 1.0, 0.0]), Ok(vec![1.0, 0.0, 0.0]));
        assert_eq!(sparsemax(&[100.0, 100.0, 99.0]), Ok(vec![0.5, 0.5, 0.0]));
        let uniform = sparsemax(&[1.0, 1.0, 1.0]).unwrap();
        assert!(uniform.iter().all(|&x| (x - 1.0 / 3.0).abs() < 1e-12));
    }

    #[test]
    fn extremes_and_translation() {
        assert_eq!(sparsemax(&[1e308]), Ok(vec![1.0]));
        assert_eq!(sparsemax(&[1e308, -1e308]), Ok(vec![1.0, 0.0]));
        let a = sparsemax(&[2.0, 1.0, 0.0]).unwrap();
        let b = sparsemax(&[102.0, 101.0, 100.0]).unwrap();
        assert!(a.iter().zip(b).all(|(x, y)| (x - y).abs() < 1e-12));
    }

    #[test]
    fn invalid() {
        assert!(sparsemax(&[]).is_err());
        assert!(sparsemax(&[f64::NAN]).is_err());
        assert!(sparsemax(&[f64::INFINITY]).is_err());
    }
}
