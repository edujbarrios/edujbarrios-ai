//! Exact full-scan cosine Top-K ranking with deterministic index ties.

fn norm(vector: &[f64]) -> Result<f64,&'static str> {
    let mut magnitude=0.0_f64;
    for &value in vector {
        if !value.is_finite(){return Err("non-finite component");}
        magnitude=magnitude.hypot(value);
        if !magnitude.is_finite(){return Err("norm overflow");}
    }
    if magnitude<=0.0 {return Err("zero vector");}
    Ok(magnitude)
}

/// Rank n row-major d-dimensional vectors by descending cosine to query.
pub fn exact_cosine_top_k(query: &[f64], candidates: &[f64],
                          n: usize, k: usize) -> Result<Vec<usize>,&'static str> {
    let d=query.len();
    if d==0 || n==0 || k==0 || k>n ||
       n.checked_mul(d).ok_or("dimension overflow")?!=candidates.len() {
        return Err("invalid dimensions or k");
    }
    let qnorm=norm(query)?;
    let mut scores=Vec::with_capacity(n);
    for row in candidates.chunks_exact(d) {
        let vnorm=norm(row)?;
        let mut score=0.0_f64;
        for (&a,&b) in query.iter().zip(row) {
            let term=(a/qnorm)*(b/vnorm);
            score+=term;
            if !term.is_finite() || !score.is_finite(){
                return Err("cosine score overflow");
            }
        }
        scores.push(score.clamp(-1.0,1.0));
    }
    let mut indices:Vec<usize>=(0..n).collect();
    indices.sort_by(|&a,&b| {
        scores[b].partial_cmp(&scores[a]).expect("finite cosine")
            .then(a.cmp(&b))
    });
    indices.truncate(k);
    Ok(indices)
}

#[cfg(test)]
mod tests {
    use super::exact_cosine_top_k as search;
    #[test]
    fn rank_and_ties() {
        assert_eq!(search(&[1.0,0.0],
                          &[1.0,0.0,0.0,1.0,-1.0,0.0,1.0,1.0,1.0,0.0],5,4),
                   Ok(vec![0,4,3,1]));
        assert_eq!(search(&[1.0],&[-2.0,3.0,3.0],3,3),Ok(vec![1,2,0]));
        assert_eq!(search(&[1.0],&[1.0],1,1),Ok(vec![0]));
    }
    #[test]
    fn invalid() {
        assert!(search(&[],&[1.0],1,1).is_err());
        assert!(search(&[1.0],&[],0,1).is_err());
        assert!(search(&[1.0],&[1.0],1,0).is_err());
        assert!(search(&[1.0,0.0],&[1.0,0.0,0.0,0.0],2,1).is_err());
        assert!(search(&[1.0,0.0],&[1.0,0.0,f64::NAN,1.0],2,1).is_err());
        assert!(search(&[1.7e308,1.7e308],&[1.0,0.0],1,1).is_err());
    }
}
