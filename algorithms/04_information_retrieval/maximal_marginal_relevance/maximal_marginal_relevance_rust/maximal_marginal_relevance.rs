//! Greedy Maximal Marginal Relevance over precomputed score vectors.

/// Return indices balancing relevance with similarity to already selected rows.
pub fn maximal_marginal_relevance(
    relevance:&[f64], similarities:&[f64], lambda:f64,k:usize
) -> Result<Vec<usize>,&'static str> {
    let n=relevance.len();
    if n==0 || k==0 || k>n || !lambda.is_finite() || !(0.0..=1.0).contains(&lambda) ||
       n.checked_mul(n).ok_or("dimension overflow")?!=similarities.len() ||
       relevance.iter().any(|v|!v.is_finite() || !(0.0..=1.0).contains(v)) ||
       similarities.iter().any(|v|!v.is_finite() || !(-1.0..=1.0).contains(v)) {
        return Err("invalid relevance, similarity, dimensions, lambda, or k");
    }
    let mut selected=Vec::with_capacity(k);
    let mut used=vec![false;n];
    let mut max_sim=vec![f64::NEG_INFINITY;n];
    for step in 0..k {
        let mut best=None;
        for i in 0..n {
            if used[i] {continue;}
            let novelty=if step==0 {0.0} else {(1.0-lambda)*max_sim[i]};
            let score=lambda*relevance[i]-novelty;
            if !score.is_finite(){return Err("MMR score overflow");}
            if best.is_none_or(|(best_score,_): (f64,usize)|score>best_score) {
                best=Some((score,i));
            }
        }
        let chosen=best.ok_or("no remaining candidate")?.1;
        used[chosen]=true;
        selected.push(chosen);
        for i in 0..n {
            if !used[i] {
                max_sim[i]=max_sim[i].max(similarities[i*n+chosen]);
            }
        }
    }
    Ok(selected)
}

#[cfg(test)]
mod tests {
    use super::maximal_marginal_relevance as mmr;
    #[test]
    fn rank_and_ties() {
        let r=[0.9,0.8,0.7];
        let s=[1.0,0.9,-0.5,0.9,1.0,0.0,-0.5,0.0,1.0];
        assert_eq!(mmr(&r,&s,0.5,3),Ok(vec![0,2,1]));
        assert_eq!(mmr(&r,&s,1.0,3),Ok(vec![0,1,2]));
        assert_eq!(mmr(&r,&s,0.0,3),Ok(vec![0,2,1]));
        assert_eq!(mmr(&[1.0,1.0,1.0],&[1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0],0.5,3),
                   Ok(vec![0,1,2]));
        assert_eq!(mmr(&[1.0],&[1.0],0.5,1),Ok(vec![0]));
    }
    #[test]
    fn invalid() {
        let r=[0.9,0.8,0.7];
        let s=[1.0,0.9,-0.5,0.9,1.0,0.0,-0.5,0.0,1.0];
        assert!(mmr(&[],&[],0.5,1).is_err());
        assert!(mmr(&r,&s,0.5,0).is_err());
        assert!(mmr(&r,&s,0.5,4).is_err());
        assert!(mmr(&r,&s,f64::NAN,1).is_err());
        assert!(mmr(&r,&s,1.1,1).is_err());
        assert!(mmr(&r,&[1.0,0.0],0.5,1).is_err());
        assert!(mmr(&[0.9,0.8,-0.1],&s,0.5,1).is_err());
        assert!(mmr(&r,&[1.0,2.0,-0.5,0.9,1.0,0.0,-0.5,0.0,1.0],0.5,1).is_err());
    }
}
