//! Exponential linear unit activation with fixed alpha=1.

/// Return x for x>=0, exp_m1(x) for x<0; validate before computing.
pub fn elu(values: &[f64]) -> Result<Vec<f64>, &'static str> {
    if !(1..=10000).contains(&values.len()) ||
       values.iter().any(|x| !x.is_finite() || x.abs()>1_000_000.0) {
        return Err("finite bounded input of length 1..10000 required");
    }
    Ok(values.iter().map(|&x| if x>=0.0 {x} else {x.exp_m1()}).collect())
}
#[cfg(test)]
mod tests {
    use super::elu;
    fn close(a:f64,b:f64) { assert!((a-b).abs()<1e-12,"{a} != {b}"); }
    #[test] fn examples() {
        let output=elu(&[-1.,0.,1.]).unwrap();
        close(output[0],-0.6321205588285577);
        assert_eq!(output[1],0.);
        assert_eq!(output[2],1.);
        assert_eq!(elu(&[-1e6,1e6]),Ok(vec![-1.,1e6]));
        close(elu(&[-1e-9]).unwrap()[0],-1e-9);
        assert_eq!(elu(&[1e-9]),Ok(vec![1e-9]));
    }
    #[test] fn invalid() {
        assert!(elu(&[]).is_err());
        assert!(elu(&[f64::NAN]).is_err());
        assert!(elu(&[f64::INFINITY]).is_err());
        assert!(elu(&[1_000_001.]).is_err());
        assert!(elu(&[-1_000_001.]).is_err());
        assert!(elu(&vec![0.;10001]).is_err());
    }
}
