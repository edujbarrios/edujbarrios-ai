//! Independent symmetric element-wise clipping of a finite gradient vector.
/// Return a new clipped vector, or Err on invalid input.
pub fn gradient_clip_by_value(gradients: &[f64], limit: f64) -> Result<Vec<f64>, &'static str> {
    if gradients.is_empty() || !limit.is_finite() || limit <= 0.0 ||
       !gradients.iter().all(|x| x.is_finite()) {
        return Err("finite nonempty gradients and positive finite limit required");
    }
    Ok(gradients.iter().map(|&x| x.max(-limit).min(limit)).collect())
}
#[cfg(test)]
mod tests {
    use super::gradient_clip_by_value as clip;
    #[test] fn clip_values() {
        assert_eq!(clip(&[-3.,-0.5,0.,0.5,3.],1.),Ok(vec![-1.,-0.5,0.,0.5,1.]));
        assert_eq!(clip(&[-1.,0.,1.],1.),Ok(vec![-1.,0.,1.]));
        assert_eq!(clip(&[-1e308,1e308],1.),Ok(vec![-1.,1.]));
        assert_eq!(clip(&[0.],2.),Ok(vec![0.]));
    }
    #[test] fn invalid() {
        assert!(clip(&[],1.).is_err());
        assert!(clip(&[f64::NAN],1.).is_err());
        assert!(clip(&[1.],0.).is_err());
        assert!(clip(&[1.],f64::INFINITY).is_err());
    }
}
