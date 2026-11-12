//! One-dimensional Gaussian kernel density estimation.

/// Equal-weight KDE at a single query, or None on invalid input.
pub fn gaussian_kde_1d(samples: &[f64], query: f64, bandwidth: f64) -> Option<f64> {
    if !(1..=512).contains(&samples.len()) || !query.is_finite() ||
        query.abs() > 1000.0 || !bandwidth.is_finite() ||
        !(0.001..=1000.0).contains(&bandwidth) ||
        samples.iter().any(|x| !x.is_finite() || x.abs() > 1000.0) {
        return None;
    }
    let total: f64 = samples.iter().map(|x| {
        let z = (query - x) / bandwidth;
        (-0.5 * z * z).exp()
    }).sum();
    Some(total / (samples.len() as f64 * bandwidth * (2.0 * std::f64::consts::PI).sqrt()))
}

#[cfg(test)]
mod tests {
    use super::gaussian_kde_1d as kde;

    #[test]
    fn examples() {
        let s = (2.0 * std::f64::consts::PI).sqrt();
        assert!((kde(&[0.0],0.0,1.0).unwrap() - 1.0/s).abs() < 1e-12);
        assert!((kde(&[-1.0,1.0],0.0,1.0).unwrap() - (-0.5_f64).exp()/s).abs() < 1e-12);
        assert!((kde(&[1.0,1.0],1.0,2.0).unwrap() - 1.0/(2.0*s)).abs() < 1e-12);
        assert_eq!(kde(&[0.0],1000.0,0.001),Some(0.0));
        assert!((kde(&vec![0.0;512],0.0,1.0).unwrap() - 1.0/s).abs() < 1e-12);
        assert!((kde(&[0.0],-1000.0,1000.0).unwrap()
                 - (-0.5_f64).exp()/(1000.0*s)).abs() < 1e-12);
    }
    #[test]
    fn invalid() {
        assert!(kde(&[],0.0,1.0).is_none());
        assert!(kde(&vec![0.0;513],0.0,1.0).is_none());
        assert!(kde(&[f64::NAN],0.0,1.0).is_none());
        assert!(kde(&[f64::INFINITY],0.0,1.0).is_none());
        assert!(kde(&[1001.0],0.0,1.0).is_none());
        assert!(kde(&[0.0],f64::NAN,1.0).is_none());
        assert!(kde(&[0.0],1001.0,1.0).is_none());
        assert!(kde(&[0.0],0.0,0.0).is_none());
        assert!(kde(&[0.0],0.0,0.0001).is_none());
        assert!(kde(&[0.0],0.0,1001.0).is_none());
        assert!(kde(&[0.0],0.0,f64::INFINITY).is_none());
    }
}
