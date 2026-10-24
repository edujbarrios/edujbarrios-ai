//! Exact Hamming-radius search over packed 30-bit LSH signatures.

const MAX_SIGNATURE:u32=(1_u32<<30)-1;

/// Return zero-based indices of all signatures within radius, distance-first.
pub fn hamming_radius_search(query:u32,signatures:&[u32],radius:u32)
    -> Result<Vec<usize>,&'static str> {
    if query>MAX_SIGNATURE || radius>30 ||
       signatures.iter().any(|&value|value>MAX_SIGNATURE) {
        return Err("invalid signature or radius");
    }
    let mut matches:Vec<(u32,usize)>=signatures.iter().enumerate()
        .filter_map(|(index,&value)| {
            let distance=(query^value).count_ones();
            if distance<=radius {Some((distance,index))} else {None}
        }).collect();
    matches.sort_unstable();
    Ok(matches.into_iter().map(|(_,index)|index).collect())
}

#[cfg(test)]
mod tests {
    use super::hamming_radius_search as search;
    #[test]
    fn thresholds_duplicates_and_empty() {
        let values=[3,1,7,15,2];
        assert_eq!(search(3,&values,1),Ok(vec![0,1,2,4]));
        assert_eq!(search(3,&values,0),Ok(vec![0]));
        assert_eq!(search(3,&values,30),Ok(vec![0,1,2,4,3]));
        assert_eq!(search(3,&[3,3,1],0),Ok(vec![0,1]));
        assert_eq!(search(0,&[],0),Ok(vec![]));
        assert_eq!(search(0,&[0,(1<<30)-1],30),Ok(vec![0,1]));
    }
    #[test]
    fn invalid() {
        assert!(search(3,&[3,1<<30],0).is_err());
        assert!(search(1<<30,&[3],0).is_err());
        assert!(search(3,&[3],31).is_err());
    }
}
