//! Deterministic top-p nucleus candidate selection without random sampling.

/// Return zero-based indices sorted by probability, stopping at cumulative p.
pub fn top_p_nucleus_filtering(probabilities:&[f64],threshold:f64)
    ->Result<Vec<usize>,&'static str>{
    let n=probabilities.len();
    if !(1..=4096).contains(&n) || !threshold.is_finite() ||
       !(0.0<=(threshold)).then_some(()).is_some() ||
       threshold<=0.0 || threshold>1.0 ||
       probabilities.iter().any(|x|!x.is_finite() || !(0.0..=1.0).contains(x)) {
        return Err("invalid probability distribution or threshold");
    }
    let sum:f64=probabilities.iter().sum();
    if !sum.is_finite() || (sum-1.0).abs()>1e-9 {
        return Err("probabilities must sum to 1");
    }
    let mut ranked:Vec<usize>=(0..n).collect();
    ranked.sort_by(|&a,&b|{
        probabilities[b].partial_cmp(&probabilities[a]).expect("finite probabilities")
            .then(a.cmp(&b))
    });
    let mut selected=Vec::new();
    let mut cumulative=0.0_f64;
    for index in ranked {
        cumulative+=probabilities[index];
        selected.push(index);
        if cumulative>=threshold {break;}
    }
    Ok(selected)
}

#[cfg(test)]
mod tests {
    use super::top_p_nucleus_filtering as nucleus;
    #[test]
    fn prefixes_and_ties() {
        let p=[0.4,0.3,0.2,0.1];
        assert_eq!(nucleus(&p,0.6),Ok(vec![0,1]));
        assert_eq!(nucleus(&p,0.85),Ok(vec![0,1,2]));
        assert_eq!(nucleus(&p,0.95),Ok(vec![0,1,2,3]));
        assert_eq!(nucleus(&[0.5,0.5,0.0],0.5),Ok(vec![0]));
        assert_eq!(nucleus(&[0.5,0.5,0.0],0.75),Ok(vec![0,1]));
        assert_eq!(nucleus(&[0.5,0.5,0.0],1.0),Ok(vec![0,1]));
        assert_eq!(nucleus(&[1.0],1.0),Ok(vec![0]));
    }
    #[test]
    fn invalid() {
        assert!(nucleus(&[],0.5).is_err());
        assert!(nucleus(&[0.7,0.7],0.5).is_err());
        assert!(nucleus(&[1.1,-0.1],0.5).is_err());
        assert!(nucleus(&[f64::NAN,1.0],0.5).is_err());
        assert!(nucleus(&[0.5,0.5],0.0).is_err());
        assert!(nucleus(&[0.5,0.5],1.1).is_err());
    }
}
