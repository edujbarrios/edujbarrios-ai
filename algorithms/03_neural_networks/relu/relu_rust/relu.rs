//! Elementwise rectified-linear forward activation.

pub fn relu(values: &[f64]) -> Result<Vec<f64>, &'static str> {
    if !values.iter().all(|v| v.is_finite()) {
        return Err("components must be finite");
    }
    Ok(values.iter().map(|&x| x.max(0.0)).collect())
}

#[cfg(test)]
mod tests {
    use super::relu;

    #[test]
    fn regular_and_empty() {
        assert_eq!(relu(&[-2.,-0.5,0.,3.]), Ok(vec![0.,0.,0.,3.]));
        assert_eq!(relu(&[]), Ok(vec![]));
    }

    #[test]
    fn invalid() {
        assert!(relu(&[1.,f64::NAN]).is_err());
        assert!(relu(&[f64::INFINITY]).is_err());
    }
}
