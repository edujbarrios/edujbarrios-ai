//! Nonnegative Rocchio relevance-feedback vector update.

/// Return q + 0.75 mean(relevant) - 0.15 mean(nonrelevant), clamped at zero.
/// Relevant/nonrelevant matrices are row-major with width query.len().
pub fn rocchio_relevance_feedback(
    query:&[f64],relevant:&[f64],nr:usize,nonrelevant:&[f64],ni:usize
)->Result<Vec<f64>,&'static str>{
    let d=query.len();
    if !(1..=64).contains(&d) || nr>128 || ni>128 ||
       nr+ni==0 ||
       nr.checked_mul(d).ok_or("shape overflow")?!=relevant.len() ||
       ni.checked_mul(d).ok_or("shape overflow")?!=nonrelevant.len() ||
       query.iter().chain(relevant.iter()).chain(nonrelevant.iter())
         .any(|x|!x.is_finite() || !(0.0..=1e6).contains(x)) {
        return Err("invalid query dimensions or feedback vectors");
    }
    let mut out=Vec::with_capacity(d);
    for j in 0..d {
        let mut pos=0.0_f64;
        let mut neg=0.0_f64;
        for i in 0..nr {pos+=relevant[i*d+j]/nr as f64;}
        for i in 0..ni {neg+=nonrelevant[i*d+j]/ni as f64;}
        let value=query[j]+0.75*pos-0.15*neg;
        if !value.is_finite(){return Err("Rocchio arithmetic overflow");}
        out.push(value.max(0.0));
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::rocchio_relevance_feedback as rocchio;
    #[test]
    fn fixtures(){
        let r=rocchio(&[1.0,1.0],&[2.0,0.0,4.0,2.0],2,
                      &[0.0,2.0,2.0,2.0],2).unwrap();
        assert!((r[0]-3.1).abs()<1e-12 && (r[1]-1.45).abs()<1e-12);
        assert_eq!(rocchio(&[1.0,1.0],&[2.0,0.0,4.0,2.0],2,&[],0),
                   Ok(vec![3.25,1.75]));
        assert_eq!(rocchio(&[0.0,0.0],&[],0,&[1.0,1.0],1),
                   Ok(vec![0.0,0.0]));
        assert_eq!(rocchio(&[0.0],&[0.0],1,&[],0),Ok(vec![0.0]));
    }
    #[test]
    fn invalid() {
        assert!(rocchio(&[1.0],&[],0,&[],0).is_err());
        assert!(rocchio(&[1.0],&[1.0,2.0],1,&[],0).is_err());
        assert!(rocchio(&[1.0],&[f64::NAN],1,&[],0).is_err());
        assert!(rocchio(&[1.0],&[-1.0],1,&[],0).is_err());
        assert!(rocchio(&[],&[1.0],1,&[],0).is_err());
    }
}
