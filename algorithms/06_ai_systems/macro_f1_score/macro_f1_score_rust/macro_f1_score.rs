//! Unweighted macro F1 across all declared multiclass labels.

/// One-vs-rest F1 per class, including zero for absent classes.
pub fn macro_f1_score(
    actual:&[u8], predicted:&[u8], classes:usize,
)->Result<f64,&'static str>{
    if !(2..=16).contains(&classes) || !(1..=512).contains(&actual.len()) ||
        predicted.len()!=actual.len() || actual.iter().chain(predicted.iter())
            .any(|&x|usize::from(x)>=classes) {
        return Err("invalid classes or aligned label arrays");
    }
    let mut truth=[0usize;16];
    let mut guesses=[0usize;16];
    let mut hits=[0usize;16];
    for (&a,&p) in actual.iter().zip(predicted.iter()) {
        truth[a as usize]+=1;
        guesses[p as usize]+=1;
        if a==p {hits[a as usize]+=1;}
    }
    let mut sum=0.0f64;
    for c in 0..classes {
        let denom=truth[c]+guesses[c];
        if denom>0 {sum+=2.0*hits[c] as f64/denom as f64;}
    }
    Ok(sum/classes as f64)
}

#[cfg(test)]
mod tests {
    use super::macro_f1_score as macro_f1;
    fn near(a:&[u8],p:&[u8],k:usize,want:f64) {
        let result=macro_f1(a,p,k).unwrap();
        assert!((result-want).abs()<1e-12);
    }
    #[test]
    fn examples(){
        near(&[0,1,2,2],&[0,2,1,2],3,0.5);
        near(&[0,0],&[0,0],3,1./3.);
        near(&[0,1,2],&[0,1,2],3,1.);
        near(&[0,0],&[1,1],2,0.);
        near(&[0,1],&[0,1],2,1.);
        near(&[0,1,2],&[0,0,2],3,(2./3.+1.)/3.);
        near(&[0;512],&[0;512],2,0.5);
        near(&[0;512],&[0;512],16,1./16.);
        near(&[15],&[15],16,1./16.);
        near(&[1],&[0],2,0.);
    }
    #[test]
    fn invalid(){
        assert!(macro_f1(&[],&[],2).is_err());
        assert!(macro_f1(&[0],&[],2).is_err());
        assert!(macro_f1(&[0],&[0],1).is_err());
        assert!(macro_f1(&[0],&[0],17).is_err());
        assert!(macro_f1(&[2],&[0],2).is_err());
        assert!(macro_f1(&[0],&[2],2).is_err());
        assert!(macro_f1(&[255],&[0],2).is_err());
        assert!(macro_f1(&[0;513],&[0;513],2).is_err());
    }
}
