//! One stateful bias-corrected Adam parameter update.

/// Return (updated parameters, first moment, second moment).
/// 'step' is one-based; all vectors have the same positive length.
pub fn adam_optimizer(
    parameters: &[f64], gradients: &[f64], m: &[f64], v: &[f64],
    learning_rate: f64, step: usize
) -> Result<(Vec<f64>,Vec<f64>,Vec<f64>),&'static str> {
    let n=parameters.len();
    if n==0 || gradients.len()!=n || m.len()!=n || v.len()!=n ||
       step==0 || !learning_rate.is_finite() || learning_rate<=0.0 {
        return Err("invalid dimensions or learning parameters");
    }
    if !parameters.iter().chain(gradients).chain(m).chain(v).all(|x|x.is_finite())
       || v.iter().any(|&x|x<0.0) {
        return Err("all components finite, second moments nonnegative");
    }
    let (b1,b2,eps)=(0.9_f64,0.999_f64,1e-8_f64);
    let c1=-((step as f64)*b1.ln()).exp_m1();
    let c2=-((step as f64)*b2.ln()).exp_m1();
    if !c1.is_finite() || !c2.is_finite() || c1<=0.0 || c2<=0.0 {
        return Err("moment correction failure");
    }
    let mut next_p=Vec::with_capacity(n);
    let mut next_m=Vec::with_capacity(n);
    let mut next_v=Vec::with_capacity(n);
    for i in 0..n {
        let square=gradients[i]*gradients[i];
        let first=b1*m[i]+(1.0-b1)*gradients[i];
        let second=b2*v[i]+(1.0-b2)*square;
        if !square.is_finite() || !first.is_finite() ||
           !second.is_finite() || second<0.0 {
            return Err("moment overflow");
        }
        let mhat=first/c1;
        let vhat=second/c2;
        if !mhat.is_finite() || !vhat.is_finite() || vhat<0.0 {
            return Err("correction overflow");
        }
        let delta=learning_rate*(mhat/(vhat.sqrt()+eps));
        let theta=parameters[i]-delta;
        if !delta.is_finite() || !theta.is_finite() {
            return Err("update overflow");
        }
        next_p.push(theta);next_m.push(first);next_v.push(second);
    }
    Ok((next_p,next_m,next_v))
}

#[cfg(test)]
mod tests {
    use super::adam_optimizer as update;
    #[test]
    fn first_and_second_step() {
        let (p,m,v)=update(&[2.0,-3.0],&[1.0,-2.0],
                           &[0.0,0.0],&[0.0,0.0],0.1,1).unwrap();
        assert!((p[0]-(2.0-0.1/(1.0+1e-8))).abs()<1e-12);
        assert!((p[1]-(-3.0+0.2/(2.0+1e-8))).abs()<1e-12);
        assert!((m[0]-0.1).abs()<1e-12);
        assert!((m[1]+0.2).abs()<1e-12);
        assert!((v[0]-0.001).abs()<1e-12);
        assert!((v[1]-0.004).abs()<1e-12);
        let (_,next_m,next_v)=update(&p,&[0.0,0.0],&m,&v,0.1,2).unwrap();
        assert!(next_m[0]<m[0] && next_m[1]>m[1]);
        assert!(next_v[0]>0.0);
    }
    #[test]
    fn invalid() {
        assert!(update(&[],&[],&[],&[],0.1,1).is_err());
        assert!(update(&[1.0],&[],&[0.0],&[0.0],0.1,1).is_err());
        assert!(update(&[1.0],&[1.0],&[0.0],&[-1.0],0.1,1).is_err());
        assert!(update(&[1.0],&[f64::NAN],&[0.0],&[0.0],0.1,1).is_err());
        assert!(update(&[1.0],&[1e308],&[0.0],&[0.0],0.1,1).is_err());
        assert!(update(&[1.0],&[1.0],&[0.0],&[0.0],0.0,1).is_err());
        assert!(update(&[1.0],&[1.0],&[0.0],&[0.0],0.1,0).is_err());
    }
}
