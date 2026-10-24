//! Brier score for binary probabilistic classifiers.
/// Return the average squared probability error over finite class-one scores.
pub fn brier_score(probabilities:&[f64],labels:&[u8])
    -> Result<f64,&'static str> {
    if probabilities.is_empty() || probabilities.len()!=labels.len() {
        return Err("nonempty aligned probabilities and labels required");
    }
    let mut sum=0.0_f64;
    for (&p,&y) in probabilities.iter().zip(labels.iter()) {
        if !p.is_finite() || !(0.0..=1.0).contains(&p) || y>1 {
            return Err("probabilities must be in 0..1 and labels 0/1");
        }
        let error=p-y as f64;
        let squared=error*error;
        sum+=squared;
        if !squared.is_finite() || !sum.is_finite() {
            return Err("Brier accumulation overflow");
        }
    }
    let result=sum/probabilities.len() as f64;
    if !result.is_finite() || !(0.0..=1.0+1e-12).contains(&result) {
        return Err("invalid Brier result");
    }
    Ok(result.min(1.0))
}

#[cfg(test)]
mod tests {
    use super::brier_score;
    #[test]
    fn fixtures() {
        assert!((brier_score(&[0.1,0.8,0.6,0.2],&[0,1,1,0]).unwrap()-0.0625).abs()<1e-12);
        assert_eq!(brier_score(&[0.0,1.0],&[0,1]),Ok(0.0));
        assert_eq!(brier_score(&[1.0,0.0],&[0,1]),Ok(1.0));
        assert_eq!(brier_score(&[0.5,0.5],&[0,1]),Ok(0.25));
        assert_eq!(brier_score(&[0.5],&[0]),Ok(0.25));
    }
    #[test]
    fn invalid() {
        assert!(brier_score(&[],&[]).is_err());
        assert!(brier_score(&[0.2],&[]).is_err());
        assert!(brier_score(&[-0.1,0.5],&[0,1]).is_err());
        assert!(brier_score(&[1.1,0.5],&[0,1]).is_err());
        assert!(brier_score(&[f64::NAN,0.5],&[0,1]).is_err());
        assert!(brier_score(&[0.0,1.0],&[0,2]).is_err());
    }
}
