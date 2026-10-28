//! Mean squared logarithmic regression error on nonnegative pairs.

/// MSLE using ln_1p, with all inputs finite and bounded by 1000000.
pub fn mean_squared_log_error(target: &[f64], prediction: &[f64])
    -> Result<f64, &'static str> {
    let n=target.len();
    if !(1..=10000).contains(&n) || prediction.len()!=n {
        return Err("equal lengths in 1..10000 required");
    }
    let mut sum=0.0_f64;
    for (&t,&p) in target.iter().zip(prediction.iter()) {
        if !t.is_finite() || !p.is_finite() || t<0.0 || p<0.0 ||
           t>1_000_000.0 || p>1_000_000.0 {
            return Err("nonnegative finite values bounded by 1000000 required");
        }
        let delta=p.ln_1p()-t.ln_1p();
        sum+=delta*delta;
    }
    let result=sum/n as f64;
    if !result.is_finite() {return Err("nonfinite result");}
    Ok(result)
}
#[cfg(test)]
mod tests {
    use super::mean_squared_log_error as msle;
    fn close(a:f64,b:f64) {assert!((a-b).abs()<1e-11,"{a} != {b}");}
    #[test] fn examples() {
        close(msle(&[0.,1.,9.],&[0.,3.,9.]).unwrap(),2.0_f64.ln().powi(2)/3.);
        assert_eq!(msle(&[0.,1.,9.],&[0.,1.,9.]),Ok(0.));
        close(msle(&[0.],&[1.]).unwrap(),2.0_f64.ln().powi(2));
        close(msle(&[0.],&[1e6]).unwrap(),1e6_f64.ln_1p().powi(2));
        assert_eq!(msle(&[0.],&[0.]),Ok(0.));
    }
    #[test] fn invalid() {
        assert!(msle(&[],&[]).is_err());
        assert!(msle(&[0.,1.],&[0.]).is_err());
        assert!(msle(&[-1.],&[0.]).is_err());
        assert!(msle(&[0.],&[-1.]).is_err());
        assert!(msle(&[1_000_001.],&[0.]).is_err());
        assert!(msle(&[f64::NAN],&[0.]).is_err());
        assert!(msle(&[f64::INFINITY],&[0.]).is_err());
        assert!(msle(&vec![0.;10001],&vec![0.;10001]).is_err());
    }
}
