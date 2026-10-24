//! Stable log-sum-exp and softmax gradient on a finite vector.

/// Return (log(sum(exp(x))), dLSE/dx) with maximum subtraction.
pub fn logsumexp(values:&[f64])->Result<(f64,Vec<f64>),&'static str>{
    if !(1..=4096).contains(&values.len()) ||
       values.iter().any(|v|!v.is_finite()) {
        return Err("nonempty finite logit vector required");
    }
    let maximum=values.iter().copied().fold(f64::NEG_INFINITY,f64::max);
    let mut weights=Vec::with_capacity(values.len());
    let mut normalizer=0.0_f64;
    for &value in values {
        let shifted=value-maximum;
        if !shifted.is_finite(){return Err("shifted coordinate overflow");}
        let weight=shifted.exp();
        normalizer+=weight;
        if !weight.is_finite() || !normalizer.is_finite(){
            return Err("exp normalization overflow");
        }
        weights.push(weight);
    }
    if normalizer<=0.0 {return Err("invalid normalizer");}
    let result=maximum+normalizer.ln();
    if !result.is_finite(){return Err("nonfinite logsumexp");}
    for weight in &mut weights {
        *weight/=normalizer;
        if !weight.is_finite(){return Err("nonfinite gradient");}
    }
    Ok((result,weights))
}

#[cfg(test)]
mod tests {
    use super::logsumexp;
    #[test]
    fn examples() {
        let (v,g)=logsumexp(&[0.0,0.0]).unwrap();
        assert!((v-2.0_f64.ln()).abs()<1e-12);
        assert_eq!(g,vec![0.5,0.5]);
        let (v,g)=logsumexp(&[2.0_f64.ln(),3.0_f64.ln()]).unwrap();
        assert!((v-5.0_f64.ln()).abs()<1e-12);
        assert!((g[0]-0.4).abs()<1e-12 && (g[1]-0.6).abs()<1e-12);
        let (v,g)=logsumexp(&[-1000.0,-1000.0]).unwrap();
        assert!((v-(-1000.0+2.0_f64.ln())).abs()<1e-12);
        assert_eq!(g,vec![0.5,0.5]);
        assert_eq!(logsumexp(&[5.0]),Ok((5.0,vec![1.0])));
    }
    #[test]
    fn invalid() {
        assert!(logsumexp(&[]).is_err());
        assert!(logsumexp(&[f64::NAN,0.0]).is_err());
        assert!(logsumexp(&[f64::INFINITY]).is_err());
        assert!(logsumexp(&[-1e308,1e308]).is_err());
    }
}
