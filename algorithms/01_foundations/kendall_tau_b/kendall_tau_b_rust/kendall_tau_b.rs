//! Pairwise Kendall tau-b with explicit tie adjustment.

/// Returns Kendall tau-b for two bounded vectors of paired finite observations.
pub fn kendall_tau_b(x: &[f64], y: &[f64]) -> Result<f64, &'static str> {
    let n = x.len();
    if !(2..=128).contains(&n) || y.len() != n {
        return Err("equal input lengths between 2 and 128 required");
    }
    if x.iter().chain(y.iter()).any(|z| !z.is_finite() || z.abs() > 1_000_000.0) {
        return Err("observations must be bounded and finite");
    }
    let (mut c, mut d, mut tx, mut ty) = (0.0_f64, 0.0_f64, 0.0_f64, 0.0_f64);
    for i in 0..n {
        for j in i + 1..n {
            let sx = (x[i] > x[j]) as i8 - (x[i] < x[j]) as i8;
            let sy = (y[i] > y[j]) as i8 - (y[i] < y[j]) as i8;
            if sx == 0 && sy == 0 { continue; }
            if sx == 0 { tx += 1.0; }
            else if sy == 0 { ty += 1.0; }
            else if sx == sy { c += 1.0; }
            else { d += 1.0; }
        }
    }
    let denom = ((c + d + tx) * (c + d + ty)).sqrt();
    if denom == 0.0 {
        return Err("tau-b undefined for constant ranks");
    }
    Ok(((c - d) / denom).clamp(-1.0, 1.0))
}

#[cfg(test)]
mod tests {
    use super::kendall_tau_b as tau;
    fn close(actual: f64, expected: f64) {
        assert!((actual - expected).abs() < 1e-12, "{actual} != {expected}");
    }
    #[test]
    fn reference_values() {
        close(tau(&[1.,2.,3.], &[2.,3.,4.]).unwrap(), 1.);
        close(tau(&[1.,2.,3.], &[4.,3.,2.]).unwrap(), -1.);
        close(tau(&[1.,2.,3.], &[1.,3.,2.]).unwrap(), 1./3.);
        close(tau(&[1.,1.,2.,3.], &[1.,2.,2.,3.]).unwrap(), 0.8);
        close(tau(&[1.,2.], &[1.,2.]).unwrap(), 1.);
    }
    #[test]
    fn invalid_values() {
        assert!(tau(&[],&[]).is_err());
        assert!(tau(&[1.],&[1.]).is_err());
        assert!(tau(&[1.,2.],&[1.]).is_err());
        assert!(tau(&[1.,2.,3.],&[7.,7.,7.]).is_err());
        assert!(tau(&[7.,7.,7.],&[1.,2.,3.]).is_err());
        assert!(tau(&[1.,2.],&[1.,f64::NAN]).is_err());
        assert!(tau(&[1.,2.],&[1.,f64::INFINITY]).is_err());
        assert!(tau(&[1.,2.],&[1.,1_000_001.]).is_err());
        assert!(tau(&vec![1.;129],&vec![1.;129]).is_err());
    }
}
