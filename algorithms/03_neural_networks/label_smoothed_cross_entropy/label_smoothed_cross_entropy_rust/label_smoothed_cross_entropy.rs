//! Single-sample label-smoothed softmax cross-entropy and logit gradients.

/// Return (loss, gradient) with targets mixed with a uniform distribution.
pub fn label_smoothed_cross_entropy(
    logits:&[f64],target:usize,epsilon:f64
)->Result<(f64,Vec<f64>),&'static str>{
    let n=logits.len();
    if !(1..=4096).contains(&n) || target>=n ||
       !epsilon.is_finite() || !(0.0..=1.0).contains(&epsilon) ||
       logits.iter().any(|v|!v.is_finite()) {
        return Err("invalid logits, target or smoothing");
    }
    let max=logits.iter().copied().fold(f64::NEG_INFINITY,f64::max);
    let mut shifted=Vec::with_capacity(n);
    let mut weights=Vec::with_capacity(n);
    let mut sum=0.0_f64;
    for &v in logits {
        let value=v-max;
        if !value.is_finite(){return Err("logit difference overflow");}
        let weight=value.exp();
        sum+=weight;
        if !weight.is_finite() || !sum.is_finite(){
            return Err("softmax normalizer overflow");
        }
        shifted.push(value);
        weights.push(weight);
    }
    if sum<=0.0 {return Err("zero softmax normalizer");}
    let logz=sum.ln();
    let mut gradients=Vec::with_capacity(n);
    let mut loss=0.0_f64;
    for i in 0..n {
        let q=epsilon/n as f64 + if i==target {1.0-epsilon} else {0.0};
        let logloss=logz-shifted[i];
        let term=q*logloss;
        loss+=term;
        let gradient=weights[i]/sum-q;
        if !logloss.is_finite() || !term.is_finite() ||
           !loss.is_finite() || !gradient.is_finite(){
            return Err("loss or gradient overflow");
        }
        gradients.push(gradient);
    }
    if loss < -1e-12 {return Err("invalid negative loss");}
    Ok((loss.max(0.0),gradients))
}

#[cfg(test)]
mod tests {
    use super::label_smoothed_cross_entropy as ce;
    #[test]
    fn values_and_singleton() {
        let z=[0.0,3.0_f64.ln()];
        let (loss,g)=ce(&z,1,0.2).unwrap();
        assert!((loss-(-0.1*0.25_f64.ln()-0.9*0.75_f64.ln())).abs()<1e-12);
        assert!((g[0]-0.15).abs()<1e-12 && (g[1]+0.15).abs()<1e-12);
        let (_,g)=ce(&z,1,0.0).unwrap();
        assert!((g[0]-0.25).abs()<1e-12);
        let (_,g)=ce(&z,1,1.0).unwrap();
        assert!((g[0]+0.25).abs()<1e-12 && (g[1]-0.25).abs()<1e-12);
        assert_eq!(ce(&[12.0],0,0.6),Ok((0.0,vec![0.0])));
        assert!((ce(&[0.0,0.0],0,0.5).unwrap().0-2.0_f64.ln()).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        assert!(ce(&[],0,0.2).is_err());
        assert!(ce(&[0.0,1.0],2,0.2).is_err());
        assert!(ce(&[0.0,1.0],1,-0.1).is_err());
        assert!(ce(&[0.0,1.0],1,1.1).is_err());
        assert!(ce(&[0.0,f64::NAN],1,0.2).is_err());
        assert!(ce(&[-1e308,1e308],1,0.2).is_err());
    }
}
