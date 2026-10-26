//! Canberra distance computed with scaled finite coordinate terms.

/// Return Canberra distance between equally long nonempty finite vectors.
pub fn canberra_distance(first: &[f64], second: &[f64]) -> Result<f64, &'static str> {
    if first.is_empty() || first.len() != second.len() {
        return Err("equal nonempty vector lengths required");
    }
    let mut distance = 0.0_f64;
    for (&a, &b) in first.iter().zip(second) {
        if !a.is_finite() || !b.is_finite() {
            return Err("finite coordinates required");
        }
        let scale = a.abs().max(b.abs());
        if scale == 0.0 {
            continue;
        }
        let x = a / scale;
        let y = b / scale;
        distance += (x - y).abs() / (x.abs() + y.abs());
        if !distance.is_finite() {
            return Err("nonfinite distance");
        }
    }
    Ok(distance)
}

#[cfg(test)]
mod tests {
    use super::canberra_distance as distance;

    #[test]
    fn fixtures() {
        assert!((distance(&[1.0, 2.0], &[2.0, 4.0]).unwrap() - 2.0/3.0).abs() < 1e-12);
        assert_eq!(distance(&[0.0,0.0], &[0.0,0.0]).unwrap(), 0.0);
        assert_eq!(distance(&[1.0,-1.0], &[-1.0,1.0]).unwrap(), 2.0);
        assert_eq!(distance(&[1e308,-1e308], &[-1e308,1e308]).unwrap(), 2.0);
    }

    #[test]
    fn invalid() {
        assert!(distance(&[], &[]).is_err());
        assert!(distance(&[1.0], &[1.0,2.0]).is_err());
        assert!(distance(&[f64::NAN], &[1.0]).is_err());
        assert!(distance(&[f64::INFINITY], &[1.0]).is_err());
    }
}
