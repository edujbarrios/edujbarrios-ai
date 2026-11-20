//! Empirical mutual information for two aligned binary variables, in bits.

/// Skip zero joint-probability cells; reject invalid or empty input.
pub fn binary_mutual_information(x: &[u8], y: &[u8]) -> Result<f64, &'static str> {
    if x.is_empty() || x.len() != y.len() {
        return Err("nonempty aligned binary vectors required");
    }
    let mut counts = [[0usize; 2]; 2];
    for (&a, &b) in x.iter().zip(y.iter()) {
        if a > 1 || b > 1 {
            return Err("values must be binary");
        }
        counts[a as usize][b as usize] += 1;
    }
    let n = x.len() as f64;
    let rows = [counts[0][0] + counts[0][1], counts[1][0] + counts[1][1]];
    let cols = [counts[0][0] + counts[1][0], counts[0][1] + counts[1][1]];
    let mut result = 0.0_f64;
    for a in 0..2 {
        for (b, &col) in cols.iter().enumerate() {
            let c = counts[a][b];
            if c != 0 {
                let p = c as f64 / n;
                let px = rows[a] as f64 / n;
                let py = col as f64 / n;
                result += p * (p / (px * py)).log2();
            }
        }
    }
    if !result.is_finite() {
        return Err("nonfinite mutual information");
    }
    Ok(result.max(0.0))
}

#[cfg(test)]
mod tests {
    use super::binary_mutual_information as mi;

    #[test]
    fn examples() {
        assert!((mi(&[0,0,1,1], &[0,0,1,1]).unwrap() - 1.0).abs() < 1e-12);
        assert!((mi(&[0,0,1,1], &[1,1,0,0]).unwrap() - 1.0).abs() < 1e-12);
        assert!(mi(&[0,0,1,1], &[0,1,0,1]).unwrap().abs() < 1e-12);
        assert_eq!(mi(&[0,0,0], &[1,0,1]), Ok(0.0));
        assert_eq!(mi(&[1], &[0]), Ok(0.0));
        assert!((mi(&[0,0,1,1], &[0,0,0,1]).unwrap() - 0.31127812445913283).abs() < 1e-12);
    }

    #[test]
    fn invalid() {
        assert!(mi(&[], &[]).is_err());
        assert!(mi(&[0,1], &[1]).is_err());
        assert!(mi(&[0,2], &[1,0]).is_err());
        assert!(mi(&[1,1], &[0,3]).is_err());
    }
}
