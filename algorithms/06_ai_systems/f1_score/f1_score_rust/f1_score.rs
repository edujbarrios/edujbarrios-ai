//! Binary hard-label F1, with zero for a zero denominator.

pub fn f1_score(actual: &[u8], predicted: &[u8]) -> Result<f64,&'static str> {
    if actual.is_empty() || actual.len()!=predicted.len() {
        return Err("nonempty aligned labels required");
    }
    let (mut tp,mut fp,mut fn_)=(0_usize,0_usize,0_usize);
    for (&y,&p) in actual.iter().zip(predicted) {
        if y>1 || p>1 {return Err("invalid binary label");}
        if y==1 && p==1 {tp+=1;}
        else if y==0 && p==1 {fp+=1;}
        else if y==1 && p==0 {fn_+=1;}
    }
    let numerator=2.0*tp as f64;
    let denominator=numerator+fp as f64+fn_ as f64;
    let score=if denominator==0.0 {0.0}else{numerator/denominator};
    if !score.is_finite() || !(0.0..=1.0).contains(&score) {
        return Err("non-finite F1 score");
    }
    Ok(score)
}

#[cfg(test)]
mod tests {
    use super::f1_score;
    #[test]
    fn outcomes() {
        let f=f1_score(&[0,0,1,1,1],&[0,1,1,0,1]).unwrap();
        assert!((f-2.0/3.0).abs()<1e-12);
        assert_eq!(f1_score(&[0,0],&[0,0]),Ok(0.0));
        assert_eq!(f1_score(&[1,1],&[1,1]),Ok(1.0));
        assert_eq!(f1_score(&[1,0],&[0,1]),Ok(0.0));
    }
    #[test]
    fn invalid() {
        assert!(f1_score(&[],&[]).is_err());
        assert!(f1_score(&[1],&[]).is_err());
        assert!(f1_score(&[0,2],&[0,1]).is_err());
        assert!(f1_score(&[0,1],&[0,2]).is_err());
    }
}
