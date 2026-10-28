//! One-feature weighted least squares with a free intercept.

/// Return (slope, intercept) for finite x,y,nonnegative weights.
pub fn weighted_linear_regression_1d(x: &[f64], y: &[f64], weights: &[f64])
    -> Result<(f64,f64), &'static str> {
    let n=x.len();
    if !(2..=128).contains(&n) || y.len()!=n || weights.len()!=n {
        return Err("three equal-length sequences of length 2..128 required");
    }
    if x.iter().chain(y.iter()).any(|z| !z.is_finite() || z.abs()>1_000_000.0)
       || weights.iter().any(|z| !z.is_finite() || !(0.0..=1_000_000.0).contains(z)) {
        return Err("finite bounded values and nonnegative weights required");
    }
    let total: f64=weights.iter().sum();
    if total<=0.0 {return Err("positive total weight required");}
    let mx: f64=x.iter().zip(weights).map(|(&xi,&wi)|wi*xi).sum::<f64>()/total;
    let my: f64=y.iter().zip(weights).map(|(&yi,&wi)|wi*yi).sum::<f64>()/total;
    let mut numerator=0.0_f64;
    let mut denominator=0.0_f64;
    for i in 0..n {
        let dx=x[i]-mx;
        numerator+=weights[i]*dx*(y[i]-my);
        denominator+=weights[i]*dx*dx;
    }
    if denominator<=0.0 || !denominator.is_finite() {
        return Err("positive weighted feature variance required");
    }
    let slope=numerator/denominator;
    let intercept=my-slope*mx;
    if !slope.is_finite() || !intercept.is_finite() {
        return Err("nonfinite fitted coefficients");
    }
    Ok((slope,intercept))
}

#[cfg(test)]
mod tests {
    use super::weighted_linear_regression_1d as fit;
    fn close(a:f64,b:f64) {assert!((a-b).abs()<1e-12,"{a} != {b}");}
    #[test] fn fits() {
        let (s,a)=fit(&[0.,1.,2.],&[1.,2.,2.],&[1.,1.,2.]).unwrap();
        close(s,5./11.);close(a,13./11.);
        let (s,a)=fit(&[0.,1.,2.],&[1.,3.,5.],&[1.,1.,1.]).unwrap();
        close(s,2.);close(a,1.);
        let (s,a)=fit(&[0.,1.],&[1.,3.],&[1.,1.]).unwrap();
        close(s,2.);close(a,1.);
    }
    #[test] fn invalid() {
        assert!(fit(&[],&[],&[]).is_err());
        assert!(fit(&[0.],&[1.],&[1.]).is_err());
        assert!(fit(&[0.,1.],&[1.],&[1.,1.]).is_err());
        assert!(fit(&[0.,1.],&[1.,2.],&[0.,0.]).is_err());
        assert!(fit(&[0.,1.],&[1.,2.],&[1.,0.]).is_err());
        assert!(fit(&[2.,2.],&[1.,2.],&[1.,1.]).is_err());
        assert!(fit(&[0.,1.],&[1.,2.],&[-1.,1.]).is_err());
        assert!(fit(&[0.,1.],&[1.,2.],&[1.,1_000_001.]).is_err());
        assert!(fit(&[0.,1.],&[1.,2.],&[1.,f64::NAN]).is_err());
        assert!(fit(&[0.,f64::INFINITY],&[1.,2.],&[1.,1.]).is_err());
        assert!(fit(&vec![0.;129],&vec![1.;129],&vec![1.;129]).is_err());
    }
}
