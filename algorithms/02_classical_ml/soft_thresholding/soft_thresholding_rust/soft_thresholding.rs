//! Coordinate-wise L1 proximal shrinkage for finite vectors.
/// Return a new vector, or Err for empty/nonfinite input or negative threshold.
pub fn soft_thresholding(values: &[f64], threshold: f64) -> Result<Vec<f64>, &'static str> {
    if values.is_empty() || !threshold.is_finite() || threshold < 0.0 ||
       !values.iter().all(|x| x.is_finite()) {
        return Err("values must be finite and nonempty; threshold finite and nonnegative");
    }
    Ok(values.iter().map(|&x| {
        if x > threshold { x - threshold }
        else if x < -threshold { x + threshold }
        else { 0.0 }
    }).collect())
}
#[cfg(test)]
mod tests {
    use super::soft_thresholding as shrink;
    #[test] fn shrink_and_boundaries() {
        assert_eq!(shrink(&[-3.,-0.5,0.,0.5,3.],1.), Ok(vec![-2.,0.,0.,0.,2.]));
        assert_eq!(shrink(&[-1.,1.],1.), Ok(vec![0.,0.]));
        assert_eq!(shrink(&[-3.,3.],0.), Ok(vec![-3.,3.]));
        assert_eq!(shrink(&[1e308,-1e308],1.), Ok(vec![1e308,-1e308]));
    }
    #[test] fn invalid() {
        assert!(shrink(&[],1.).is_err());
        assert!(shrink(&[f64::NAN],1.).is_err());
        assert!(shrink(&[1.],-1.).is_err());
        assert!(shrink(&[1.],f64::INFINITY).is_err());
    }
}
