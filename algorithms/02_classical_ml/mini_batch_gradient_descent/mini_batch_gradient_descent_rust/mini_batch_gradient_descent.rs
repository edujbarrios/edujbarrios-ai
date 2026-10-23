//! Sequential mini-batch descent for half-MSE linear regression.

pub fn mini_batch_gradient_descent(
    x: &[f64], y: &[f64], learning_rate: f64,
    epochs: usize, batch_size: usize,
) -> Result<(f64,f64), &'static str> {
    if x.is_empty() || x.len()!=y.len() || epochs==0 ||
       batch_size==0 || batch_size>x.len() ||
       !learning_rate.is_finite() || learning_rate<=0.0 {
        return Err("invalid batch sizes or learning parameters");
    }
    if !x.iter().chain(y.iter()).all(|v| v.is_finite()) {
        return Err("inputs must be finite");
    }
    let (mut weight,mut bias)=(0.0_f64,0.0_f64);
    for _ in 0..epochs {
        let mut start=0;
        while start<x.len() {
            let m=(x.len()-start).min(batch_size);
            let (mut gw,mut gb)=(0.0_f64,0.0_f64);
            for i in start..start+m {
                let prediction=weight*x[i]+bias;
                let error=prediction-y[i];
                let component=error*x[i];
                if !prediction.is_finite() || !error.is_finite() ||
                   !component.is_finite() {return Err("gradient overflow");}
                gw+=component;
                gb+=error;
                if !gw.is_finite() || !gb.is_finite() {return Err("sum overflow");}
            }
            let factor=learning_rate/m as f64;
            let dw=factor*gw;
            let db=factor*gb;
            if !factor.is_finite() || !dw.is_finite() || !db.is_finite() {
                return Err("step overflow");
            }
            weight-=dw;
            bias-=db;
            if !weight.is_finite() || !bias.is_finite(){return Err("training diverged");}
            start+=m;
        }
    }
    Ok((weight,bias))
}

#[cfg(test)]
mod tests {
    use super::mini_batch_gradient_descent as train;
    #[test]
    fn one_epoch_and_convergence() {
        let (w,b)=train(&[0.0,1.0],&[1.0,3.0],0.1,1,1).unwrap();
        assert!((w-0.29).abs()<1e-12);
        assert!((b-0.39).abs()<1e-12);
        let (w,b)=train(&[0.0,1.0,2.0,3.0],&[1.0,3.0,5.0,7.0],0.05,2000,3).unwrap();
        assert!((w-2.0).abs()<0.01);
        assert!((b-1.0).abs()<0.01);
    }
    #[test]
    fn invalid() {
        assert!(train(&[],&[],0.1,1,1).is_err());
        assert!(train(&[1.0],&[],0.1,1,1).is_err());
        assert!(train(&[1.0],&[1.0],0.0,1,1).is_err());
        assert!(train(&[1.0],&[1.0],0.1,0,1).is_err());
        assert!(train(&[1.0],&[1.0],0.1,1,2).is_err());
        assert!(train(&[f64::NAN],&[1.0],0.1,1,1).is_err());
        assert!(train(&[1e308,1e308],&[1.0,3.0],1e308,2,2).is_err());
    }
}
