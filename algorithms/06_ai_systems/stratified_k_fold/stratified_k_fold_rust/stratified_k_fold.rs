//! Deterministic binary-label stratified K-fold assignment.

/// Each fold gets both classes and nearly equal total validation size.
pub fn stratified_k_fold(labels:&[u8],k:usize)->Result<Vec<usize>,&'static str>{
    let n=labels.len();
    if !(2..=4096).contains(&n) || k<2 || k>n ||
       labels.iter().any(|&y|y>1) {
        return Err("invalid binary labels or fold count");
    }
    let mut counts=[0_usize;2];
    for &y in labels {counts[y as usize]+=1;}
    if counts[0]<k || counts[1]<k {
        return Err("each class needs at least k examples");
    }
    let mut folds=vec![0_usize;n];
    let mut counter=0_usize;
    for class in [0_u8,1_u8] {
        for (i,&y) in labels.iter().enumerate() {
            if y==class {
                folds[i]=counter%k;
                counter+=1;
            }
        }
    }
    Ok(folds)
}

#[cfg(test)]
mod tests {
    use super::stratified_k_fold as split;
    #[test]
    fn balanced_folds() {
        assert_eq!(split(&[0,1,0,1,0,1],3),Ok(vec![0,0,1,1,2,2]));
        assert_eq!(split(&[0,0,0,0,1,1,1,1],3),Ok(vec![0,1,2,0,1,2,0,1]));
        let labels=[0,0,0,0,0,0,0,1,1,1,1,1];
        let folds=split(&labels,4).unwrap();
        let sizes:Vec<usize>=(0..4).map(|f|folds.iter().filter(|&&v|v==f).count()).collect();
        assert!(sizes.iter().max().unwrap()-sizes.iter().min().unwrap()<=1);
        for f in 0..4 {
            assert!(labels.iter().zip(folds.iter()).any(|(&y,&v)|y==0 && v==f));
            assert!(labels.iter().zip(folds.iter()).any(|(&y,&v)|y==1 && v==f));
        }
    }
    #[test]
    fn errors() {
        assert!(split(&[],2).is_err());
        assert!(split(&[0,1],1).is_err());
        assert!(split(&[0,1],2).is_err());
        assert!(split(&[0,0,1,1],3).is_err());
        assert!(split(&[0,0,1,2],2).is_err());
    }
}
