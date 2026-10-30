//! Overflow-resistant global L2 norm clipping for a flat gradient vector.

/// Returns a clipped vector, or an error on invalid input.
pub fn gradient_clip_global_norm(gradients: &[f64], max_norm: f64) -> Result<Vec<f64>, &'static str> {
    if gradients.is_empty() || !max_norm.is_finite() || max_norm <= 0.0 ||
       !gradients.iter().all(|x| x.is_finite()) {
        return Err("gradients must be finite, nonempty, and max_norm positive");
    }
    let maximum = gradients.iter().map(|x| x.abs()).fold(0.0f64, f64::max);
    if maximum == 0.0 {
        return Ok(gradients.to_vec());
    }
    let squares: f64 = gradients.iter().map(|x| {
        let scaled = x / maximum;
        scaled * scaled
    }).sum();
    let factor = ((max_norm / maximum) / squares.sqrt()).min(1.0);
    Ok(gradients.iter().map(|x| x * factor).collect())
}

#[cfg(test)]
mod tests {
    use super::gradient_clip_global_norm as clip;

    #[test]
    fn simple() {
        let x = clip(&[3.0, 4.0], 1.0).unwrap();
        assert!((x[0] - 0.6).abs() < 1e-12 && (x[1] - 0.8).abs() < 1e-12);
        assert_eq!(clip(&[3.0, 4.0], 5.0), Ok(vec![3.0, 4.0]));
        assert_eq!(clip(&[0.0, 0.0], 1.0), Ok(vec![0.0, 0.0]));
    }

    #[test]
    fn extreme() {
        let x = clip(&[1e308, -1e308], 1.0).unwrap();
        assert!((x[0] - (0.5f64).sqrt()).abs() < 1e-12);
        assert!((x[1] + (0.5f64).sqrt()).abs() < 1e-12);
    }

    #[test]
    fn invalid() {
        assert!(clip(&[], 1.0).is_err());
        assert!(clip(&[1.0], 0.0).is_err());
        assert!(clip(&[1.0], f64::INFINITY).is_err());
        assert!(clip(&[f64::NAN], 1.0).is_err());
    }
}
