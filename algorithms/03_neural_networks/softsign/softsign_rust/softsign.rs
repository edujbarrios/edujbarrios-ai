//! Softsign activation for a bounded vector of finite real numbers.

/// Apply x / (1 + |x|) elementwise.
pub fn softsign(values: &[f64]) -> Result<Vec<f64>, &'static str> {
    if !(1..=10000).contains(&values.len()) ||
       values.iter().any(|x| !x.is_finite() || x.abs()>1_000_000.0) {
        return Err("bounded finite input length 1..10000 required");
    }
    Ok(values.iter().map(|&x| x/(1.0+x.abs())).collect())
}

#[cfg(test)]
mod tests {
    use super::softsign;
    fn close(x:f64,y:f64) { assert!((x-y).abs()<1e-12,"{x} != {y}"); }
    #[test]
    fn reference() {
        let output=softsign(&[-2.,-1.,0.,1.,2.]).unwrap();
        for (a,b) in output.iter().zip([-2./3.,-0.5,0.,0.5,2./3.].iter()) {
            close(*a,*b);
        }
        let max=softsign(&[1e6,-1e6]).unwrap();
        close(max[0],1e6/1_000_001.);
        close(max[1],-1e6/1_000_001.);
        assert_eq!(softsign(&[0.]),Ok(vec![0.]));
        assert!(max.iter().all(|x| *x>-1.0 && *x<1.0));
    }
    #[test]
    fn invalid() {
        assert!(softsign(&[]).is_err());
        assert!(softsign(&[f64::NAN]).is_err());
        assert!(softsign(&[f64::INFINITY]).is_err());
        assert!(softsign(&[1_000_001.]).is_err());
        assert!(softsign(&[-1_000_001.]).is_err());
        assert!(softsign(&vec![0.;10001]).is_err());
    }
}
