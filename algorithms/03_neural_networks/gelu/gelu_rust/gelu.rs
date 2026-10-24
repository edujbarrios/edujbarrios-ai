//! Tanh-approximated GELU and derivative, without ML dependencies.
const ALPHA:f64=0.7978845608028654;
const BETA:f64=0.044715;

/// Return (values, derivatives) for a nonempty finite vector.
pub fn gelu(inputs:&[f64]) -> Result<(Vec<f64>,Vec<f64>),&'static str> {
    if inputs.is_empty() || !inputs.iter().all(|x|x.is_finite()) {
        return Err("nonempty finite input required");
    }
    let mut values=Vec::with_capacity(inputs.len());
    let mut derivatives=Vec::with_capacity(inputs.len());
    for &x in inputs {
        let square=x*x;
        let cubic=square*x;
        let u=ALPHA*(x+BETA*cubic);
        if !square.is_finite() || !cubic.is_finite() || !u.is_finite(){
            return Err("GELU polynomial overflow");
        }
        let t=u.tanh();
        let gate=0.5*(1.0+t);
        let factor=ALPHA*(1.0+3.0*BETA*square);
        let slope=0.5*x*(1.0-t*t)*factor;
        let value=x*gate;
        let derivative=gate+slope;
        if !factor.is_finite() || !slope.is_finite() ||
           !value.is_finite() || !derivative.is_finite(){
            return Err("GELU output overflow");
        }
        values.push(value);
        derivatives.push(derivative);
    }
    Ok((values,derivatives))
}

#[cfg(test)]
mod tests {
    use super::gelu;
    #[test]
    fn fixtures() {
        let (v,d)=gelu(&[0.0,1.0,-1.0,100.0,-100.0]).unwrap();
        let t=(0.7978845608028654_f64*(1.0+0.044715)).tanh();
        let gate=0.5*(1.0+t);
        let deriv=gate+0.5*(1.0-t*t)*0.7978845608028654*(1.0+3.0*0.044715);
        assert_eq!((v[0],d[0]),(0.0,0.5));
        assert!((v[1]-gate).abs()<1e-12);
        assert!((d[1]-deriv).abs()<1e-12);
        assert!((v[2]-(gate-1.0)).abs()<1e-12);
        assert!((d[2]-(1.0-deriv)).abs()<1e-12);
        assert_eq!((v[3],d[3]),(100.0,1.0));
        assert_eq!((v[4],d[4]),(0.0,0.0));
    }
    #[test]
    fn invalid() {
        assert!(gelu(&[]).is_err());
        assert!(gelu(&[f64::NAN]).is_err());
        assert!(gelu(&[f64::INFINITY]).is_err());
        assert!(gelu(&[1e308]).is_err());
    }
}
