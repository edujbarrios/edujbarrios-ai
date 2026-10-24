//! Full-scan Euclidean Top-K nearest-neighbor ranking.

/// Rank row-major candidates[n*d] against the query of length d.
pub fn euclidean_top_k(query:&[f64],candidates:&[f64],n:usize,k:usize)
    -> Result<Vec<usize>,&'static str> {
    let d=query.len();
    if d==0 || n==0 || k==0 || k>n ||
       n.checked_mul(d).ok_or("shape overflow")?!=candidates.len() ||
       !query.iter().chain(candidates.iter()).all(|v|v.is_finite()) {
        return Err("invalid dimensions, k, or finite data");
    }
    let mut distances=Vec::with_capacity(n);
    for row in candidates.chunks_exact(d) {
        let mut distance=0.0_f64;
        for (&a,&b) in query.iter().zip(row) {
            let delta=a-b;
            if !delta.is_finite(){return Err("coordinate difference overflow");}
            distance=distance.hypot(delta);
            if !distance.is_finite(){return Err("distance overflow");}
        }
        distances.push(distance);
    }
    let mut indices:Vec<usize>=(0..n).collect();
    indices.sort_by(|&a,&b|{
        distances[a].partial_cmp(&distances[b]).expect("finite distances")
            .then(a.cmp(&b))
    });
    indices.truncate(k);
    Ok(indices)
}

#[cfg(test)]
mod tests {
    use super::euclidean_top_k as search;
    #[test]
    fn rank_and_ties() {
        assert_eq!(search(&[0.0,0.0],
            &[1.0,0.0,0.0,1.0,3.0,4.0,0.0,0.0,-1.0,0.0],5,5),
            Ok(vec![3,0,1,4,2]));
        assert_eq!(search(&[0.0,0.0],
            &[1.0,0.0,1.0,0.0,0.0,0.0],3,3),Ok(vec![2,0,1]));
        assert_eq!(search(&[0.0,0.0],
            &[1e200,0.0,2e200,0.0],2,2),Ok(vec![0,1]));
    }
    #[test]
    fn invalid() {
        assert!(search(&[],&[0.0],1,1).is_err());
        assert!(search(&[0.0],&[],0,1).is_err());
        assert!(search(&[0.0],&[0.0],1,0).is_err());
        assert!(search(&[0.0,0.0],&[0.0],1,1).is_err());
        assert!(search(&[0.0,0.0],&[0.0,0.0,f64::NAN,0.0],2,1).is_err());
        assert!(search(&[1.7e308],&[-1.7e308],1,1).is_err());
    }
}
