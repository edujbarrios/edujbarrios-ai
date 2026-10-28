//! Locally typical next-token candidate filtering.
pub fn typical_sampling_filter(logits: &[f64], p: f64)
    -> Result<Vec<bool>, &'static str> {
    if !(1..=4096).contains(&logits.len()) ||
       !p.is_finite() || p<=0.0 || p>1.0 ||
       logits.iter().any(|x|!x.is_finite() || x.abs()>1000.0) {
        return Err("invalid logits or typical mass");
    }
    let maximum=logits.iter().copied().fold(f64::NEG_INFINITY,f64::max);
    let weights:Vec<f64>=logits.iter().map(|s|(s-maximum).exp()).collect();
    let total:f64=weights.iter().sum();
    let logz=total.ln();
    let surprise:Vec<f64>=logits.iter().map(|s|logz-(s-maximum)).collect();
    let entropy:f64=weights.iter().zip(surprise.iter())
        .map(|(w,s)|w/total*s).sum();
    let deviation:Vec<f64>=surprise.iter().map(|s|(s-entropy).abs()).collect();
    let mut order:Vec<usize>=(0..logits.len()).collect();
    order.sort_by(|&a,&b|
        deviation[a].partial_cmp(&deviation[b]).unwrap().then(a.cmp(&b)));
    let mut keep=vec![false;logits.len()];
    let mut mass=0.0;
    for i in order {
        keep[i]=true;
        mass+=weights[i]/total;
        if p<1.0 && mass>=p {break;}
    }
    Ok(keep)
}

#[cfg(test)]
mod tests {
    use super::typical_sampling_filter as filt;
    #[test]
    fn reference() {
        assert_eq!(filt(&[0.0;4],0.6),Ok(vec![true,true,true,false]));
        assert_eq!(filt(&[2.0,1.0,0.0],0.8),Ok(vec![true,true,false]));
        assert_eq!(filt(&[0.0;4],1.0),Ok(vec![true;4]));
        assert_eq!(filt(&[-1000.0,1000.0,0.0],0.8),Ok(vec![false,true,false]));
        assert_eq!(filt(&[5.0],0.5),Ok(vec![true]));
    }
    #[test]
    fn invalid() {
        assert!(filt(&[],0.5).is_err());
        assert!(filt(&vec![0.0;4097],0.5).is_err());
        assert!(filt(&[0.0],0.0).is_err());
        assert!(filt(&[0.0],-1.0).is_err());
        assert!(filt(&[0.0],1.1).is_err());
        assert!(filt(&[0.0],f64::NAN).is_err());
        assert!(filt(&[f64::NAN],0.5).is_err());
        assert!(filt(&[1001.0],0.5).is_err());
    }
}
