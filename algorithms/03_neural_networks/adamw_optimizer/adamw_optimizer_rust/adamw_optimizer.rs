//! One AdamW update with decoupled weight decay and explicit moment state.

/// Compute updated parameters, first and second moments, using fixed betas.
pub fn adamw_optimizer(theta:&[f64],gradient:&[f64],m:&[f64],v:&[f64],
                       step:usize,learning_rate:f64)
    ->Result<(Vec<f64>,Vec<f64>,Vec<f64>),&'static str> {
    let n=theta.len();
    if n==0 || gradient.len()!=n || m.len()!=n || v.len()!=n ||
       !(1..=1_000_000).contains(&step) ||
       !learning_rate.is_finite() || learning_rate<=0.0 ||
       theta.iter().chain(gradient).chain(m).chain(v).any(|x|!x.is_finite()) ||
       v.iter().any(|x|*x<0.0) {
        return Err("invalid vector dimensions, state, step or learning rate");
    }
    let c1=1.0-0.9_f64.powf(step as f64);
    let c2=1.0-0.999_f64.powf(step as f64);
    if c1<=0.0 || c2<=0.0 {return Err("bias correction underflow");}
    let mut params=Vec::with_capacity(n);
    let mut next_m=Vec::with_capacity(n);
    let mut next_v=Vec::with_capacity(n);
    for i in 0..n {
        let square=gradient[i]*gradient[i];
        let new_m=0.9*m[i]+0.1*gradient[i];
        let new_v=0.999*v[i]+0.001*square;
        if !square.is_finite() || !new_m.is_finite() || !new_v.is_finite() ||
           new_v<0.0 {return Err("moment update overflow");}
        let mh=new_m/c1;
        let vh=new_v/c2;
        let denom=vh.sqrt()+1e-8;
        let adaptive=mh/denom;
        let decay=0.01*theta[i];
        let change=learning_rate*(adaptive+decay);
        let updated=theta[i]-change;
        if [mh,vh,denom,adaptive,decay,change,updated].iter().any(|x|!x.is_finite()){
            return Err("parameter update overflow");
        }
        params.push(updated);
        next_m.push(new_m);
        next_v.push(new_v);
    }
    Ok((params,next_m,next_v))
}

#[cfg(test)]
mod tests {
    use super::adamw_optimizer as step;
    #[test]
    fn stateful_updates() {
        let (p,m,v)=step(&[2.0,-3.0],&[1.0,-2.0],&[0.0;2],&[0.0;2],1,0.1).unwrap();
        assert!((m[0]-0.1).abs()<1e-12 && (m[1]+0.2).abs()<1e-12);
        assert!((v[0]-0.001).abs()<1e-12 && (v[1]-0.004).abs()<1e-12);
        assert!((p[0]-(2.0-0.1*(1.0/(1.0+1e-8)+0.02))).abs()<1e-12);
        assert!((p[1]-(-3.0-0.1*(-2.0/(2.0+1e-8)-0.03))).abs()<1e-12);
        let (p2,m2,v2)=step(&p,&[0.0;2],&m,&v,2,0.1).unwrap();
        assert!((m2[0]-0.09).abs()<1e-12 && (v2[0]-0.000999).abs()<1e-12);
        assert!(p2[0]<p[0] && p2[1]>p[1]);
        assert!((step(&[1.0],&[0.0],&[0.0],&[0.0],1,0.1).unwrap().0[0]-0.999).abs()<1e-12);
    }
    #[test]
    fn errors() {
        assert!(step(&[],&[],&[],&[],1,0.1).is_err());
        assert!(step(&[1.0],&[],&[0.0],&[0.0],1,0.1).is_err());
        assert!(step(&[1.0],&[1.0],&[0.0],&[-1.0],1,0.1).is_err());
        assert!(step(&[1.0],&[1.0],&[0.0],&[0.0],0,0.1).is_err());
        assert!(step(&[1.0],&[f64::NAN],&[0.0],&[0.0],1,0.1).is_err());
        assert!(step(&[1.0],&[1e308],&[0.0],&[0.0],1,0.1).is_err());
    }
}
