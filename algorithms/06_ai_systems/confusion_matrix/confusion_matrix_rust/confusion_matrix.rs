//! Binary classification confusion counts in (TN,FP,FN,TP) order.

pub fn confusion_matrix(
    actual: &[u8], predicted: &[u8]
) -> Result<(usize,usize,usize,usize),&'static str> {
    if actual.is_empty() || actual.len()!=predicted.len() {
        return Err("nonempty equal-length labels required");
    }
    let mut counts=[0_usize;4];
    for (&a,&p) in actual.iter().zip(predicted) {
        if a>1 || p>1 {return Err("labels must be binary");}
        counts[a as usize*2+p as usize]+=1;
    }
    Ok((counts[0],counts[1],counts[2],counts[3]))
}

#[cfg(test)]
mod tests {
    use super::confusion_matrix;
    #[test]
    fn mixed_and_perfect() {
        assert_eq!(confusion_matrix(&[0,0,1,1,1],&[0,1,1,0,1]),
                   Ok((1,1,1,2)));
        assert_eq!(confusion_matrix(&[0,1],&[0,1]),Ok((1,0,0,1)));
        assert_eq!(confusion_matrix(&[0,0],&[0,0]),Ok((2,0,0,0)));
    }
    #[test]
    fn invalid() {
        assert!(confusion_matrix(&[],&[]).is_err());
        assert!(confusion_matrix(&[0],&[0,1]).is_err());
        assert!(confusion_matrix(&[0,2],&[0,1]).is_err());
        assert!(confusion_matrix(&[0,1],&[0,2]).is_err());
    }
}
