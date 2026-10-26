//! Positive-class F-beta score using a numerically stable weighting.

/// Return F-beta in [0,1] for nonnegative confusion counts and beta > 0.
pub fn f_beta_score(tp: i32, fp: i32, fn_: i32, beta: f64) -> Result<f64, &'static str> {
    if tp < 0 || fp < 0 || fn_ < 0 || !beta.is_finite() || beta <= 0.0 {
        return Err("nonnegative counts and positive finite beta required");
    }
    if tp == 0 {
        return Ok(0.0);
    }
    let precision = f64::from(tp) / (f64::from(tp) + f64::from(fp));
    let recall = f64::from(tp) / (f64::from(tp) + f64::from(fn_));
    let weight = if beta <= 1.0 {
        let b2 = beta * beta;
        b2 / (1.0 + b2)
    } else {
        let inverse = 1.0 / beta;
        1.0 / (1.0 + inverse * inverse)
    };
    Ok(precision * recall / (weight * precision + (1.0 - weight) * recall))
}

#[cfg(test)]
mod tests {
    use super::f_beta_score as score;
    #[test]
    fn fixtures() {
        assert!((score(8, 2, 4, 1.0).unwrap() - 8.0 / 11.0).abs() < 1e-12);
        assert!((score(8, 2, 4, 2.0).unwrap() - 20.0 / 29.0).abs() < 1e-12);
        assert!((score(8, 2, 4, 0.5).unwrap() - 10.0 / 13.0).abs() < 1e-12);
        assert!((score(8, 2, 4, 1e-300).unwrap() - 0.8).abs() < 1e-12);
        assert!((score(8, 2, 4, 1e300).unwrap() - 2.0 / 3.0).abs() < 1e-12);
        assert_eq!(score(0, 0, 0, 1.0).unwrap(), 0.0);
        assert_eq!(score(i32::MAX, 0, 0, 1.0).unwrap(), 1.0);
    }
    #[test]
    fn invalid() {
        assert!(score(-1, 0, 0, 1.0).is_err());
        assert!(score(1, 0, 0, 0.0).is_err());
        assert!(score(1, 0, 0, f64::NAN).is_err());
        assert!(score(1, 0, 0, f64::INFINITY).is_err());
    }
}
