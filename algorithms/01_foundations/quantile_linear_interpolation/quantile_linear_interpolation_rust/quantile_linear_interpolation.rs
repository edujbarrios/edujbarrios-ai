//! Type-7 linearly interpolated quantile of finite observations.

/// Return the type-7 sample quantile for a probability in [0,1].
pub fn quantile_linear_interpolation(
    values: &[f64], probability: f64,
) -> Result<f64, &'static str> {
    if !(1..=512).contains(&values.len()) || values.iter().any(|v| !v.is_finite()) {
        return Err("expected 1..512 finite observations");
    }
    if !probability.is_finite() || !(0.0..=1.0).contains(&probability) {
        return Err("probability must be finite in [0,1]");
    }
    let mut sorted=values.to_vec();
    sorted.sort_by(f64::total_cmp);
    let h=(sorted.len()-1) as f64*probability;
    let i=h.floor() as usize;
    let f=h-i as f64;
    let result=if f==0.0 {sorted[i]} else {(1.0-f)*sorted[i]+f*sorted[i+1]};
    if !result.is_finite(){return Err("nonfinite quantile");}
    Ok(result)
}
#[cfg(test)]
mod tests {
    use super::quantile_linear_interpolation as q;
    fn near(values:&[f64],p:f64,want:f64) {
        let got=q(values,p).unwrap();
        assert!((got-want).abs()<=1e-11f64*want.abs().max(1.0));
    }
    #[test]
    fn examples() {
        near(&[1.,2.,3.,4.],0.25,1.75);
        near(&[1.,2.,3.,4.],0.5,2.5);
        near(&[1.,2.,3.,4.],0.75,3.25);
        near(&[5.,10.],0.2,6.);
        near(&[42.],0.9,42.);
        near(&[100.,2.,-5.],0.,-5.);
        near(&[100.,2.,-5.],1.,100.);
        near(&[-1e308,1e308],0.5,0.);
        near(&[-1e308,1e308],0.,-1e308);
        near(&[-1e308,1e308],1.,1e308);
        near(&[7.;512],0.37,7.);
    }
    #[test]
    fn invalid() {
        assert!(q(&[],0.5).is_err());
        assert!(q(&[1.;513],0.5).is_err());
        assert!(q(&[f64::NAN],0.5).is_err());
        assert!(q(&[f64::INFINITY],0.5).is_err());
        assert!(q(&[1.],-0.01).is_err());
        assert!(q(&[1.],1.01).is_err());
        assert!(q(&[1.],f64::NAN).is_err());
        assert!(q(&[1.],f64::INFINITY).is_err());
    }
}
