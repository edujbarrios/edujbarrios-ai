//! Nucleus filtering with stable score/ID ordering and softmax weights.

/// Return the smallest nonempty candidate mask with cumulative mass >= top_p.
pub fn top_p_logit_filter(logits: &[f64], top_p: f64)
    -> Result<Vec<bool>, &'static str> {
    if !(1..=4096).contains(&logits.len()) ||
        !top_p.is_finite() || top_p <= 0.0 || top_p > 1.0 ||
        logits.iter().any(|x| !x.is_finite() || x.abs() > 1000.0) {
        return Err("invalid logits or nucleus threshold");
    }
    let maximum = logits.iter().copied().fold(f64::NEG_INFINITY, f64::max);
    let weights: Vec<f64> = logits.iter().map(|x| (x-maximum).exp()).collect();
    let total: f64 = weights.iter().sum();
    let mut order: Vec<usize> = (0..logits.len()).collect();
    order.sort_by(|&a,&b| logits[b].partial_cmp(&logits[a]).unwrap().then(a.cmp(&b)));
    let mut keep = vec![false; logits.len()];
    let mut cumulative = 0.0;
    for i in order {
        keep[i] = true;
        cumulative += weights[i]/total;
        if top_p < 1.0 && cumulative >= top_p {
            break;
        }
    }
    Ok(keep)
}

#[cfg(test)]
mod tests {
    use super::top_p_logit_filter as filt;
    #[test]
    fn reference_cases() {
        assert_eq!(filt(&[0.0;4],0.6),Ok(vec![true,true,true,false]));
        assert_eq!(filt(&[0.0;4],0.1),Ok(vec![true,false,false,false]));
        assert_eq!(filt(&[0.0;4],1.0),Ok(vec![true;4]));
        assert_eq!(filt(&[2.0,1.0,0.0],0.5),Ok(vec![true,false,false]));
        assert_eq!(filt(&[2.0,1.0,0.0],0.8),Ok(vec![true,true,false]));
        assert_eq!(filt(&[-1000.0,1000.0,0.0],0.9),Ok(vec![false,true,false]));
        assert_eq!(filt(&[5.0],0.3),Ok(vec![true]));
    }
    #[test]
    fn invalid_cases() {
        assert!(filt(&[],0.5).is_err());
        assert!(filt(&vec![0.0;4097],0.5).is_err());
        assert!(filt(&[0.0],0.0).is_err());
        assert!(filt(&[0.0],-0.1).is_err());
        assert!(filt(&[0.0],1.1).is_err());
        assert!(filt(&[0.0],f64::NAN).is_err());
        assert!(filt(&[f64::INFINITY],0.5).is_err());
        assert!(filt(&[1001.0],0.5).is_err());
    }
}
