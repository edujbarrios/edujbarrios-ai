//! Spearman rank correlation with average ranks for tied observations.

fn midranks(values: &[f64]) -> Vec<f64> {
    values.iter().map(|&current| {
        let less = values.iter().filter(|&&other| other < current).count();
        let equal = values.iter().filter(|&&other| other == current).count();
        less as f64 + (equal as f64 + 1.0) / 2.0
    }).collect()
}

/// Return rho for paired finite observations, rejecting undefined ranks.
pub fn spearman_rank_correlation(x: &[f64], y: &[f64]) -> Result<f64, &'static str> {
    let n = x.len();
    if !(2..=128).contains(&n) || y.len() != n {
        return Err("equal lengths between 2 and 128 required");
    }
    if x.iter().chain(y.iter()).any(|v| !v.is_finite() || v.abs() > 1_000_000.0) {
        return Err("finite observations in [-1000000,1000000] required");
    }
    let rx = midranks(x);
    let ry = midranks(y);
    let mean = (n as f64 + 1.0) / 2.0;
    let (mut sxx, mut syy, mut sxy) = (0.0, 0.0, 0.0);
    for (&a, &b) in rx.iter().zip(ry.iter()) {
        let dx = a - mean;
        let dy = b - mean;
        sxx += dx * dx;
        syy += dy * dy;
        sxy += dx * dy;
    }
    if sxx == 0.0 || syy == 0.0 {
        return Err("correlation undefined for constant ranks");
    }
    let rho: f64 = sxy / sxx.sqrt() / syy.sqrt();
    if !rho.is_finite() {
        return Err("nonfinite correlation");
    }
    Ok(rho.clamp(-1.0, 1.0))
}

#[cfg(test)]
mod tests {
    use super::spearman_rank_correlation as corr;

    fn close(actual: f64, expected: f64) {
        assert!((actual - expected).abs() < 1e-12, "{actual} vs {expected}");
    }

    #[test]
    fn reference_cases() {
        close(corr(&[1.0, 2.0, 3.0], &[4.0, 5.0, 6.0]).unwrap(), 1.0);
        close(corr(&[1.0, 2.0, 3.0], &[6.0, 5.0, 4.0]).unwrap(), -1.0);
        close(corr(&[1.0, 2.0, 3.0], &[1.0, 3.0, 2.0]).unwrap(), 0.5);
        close(corr(&[1.0, 1.0, 2.0, 3.0], &[1.0, 2.0, 2.0, 3.0]).unwrap(), 5.0 / 6.0);
        close(corr(&[1.0, 2.0], &[2.0, 1.0]).unwrap(), -1.0);
    }

    #[test]
    fn invalid_cases() {
        assert!(corr(&[], &[]).is_err());
        assert!(corr(&[1.0], &[1.0]).is_err());
        assert!(corr(&[1.0, 2.0], &[1.0]).is_err());
        assert!(corr(&[1.0, 2.0, 3.0], &[7.0, 7.0, 7.0]).is_err());
        assert!(corr(&[1.0, 2.0], &[1.0, f64::NAN]).is_err());
        assert!(corr(&[1.0, 2.0], &[1.0, f64::INFINITY]).is_err());
        assert!(corr(&[1.0, 2.0], &[1.0, 1_000_001.0]).is_err());
        assert!(corr(&vec![1.0; 129], &vec![2.0; 129]).is_err());
    }
}
