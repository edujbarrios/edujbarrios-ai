//! Bounded mean symmetric absolute percentage error (ratio 0..2).

/// Return mean 2*|p-t|/(|t|+|p|); define zero/zero as zero.
pub fn symmetric_mean_absolute_percentage_error(
    target: &[f64], prediction: &[f64],
) -> Result<f64, &'static str> {
    let n=target.len();
    if !(1..=10000).contains(&n) || prediction.len()!=n {
        return Err("equal lengths in 1..10000 required");
    }
    let mut total=0.0;
    for (&t,&p) in target.iter().zip(prediction.iter()) {
        if !t.is_finite() || !p.is_finite() ||
           t.abs()>1_000_000.0 || p.abs()>1_000_000.0 {
            return Err("bounded finite paired predictions required");
        }
        let denom=t.abs()+p.abs();
        if denom>0.0 { total+=2.0*(p-t).abs()/denom; }
    }
    let result=total/n as f64;
    if !result.is_finite() { return Err("nonfinite result"); }
    Ok(result.clamp(0.0,2.0))
}

#[cfg(test)]
mod tests {
    use super::symmetric_mean_absolute_percentage_error as smape;
    fn close(a:f64,b:f64) { assert!((a-b).abs()<1e-12,"{a} != {b}"); }
    #[test]
    fn examples() {
        close(smape(&[0.,1.,-1.],&[0.,2.,1.]).unwrap(),8./9.);
        assert_eq!(smape(&[0.,1.,-1.],&[0.,1.,-1.]),Ok(0.));
        assert_eq!(smape(&[0.],&[0.]),Ok(0.));
        assert_eq!(smape(&[1.],&[0.]),Ok(2.));
        assert_eq!(smape(&[1.],&[-1.]),Ok(2.));
        assert_eq!(smape(&[1e6],&[-1e6]),Ok(2.));
        close(smape(&[1.],&[2.]).unwrap(),2./3.);
    }
    #[test]
    fn invalid() {
        assert!(smape(&[],&[]).is_err());
        assert!(smape(&[0.,1.],&[0.]).is_err());
        assert!(smape(&[0.],&[f64::NAN]).is_err());
        assert!(smape(&[f64::INFINITY],&[0.]).is_err());
        assert!(smape(&[0.],&[1_000_001.]).is_err());
        assert!(smape(&[-1_000_001.],&[0.]).is_err());
        assert!(smape(&vec![0.;10001],&vec![0.;10001]).is_err());
    }
}
