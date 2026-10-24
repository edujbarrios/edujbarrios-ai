//! ROC AUC from sorted classifier scores with average-rank ties.

/// Calculate binary AUC, rejecting single-class or invalid observations.
pub fn roc_auc(scores:&[f64],labels:&[u8]) -> Result<f64,&'static str> {
    let n=scores.len();
    if n<2 || labels.len()!=n ||
       scores.iter().any(|x|!x.is_finite()) ||
       labels.iter().any(|&v|v>1) {
        return Err("invalid aligned finite scores or binary labels");
    }
    let positives=labels.iter().filter(|&&y|y==1).count();
    if positives==0 || positives==n {
        return Err("ROC AUC requires both classes");
    }
    let mut pairs:Vec<(f64,u8)>=scores.iter().copied().zip(labels.iter().copied()).collect();
    pairs.sort_by(|a,b|a.0.partial_cmp(&b.0).expect("finite score"));
    let mut positive_ranks=0.0_f64;
    let mut start=0;
    while start<n {
        let mut end=start+1;
        let mut group_positives=pairs[start].1 as usize;
        while end<n && pairs[end].0==pairs[start].0 {
            group_positives+=pairs[end].1 as usize;
            end+=1;
        }
        positive_ranks+=group_positives as f64*((start+1) as f64+end as f64)/2.0;
        start=end;
    }
    let p=positives as f64;
    let statistic=positive_ranks-p*(p+1.0)/2.0;
    let result=statistic/(p*(n-positives) as f64);
    if !result.is_finite() || !(-1e-12..=1.0+1e-12).contains(&result) {
        return Err("nonfinite ROC AUC");
    }
    Ok(result.clamp(0.0,1.0))
}

#[cfg(test)]
mod tests {
    use super::roc_auc;
    #[test]
    fn examples_and_ties() {
        assert!((roc_auc(&[.1,.4,.35,.8],&[0,0,1,1]).unwrap()-.75).abs()<1e-12);
        assert_eq!(roc_auc(&[.5,.5],&[0,1]),Ok(.5));
        assert_eq!(roc_auc(&[.9,.1],&[0,1]),Ok(0.0));
        assert_eq!(roc_auc(&[.1,.9],&[0,1]),Ok(1.0));
        assert_eq!(roc_auc(&[0.0,0.0,0.0,1.0],&[0,1,0,1]),Ok(.75));
    }
    #[test]
    fn invalid() {
        assert!(roc_auc(&[],&[]).is_err());
        assert!(roc_auc(&[.1],&[1]).is_err());
        assert!(roc_auc(&[.1,.9],&[1,1]).is_err());
        assert!(roc_auc(&[.1,.9],&[0,0]).is_err());
        assert!(roc_auc(&[f64::NAN,.9],&[0,1]).is_err());
        assert!(roc_auc(&[.1,.9],&[0,2]).is_err());
    }
}
