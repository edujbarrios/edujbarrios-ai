//! Batch-mean Huber regression loss and gradients wrt predictions.

/// Return mean Huber loss and prediction gradients for positive delta.
pub fn huber_loss(predictions:&[f64],targets:&[f64],delta:f64)
    ->Result<(f64,Vec<f64>),&'static str>{
    let n=predictions.len();
    if n==0 || targets.len()!=n || !delta.is_finite() || delta<=0.0 ||
       predictions.iter().chain(targets.iter()).any(|v|!v.is_finite()){
        return Err("nonempty finite predictions, targets and positive delta required");
    }
    let mut sum=0.0_f64;
    let mut gradients=Vec::with_capacity(n);
    for (&p,&y) in predictions.iter().zip(targets.iter()) {
        let error=p-y;
        if !error.is_finite(){return Err("residual overflow");}
        let magnitude=error.abs();
        let (sample,gradient)=if magnitude<=delta {
            (0.5*error*error,error/n as f64)
        } else {
            (delta*(magnitude-0.5*delta),
             if error>0.0 {delta/n as f64} else {-delta/n as f64})
        };
        if !sample.is_finite() || !gradient.is_finite(){
            return Err("Huber arithmetic overflow");
        }
        sum+=sample;
        if !sum.is_finite(){return Err("Huber sum overflow");}
        gradients.push(gradient);
    }
    let result=sum/n as f64;
    if !result.is_finite() || result<0.0 {
        return Err("invalid Huber loss");
    }
    Ok((result,gradients))
}

#[cfg(test)]
mod tests {
    use super::huber_loss;
    #[test]
    fn values_and_boundaries() {
        let (loss,grad)=huber_loss(&[2.0,0.0,-2.0],&[0.0,1.0,0.0],1.0).unwrap();
        assert!((loss-7.0/6.0).abs()<1e-12);
        assert!((grad[0]-1.0/3.0).abs()<1e-12);
        assert!((grad[1]+1.0/3.0).abs()<1e-12);
        assert!((grad[2]+1.0/3.0).abs()<1e-12);
        assert_eq!(huber_loss(&[1.0,-1.0],&[0.0,0.0],1.0),
                   Ok((0.5,vec![0.5,-0.5])));
        assert_eq!(huber_loss(&[0.0,0.0],&[0.0,0.0],1.0),
                   Ok((0.0,vec![0.0,0.0])));
        assert_eq!(huber_loss(&[0.5],&[0.0],1.0),Ok((0.125,vec![0.5])));
    }
    #[test]
    fn invalid() {
        assert!(huber_loss(&[],&[],1.0).is_err());
        assert!(huber_loss(&[1.0],&[],1.0).is_err());
        assert!(huber_loss(&[1.0],&[0.0],0.0).is_err());
        assert!(huber_loss(&[f64::NAN],&[0.0],1.0).is_err());
        assert!(huber_loss(&[1.7e308],&[-1.7e308],1.0).is_err());
        assert!(huber_loss(&[1e308],&[0.0],1e308).is_err());
    }
}
