//! Fixed-initialization two-layer ReLU/sigmoid binary network training.

fn sigmoid(logit: f64) -> f64 {
    if logit >= 0.0 {
        let e=(-logit).exp();
        1.0/(1.0+e)
    } else {
        let e=logit.exp();
        e/(1.0+e)
    }
}

/// Predict class-one probability given [w1,b1,w2,b2].
pub fn two_layer_predict(params: &[f64;4], x: f64) -> Result<f64,&'static str> {
    if !x.is_finite() || !params.iter().all(|v|v.is_finite()) {
        return Err("non-finite parameters or feature");
    }
    let pre=params[0]*x+params[1];
    if !pre.is_finite(){return Err("hidden preactivation overflow");}
    let logit=params[2]*pre.max(0.0)+params[3];
    if !logit.is_finite(){return Err("output logit overflow");}
    Ok(sigmoid(logit))
}

/// Train one hidden ReLU neuron and a sigmoid output on binary scalar labels.
pub fn two_layer_binary_classifier(
    features: &[f64], labels: &[f64], learning_rate: f64, epochs: usize,
) -> Result<[f64;4],&'static str> {
    if features.is_empty() || features.len()!=labels.len() ||
       !learning_rate.is_finite() || learning_rate<=0.0 || epochs==0 {
        return Err("invalid dimensions or training parameters");
    }
    if !features.iter().all(|x|x.is_finite()) ||
       !labels.iter().all(|y|y.is_finite() && (*y==0.0 || *y==1.0)) {
        return Err("finite features and binary labels required");
    }
    let mut params=[1.0_f64,0.5,1.0,0.0];
    for _ in 0..epochs {
        let mut gradient=[0.0_f64;4];
        for (&x,&label) in features.iter().zip(labels) {
            let pre=params[0]*x+params[1];
            if !pre.is_finite(){return Err("hidden preactivation overflow");}
            let hidden=pre.max(0.0);
            let logit=params[2]*hidden+params[3];
            if !logit.is_finite(){return Err("output logit overflow");}
            let error=sigmoid(logit)-label;
            let delta=if pre>0.0 {error*params[2]} else {0.0};
            let terms=[delta*x,delta,error*hidden,error];
            for j in 0..4 {
                if !terms[j].is_finite(){return Err("gradient product overflow");}
                gradient[j]+=terms[j];
                if !gradient[j].is_finite(){return Err("gradient accumulation overflow");}
            }
        }
        let factor=learning_rate/features.len() as f64;
        for j in 0..4 {
            let update=factor*gradient[j];
            if !update.is_finite(){return Err("parameter update overflow");}
            params[j]-=update;
            if !params[j].is_finite(){return Err("training diverged");}
        }
    }
    Ok(params)
}

#[cfg(test)]
mod tests {
    use super::{two_layer_binary_classifier as train,two_layer_predict as predict};
    #[test]
    fn single_gradient_step() {
        let params=train(&[0.0],&[1.0],0.2,1).unwrap();
        let error=1.0/(1.0+(-0.5_f64).exp())-1.0;
        assert!((params[0]-1.0).abs()<1e-12);
        assert!((params[1]-(0.5-0.2*error)).abs()<1e-12);
        assert!((params[2]-(1.0-0.2*error*0.5)).abs()<1e-12);
        assert!((params[3]-(-0.2*error)).abs()<1e-12);
    }
    #[test]
    fn training_and_inactive_hidden() {
        let dead=train(&[-1.0],&[1.0],0.2,1).unwrap();
        assert_eq!(&dead[..3], &[1.0,0.5,1.0]);
        let params=train(&[0.0,1.0,2.0,3.0],&[0.0,0.0,1.0,1.0],0.1,500).unwrap();
        assert!(predict(&params,0.0).unwrap()<0.1);
        assert!(predict(&params,3.0).unwrap()>0.9);
    }
    #[test]
    fn invalid() {
        assert!(train(&[],&[],0.1,1).is_err());
        assert!(train(&[0.0],&[],0.1,1).is_err());
        assert!(train(&[0.0],&[2.0],0.1,1).is_err());
        assert!(train(&[f64::NAN],&[1.0],0.1,1).is_err());
        assert!(train(&[0.0],&[1.0],0.0,1).is_err());
        assert!(train(&[0.0],&[1.0],0.1,0).is_err());
        assert!(train(&[1e308],&[0.0],1e308,2).is_err());
        assert!(predict(&[1.0,0.5,1.0,0.0],f64::INFINITY).is_err());
    }
}
