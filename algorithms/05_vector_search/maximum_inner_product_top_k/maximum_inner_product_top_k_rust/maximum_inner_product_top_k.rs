//! Exact MIPS: rank dense vectors by descending dot product with index ties.

/// Search flat row-major candidate matrix, return k distinct original indices.
pub fn maximum_inner_product_top_k(
    query:&[f64],candidates:&[f64],n:usize,k:usize
)->Result<Vec<usize>,&'static str>{
    let d=query.len();
    if d==0 || n==0 || k==0 || k>n ||
       n.checked_mul(d).ok_or("shape overflow")?!=candidates.len() ||
       !query.iter().chain(candidates.iter()).all(|v|v.is_finite()) {
        return Err("invalid query, candidates, or cutoff");
    }
    let mut scores=Vec::with_capacity(n);
    for row in candidates.chunks_exact(d) {
        let mut score=0.0_f64;
        for (&a,&b) in query.iter().zip(row) {
            let product=a*b;
            if !product.is_finite(){return Err("dot product overflow");}
            score+=product;
            if !score.is_finite(){return Err("dot accumulation overflow");}
        }
        scores.push(score);
    }
    let mut ids:Vec<usize>=(0..n).collect();
    ids.sort_by(|&a,&b|{
        scores[b].partial_cmp(&scores[a]).expect("finite scores")
            .then(a.cmp(&b))
    });
    ids.truncate(k);
    Ok(ids)
}

#[cfg(test)]
mod tests {
    use super::maximum_inner_product_top_k as search;
    #[test]
    fn ranking_and_ties() {
        let x=[1.0,0.0,0.0,1.0,1.0,2.0,-1.0,0.0,1.0,2.0];
        assert_eq!(search(&[1.0,2.0],&x,5,5),Ok(vec![2,4,1,0,3]));
        assert_eq!(search(&[0.0,0.0],&x,5,3),Ok(vec![0,1,2]));
        assert_eq!(search(&[1.0],&[5.0],1,1),Ok(vec![0]));
    }
    #[test]
    fn invalid() {
        assert!(search(&[],&[0.0],1,1).is_err());
        assert!(search(&[1.0],&[],0,1).is_err());
        assert!(search(&[1.0],&[1.0],1,0).is_err());
        assert!(search(&[1.0],&[1.0],1,2).is_err());
        assert!(search(&[1.0,2.0],&[1.0],1,1).is_err());
        assert!(search(&[1.0],&[f64::NAN],1,1).is_err());
        assert!(search(&[1e308],&[1e308],1,1).is_err());
        assert!(search(&[1e308,1e308],&[2.0,2.0],1,1).is_err());
    }
}
