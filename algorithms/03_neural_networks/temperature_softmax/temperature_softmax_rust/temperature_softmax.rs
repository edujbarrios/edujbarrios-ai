//! Stable categorical softmax from finite logits with positive temperature.

/// Return probabilities, rejecting unrepresentable shifted/scaled logits.
pub fn temperature_softmax(logits:&[f64],temperature:f64)
    ->Result<Vec<f64>,&'static str>{
    let n=logits.len();
    if !(1..=4096).contains(&n) || !temperature.is_finite() ||
       temperature<=0.0 || logits.iter().any(|v|!v.is_finite()) {
        return Err("invalid logits or temperature");
    }
    let maximum=logits.iter().copied().fold(f64::NEG_INFINITY,f64::max);
    let mut weights=Vec::with_capacity(n);
    let mut sum=0.0_f64;
    for &value in logits {
        let shifted=value-maximum;
        let scaled=shifted/temperature;
        if !shifted.is_finite() || !scaled.is_finite(){
            return Err("scaled logits overflow");
        }
        let weight=scaled.exp();
        sum+=weight;
        if !weight.is_finite() || !sum.is_finite(){
            return Err("exponential overflow");
        }
        weights.push(weight);
    }
    if sum<=0.0 {return Err("zero normalizer");}
    for weight in &mut weights {
        *weight/=sum;
        if !weight.is_finite(){return Err("nonfinite probability");}
    }
    Ok(weights)
}

#[cfg(test)]
mod tests {
    use super::temperature_softmax as softmax;
    #[test]
    fn temperatures_and_equal() {
        let x=[0.0,3.0_f64.ln()];
        let p=softmax(&x,1.0).unwrap();
        assert!((p[0]-0.25).abs()<1e-12 && (p[1]-0.75).abs()<1e-12);
        assert!((softmax(&x,0.5).unwrap()[0]-0.1).abs()<1e-12);
        assert!((softmax(&x,2.0).unwrap()[0]-1.0/(1.0+3.0_f64.sqrt())).abs()<1e-12);
        assert_eq!(softmax(&[10.0,10.0],1.0),Ok(vec![0.5,0.5]));
        assert_eq!(softmax(&[0.0],1.0),Ok(vec![1.0]));
    }
    #[test]
    fn invalid() {
        assert!(softmax(&[],1.0).is_err());
        assert!(softmax(&[0.0,1.0],0.0).is_err());
        assert!(softmax(&[0.0,1.0],-1.0).is_err());
        assert!(softmax(&[0.0,1.0],f64::NAN).is_err());
        assert!(softmax(&[0.0,f64::NAN],1.0).is_err());
        assert!(softmax(&[-1e308,1e308],1.0).is_err());
    }
}
