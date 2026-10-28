//! Root mean squared error over aligned bounded finite predictions.

/// Return sqrt(mean squared residual), preserving original target units.
pub fn root_mean_squared_error(target: &[f64], prediction: &[f64])
    -> Result<f64,&'static str> {
    let n=target.len();
    if !(1..=10000).contains(&n) || prediction.len()!=n {
        return Err("paired vectors with length 1..10000 required");
    }
    let mut sum=0.0_f64;
    for (&t,&p) in target.iter().zip(prediction.iter()) {
        if !t.is_finite() || !p.is_finite() ||
           t.abs()>1_000_000.0 || p.abs()>1_000_000.0 {
            return Err("finite bounded predictions required");
        }
        let delta=p-t;
        sum+=delta*delta;
    }
    let value=(sum/n as f64).sqrt();
    if !value.is_finite() {return Err("nonfinite RMSE");}
    Ok(value)
}
#[cfg(test)]
mod tests {
    use super::root_mean_squared_error as rmse;
    fn close(a:f64,b:f64) {assert!((a-b).abs()<1e-12,"{a} != {b}");}
    #[test] fn examples() {
        close(rmse(&[0.,1.,2.],&[0.,2.,4.]).unwrap(),(5.0_f64/3.0).sqrt());
        assert_eq!(rmse(&[0.,1.,2.],&[0.,1.,2.]),Ok(0.));
        assert_eq!(rmse(&[-1e6],&[1e6]),Ok(2e6));
        assert_eq!(rmse(&[0.],&[1.]),Ok(1.));
        assert_eq!(rmse(&[0.],&[0.]),Ok(0.));
    }
    #[test] fn invalid() {
        assert!(rmse(&[],&[]).is_err());
        assert!(rmse(&[0.,1.],&[0.]).is_err());
        assert!(rmse(&[0.],&[f64::NAN]).is_err());
        assert!(rmse(&[f64::INFINITY],&[0.]).is_err());
        assert!(rmse(&[0.],&[1_000_001.]).is_err());
        assert!(rmse(&[-1_000_001.],&[0.]).is_err());
        assert!(rmse(&vec![0.;10001],&vec![0.;10001]).is_err());
    }
}
