//! One SGD-with-momentum step; velocity accumulates gradients.

/// Return (updated parameters, updated velocity) with momentum=0.9.
pub fn momentum_sgd(theta:&[f64],gradient:&[f64],velocity:&[f64],
                    learning_rate:f64)->Result<(Vec<f64>,Vec<f64>),&'static str> {
    let n=theta.len();
    if n==0 || gradient.len()!=n || velocity.len()!=n ||
       !learning_rate.is_finite() || learning_rate<=0.0 ||
       !theta.iter().chain(gradient.iter()).chain(velocity.iter()).all(|v|v.is_finite()){
        return Err("invalid vector shapes, state or learning rate");
    }
    let mut next=Vec::with_capacity(n);
    let mut state=Vec::with_capacity(n);
    for i in 0..n {
        let retained=0.9*velocity[i];
        let v=retained+gradient[i];
        let delta=learning_rate*v;
        let p=theta[i]-delta;
        if !retained.is_finite() || !v.is_finite() ||
           !delta.is_finite() || !p.is_finite(){
            return Err("momentum update overflow");
        }
        next.push(p);
        state.push(v);
    }
    Ok((next,state))
}

#[cfg(test)]
mod tests {
    use super::momentum_sgd as step;
    #[test]
    fn successive_updates() {
        let (theta,v)=step(&[2.0,-3.0],&[1.0,-2.0],&[0.0,0.0],0.1).unwrap();
        assert!((theta[0]-1.9).abs()<1e-12);
        assert!((theta[1]+2.8).abs()<1e-12);
        assert_eq!(v,vec![1.0,-2.0]);
        let (theta,v)=step(&theta,&[0.0,0.0],&v,0.1).unwrap();
        assert!((theta[0]-1.81).abs()<1e-12);
        assert!((theta[1]+2.62).abs()<1e-12);
        assert!((v[0]-0.9).abs()<1e-12);
        assert!((v[1]+1.8).abs()<1e-12);
        assert_eq!(step(&[1.0],&[0.0],&[0.0],0.1),Ok((vec![1.0],vec![0.0])));
    }
    #[test]
    fn invalid() {
        assert!(step(&[],&[],&[],0.1).is_err());
        assert!(step(&[1.0],&[0.0],&[],0.1).is_err());
        assert!(step(&[1.0],&[0.0],&[0.0],0.0).is_err());
        assert!(step(&[1.0],&[f64::NAN],&[0.0],0.1).is_err());
        assert!(step(&[1e308],&[1e308],&[0.0],1e308).is_err());
    }
}
