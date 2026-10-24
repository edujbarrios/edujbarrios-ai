//! Binary AP@K with min(total_relevant, k) normalization.

pub fn average_precision_at_k(
    relevance:&[u8],k:usize,total_relevant:usize
) -> Result<f64,&'static str> {
    if relevance.is_empty() || k==0 || k>relevance.len() ||
       total_relevant>i32::MAX as usize ||
       relevance.iter().any(|&v|v>1) {
        return Err("invalid ranked relevance, k or corpus count");
    }
    let observed:usize=relevance.iter().map(|&x|x as usize).sum();
    if observed>total_relevant {
        return Err("total relevant below observed relevance");
    }
    if total_relevant==0 {return Ok(0.0);}
    let mut hits=0_usize;
    let mut weighted=0.0_f64;
    for (i,&flag) in relevance[..k].iter().enumerate() {
        hits+=flag as usize;
        if flag==1 {
            weighted+=hits as f64/(i+1) as f64;
        }
    }
    let score=weighted/total_relevant.min(k) as f64;
    if !score.is_finite() || !(0.0..=1.0+1e-12).contains(&score) {
        return Err("invalid AP@K output");
    }
    Ok(score.min(1.0))
}

#[cfg(test)]
mod tests {
    use super::average_precision_at_k as ap;
    #[test]
    fn cutoffs() {
        let r=[1,0,1,0,1];
        assert!((ap(&r,2,3).unwrap()-0.5).abs()<1e-12);
        assert!((ap(&r,4,3).unwrap()-5.0/9.0).abs()<1e-12);
        assert!((ap(&r,5,3).unwrap()-34.0/45.0).abs()<1e-12);
        assert_eq!(ap(&[0,0],2,0),Ok(0.0));
        assert_eq!(ap(&[1,1,1],3,3),Ok(1.0));
        assert_eq!(ap(&[0,0,1],2,1),Ok(0.0));
        assert_eq!(ap(&[1,1,1],2,3),Ok(1.0));
    }
    #[test]
    fn invalid() {
        assert!(ap(&[],1,0).is_err());
        assert!(ap(&[1,0],0,1).is_err());
        assert!(ap(&[1,0],3,1).is_err());
        assert!(ap(&[1,0,1],1,1).is_err());
        assert!(ap(&[0,0,2],1,2).is_err());
        assert!(ap(&[1],1,0).is_err());
        assert!(ap(&[1],1,i32::MAX as usize+1).is_err());
    }
}
