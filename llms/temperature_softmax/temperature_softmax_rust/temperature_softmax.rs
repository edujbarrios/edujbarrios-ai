//! Numerically stable next-token temperature softmax.

/// Normalize finite logits with 0.01 <= temperature <= 100.
pub fn temperature_softmax(logits: &[f64], temperature: f64)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=4096).contains(&logits.len()) || !temperature.is_finite() ||
       !(0.01..=100.0).contains(&temperature) ||
       logits.iter().any(|x| !x.is_finite() || x.abs() > 1000.0) {
        return Err("invalid logits or temperature");
    }
    let maximum = logits.iter().copied().fold(f64::NEG_INFINITY, f64::max);
    let weights: Vec<f64> =
        logits.iter().map(|x| ((x - maximum) / temperature).exp()).collect();
    let total: f64 = weights.iter().sum();
    Ok(weights.iter().map(|w| w / total).collect())
}

#[cfg(test)]
mod tests {
    use super::temperature_softmax as softmax;
    #[test]
    fn reference() {
        let expected = 1.0f64.exp() / (1.0+1.0f64.exp());
        assert!((softmax(&[0.0,2.0],2.0).unwrap()[1]-expected).abs()<1e-12);
        assert!((softmax(&[998.0,1000.0],2.0).unwrap()[1]-expected).abs()<1e-12);
        for v in softmax(&[3.0;3],100.0).unwrap() {
            assert!((v-1.0/3.0).abs()<1e-12);
        }
        assert_eq!(softmax(&[-1000.0,1000.0],0.01),Ok(vec![0.0,1.0]));
        assert_eq!(softmax(&[5.0],1.0),Ok(vec![1.0]));
    }
    #[test]
    fn invalid() {
        assert!(softmax(&[],1.0).is_err());
        assert!(softmax(&vec![1.0;4097],1.0).is_err());
        assert!(softmax(&[1.0],0.0).is_err());
        assert!(softmax(&[1.0],0.001).is_err());
        assert!(softmax(&[1.0],101.0).is_err());
        assert!(softmax(&[1.0],f64::NAN).is_err());
        assert!(softmax(&[f64::NAN],1.0).is_err());
        assert!(softmax(&[1001.0],1.0).is_err());
    }
}
