//! Stable Swish (SiLU) activation and analytical derivative.

/// Return activations and derivatives for a nonempty finite input vector.
pub fn swish(inputs: &[f64]) -> Result<(Vec<f64>,Vec<f64>),&'static str> {
    if inputs.is_empty() || !inputs.iter().all(|x|x.is_finite()) {
        return Err("nonempty finite input required");
    }
    let mut values=Vec::with_capacity(inputs.len());
    let mut derivatives=Vec::with_capacity(inputs.len());
    for &x in inputs {
        let s=if x>=0.0 {
            1.0/(1.0+(-x).exp())
        } else {
            let z=x.exp();
            z/(1.0+z)
        };
        let value=x*s;
        let derivative=s+(x*s)*(1.0-s);
        if !value.is_finite() || !derivative.is_finite() {
            return Err("nonfinite Swish output");
        }
        values.push(value);
        derivatives.push(derivative);
    }
    Ok((values,derivatives))
}

#[cfg(test)]
mod tests {
    use super::swish;
    #[test]
    fn fixtures() {
        let (v,d)=swish(&[0.0,1.0,-1.0,1000.0,-1000.0]).unwrap();
        let s=1.0/(1.0+(-1.0_f64).exp());
        assert_eq!((v[0],d[0]),(0.0,0.5));
        assert!((v[1]-s).abs()<1e-12);
        assert!((d[1]-(s+s*(1.0-s))).abs()<1e-12);
        assert!((v[2]-(s-1.0)).abs()<1e-12);
        assert!((d[2]-(1.0-s)*(1.0-s+s)).abs()<1e-12);
        assert_eq!((v[3],d[3]),(1000.0,1.0));
        assert_eq!((v[4],d[4]),(0.0,0.0));
    }
    #[test]
    fn invalid() {
        assert!(swish(&[]).is_err());
        assert!(swish(&[f64::NAN]).is_err());
        assert!(swish(&[f64::INFINITY]).is_err());
    }
}
