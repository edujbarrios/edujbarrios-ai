//! Mean Euclidean silhouette coefficient for nontrivial labeled clusters.

/// Return mean silhouette; singleton clusters contribute zero.
pub fn silhouette_score(points:&[f64],labels:&[usize],d:usize)
    ->Result<f64,&'static str> {
    let n=labels.len();
    if !(2..=128).contains(&n) || !(1..=32).contains(&d) ||
       n.checked_mul(d).ok_or("dimension overflow")?!=points.len() ||
       points.iter().any(|v|!v.is_finite()) ||
       labels.iter().any(|&v|v>=n) {
        return Err("invalid dataset or cluster labels");
    }
    let mut counts=vec![0_usize;n];
    for &label in labels {counts[label]+=1;}
    let cluster_count=counts.iter().filter(|&&v|v>0).count();
    if !(2..n).contains(&cluster_count) {
        return Err("require 2..n-1 populated clusters");
    }
    let mut total=0.0_f64;
    for i in 0..n {
        if counts[labels[i]]==1 {continue;}
        let mut sums=vec![0.0_f64;n];
        for j in 0..n {
            if i==j {continue;}
            let mut distance=0.0_f64;
            for col in 0..d {
                let delta=points[i*d+col]-points[j*d+col];
                if !delta.is_finite(){return Err("difference overflow");}
                distance=distance.hypot(delta);
                if !distance.is_finite(){return Err("distance overflow");}
            }
            sums[labels[j]]+=distance;
            if !sums[labels[j]].is_finite(){return Err("distance sum overflow");}
        }
        let a=sums[labels[i]]/(counts[labels[i]]-1) as f64;
        let mut b=f64::INFINITY;
        for c in 0..n {
            if c!=labels[i] && counts[c]>0 {
                b=b.min(sums[c]/counts[c] as f64);
            }
        }
        if !a.is_finite() || !b.is_finite(){return Err("invalid mean distance");}
        let denom=a.max(b);
        total+=if denom==0.0 {0.0} else {(b-a)/denom};
        if !total.is_finite(){return Err("silhouette sum overflow");}
    }
    let result=total/n as f64;
    if !result.is_finite() || !(-1.0-1e-12..=1.0+1e-12).contains(&result) {
        return Err("invalid silhouette score");
    }
    Ok(result.clamp(-1.0,1.0))
}

#[cfg(test)]
mod tests {
    use super::silhouette_score;
    #[test]
    fn examples() {
        let points=[0.0,1.0,5.0,6.0];
        assert!((silhouette_score(&points,&[0,0,1,1],1).unwrap()-79.0/99.0).abs()<1e-12);
        assert!((silhouette_score(&points,&[1,1,3,3],1).unwrap()-79.0/99.0).abs()<1e-12);
        assert_eq!(silhouette_score(&[0.0;4],&[0,0,1,1],1),Ok(0.0));
        assert!(silhouette_score(&points,&[0,0,1,2],1).unwrap().is_finite());
    }
    #[test]
    fn invalid() {
        let x=[0.0,1.0,5.0,6.0];
        assert!(silhouette_score(&x,&[0,1,2,3],1).is_err());
        assert!(silhouette_score(&x,&[0,0,0,0],1).is_err());
        assert!(silhouette_score(&x,&[0,0,1,4],1).is_err());
        assert!(silhouette_score(&x,&[0,0,1],1).is_err());
        assert!(silhouette_score(&[0.0,1.0,f64::NAN,6.0],&[0,0,1,1],1).is_err());
        assert!(silhouette_score(&[-1.7e308,1.7e308,0.0,1.0],&[0,0,1,1],1).is_err());
    }
}
