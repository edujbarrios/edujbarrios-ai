//! Mean binary hinge loss and score subgradient at unit margin.

/// At exactly margin one, choose zero as the deterministic subgradient.
pub fn hinge_loss(scores:&[f64],labels:&[i8])
    ->Result<(f64,Vec<f64>),&'static str>{
    let n=scores.len();
    if n==0 || labels.len()!=n ||
       !scores.iter().all(|x|x.is_finite()) ||
       labels.iter().any(|&y|y!=-1 && y!=1) {
        return Err("invalid finite scores or signed class labels");
    }
    let mut accumulated=0.0_f64;
    let mut gradients=Vec::with_capacity(n);
    for (&score,&label) in scores.iter().zip(labels.iter()) {
        let margin=label as f64*score;
        let deficit=1.0-margin;
        if !margin.is_finite() || !deficit.is_finite(){
            return Err("hinge margin overflow");
        }
        accumulated+=deficit.max(0.0);
        if !accumulated.is_finite(){return Err("loss accumulation overflow");}
        gradients.push(if margin<1.0 {-(label as f64)/n as f64} else {0.0});
    }
    let result=accumulated/n as f64;
    if !result.is_finite(){return Err("loss overflow");}
    Ok((result,gradients))
}

#[cfg(test)]
mod tests {
    use super::hinge_loss;
    #[test]
    fn example_and_kink() {
        assert_eq!(hinge_loss(&[2.0,0.0,-0.5,1.0],&[1,1,-1,-1]),
                   Ok((0.875,vec![0.0,-0.25,0.25,0.25])));
        assert_eq!(hinge_loss(&[1.0,-1.0],&[1,-1]),
                   Ok((0.0,vec![0.0,0.0])));
        assert_eq!(hinge_loss(&[0.0],&[-1]),Ok((1.0,vec![1.0])));
    }
    #[test]
    fn invalid() {
        assert!(hinge_loss(&[],&[]).is_err());
        assert!(hinge_loss(&[0.0],&[]).is_err());
        assert!(hinge_loss(&[0.0],&[0]).is_err());
        assert!(hinge_loss(&[f64::NAN],&[1]).is_err());
        assert!(hinge_loss(&[-1e308,-1e308],&[1,1]).is_err());
    }
}
