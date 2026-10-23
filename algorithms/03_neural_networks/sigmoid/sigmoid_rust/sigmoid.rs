//! Stable elementwise logistic sigmoid for finite real inputs.

pub fn sigmoid(values: &[f64]) -> Result<Vec<f64>, &'static str> {
    if !values.iter().all(|x| x.is_finite()) {
        return Err("components must be finite");
    }
    Ok(values.iter().map(|&x| {
        if x >= 0.0 {
            1.0 / (1.0 + (-x).exp())
        } else {
            let ex = x.exp();
            ex / (1.0 + ex)
        }
    }).collect())
}

#[cfg(test)]
mod tests {
    use super::sigmoid;

    #[test]
    fn typical_and_extreme_values() {
        let v = sigmoid(&[-1.,0.,1.]).unwrap();
        assert!((v[0]-0.2689414213699951).abs()<1e-12);
        assert_eq!(v[1], 0.5);
        assert!((v[2]-0.7310585786300049).abs()<1e-12);
        assert_eq!(sigmoid(&[-1e308,1e308]), Ok(vec![0.,1.]));
        assert_eq!(sigmoid(&[]), Ok(vec![]));
    }
    #[test]
    fn invalid() {
        assert!(sigmoid(&[1., f64::NAN]).is_err());
        assert!(sigmoid(&[f64::INFINITY]).is_err());
    }
}
