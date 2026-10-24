//! RMSProp one-step optimization, with caller-managed squared-gradient state.
const DECAY:f64=0.9;
const EPSILON:f64=1e-8;

/// Return new (parameters, squared gradient average) using fixed RMSProp constants.
pub fn rmsprop_optimizer(
    parameters:&[f64],gradients:&[f64],previous_average:&[f64],learning_rate:f64
) -> Result<(Vec<f64>,Vec<f64>),&'static str> {
    let n=parameters.len();
    if n==0 || gradients.len()!=n || previous_average.len()!=n ||
       !learning_rate.is_finite() || learning_rate<=0.0 ||
       !parameters.iter().chain(gradients.iter()).chain(previous_average.iter())
           .all(|v|v.is_finite()) ||
       previous_average.iter().any(|&v|v<0.0) {
        return Err("invalid learning rate, state or vector shapes");
    }
    let mut next=Vec::with_capacity(n);
    let mut averages=Vec::with_capacity(n);
    for i in 0..n {
        let square=gradients[i]*gradients[i];
        if !square.is_finite(){return Err("gradient square overflow");}
        let average=DECAY*previous_average[i]+(1.0-DECAY)*square;
        if !average.is_finite() || average<0.0 {return Err("average overflow");}
        let denominator=average.sqrt()+EPSILON;
        let step=learning_rate*gradients[i]/denominator;
        let updated=parameters[i]-step;
        if !denominator.is_finite() || !step.is_finite() || !updated.is_finite(){
            return Err("RMSProp update overflow");
        }
        averages.push(average);
        next.push(updated);
    }
    Ok((next,averages))
}

#[cfg(test)]
mod tests {
    use super::rmsprop_optimizer as step;
    #[test]
    fn first_and_second_step() {
        let (theta,v)=step(&[2.0,-3.0],&[1.0,-2.0],&[0.0,0.0],0.1).unwrap();
        assert!((v[0]-0.1).abs()<1e-12);
        assert!((v[1]-0.4).abs()<1e-12);
        assert!((theta[0]-(2.0-0.1/(0.1_f64.sqrt()+1e-8))).abs()<1e-12);
        assert!((theta[1]-(-3.0+0.2/(0.4_f64.sqrt()+1e-8))).abs()<1e-12);
        let (second,next_v)=step(&theta,&[0.0,0.0],&v,0.1).unwrap();
        assert_eq!(second,theta);
        assert!((next_v[0]-0.09).abs()<1e-12);
        assert!((next_v[1]-0.36).abs()<1e-12);
        assert_eq!(step(&[1.0],&[0.0],&[0.0],0.1),Ok((vec![1.0],vec![0.0])));
    }
    #[test]
    fn invalid() {
        assert!(step(&[],&[],&[],0.1).is_err());
        assert!(step(&[1.0],&[1.0],&[],0.1).is_err());
        assert!(step(&[1.0],&[1.0],&[-1.0],0.1).is_err());
        assert!(step(&[1.0],&[1.0],&[0.0],0.0).is_err());
        assert!(step(&[1.0],&[f64::NAN],&[0.0],0.1).is_err());
        assert!(step(&[1e308],&[1e308],&[0.0],0.1).is_err());
    }
}
