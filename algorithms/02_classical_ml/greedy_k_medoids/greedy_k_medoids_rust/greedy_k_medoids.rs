//! BUILD-style greedy k-medoid selection over a dense Euclidean matrix.

/// Return k distinct zero-based representative row indices (not PAM swaps).
pub fn greedy_k_medoids(points:&[f64],n:usize,d:usize,k:usize)
    ->Result<Vec<usize>,&'static str>{
    if !(1..=128).contains(&n) || !(1..=32).contains(&d) ||
       k==0 || k>n || n.checked_mul(d).ok_or("shape overflow")?!=points.len() ||
       points.iter().any(|v|!v.is_finite()) {
        return Err("invalid dataset dimensions, coordinates or k");
    }
    let mut distances=vec![0.0_f64;n*n];
    for i in 0..n {
        for j in i..n {
            let mut distance=0.0_f64;
            for h in 0..d {
                let delta=points[i*d+h]-points[j*d+h];
                if !delta.is_finite(){return Err("coordinate difference overflow");}
                distance=distance.hypot(delta);
                if !distance.is_finite(){return Err("Euclidean distance overflow");}
            }
            distances[i*n+j]=distance;
            distances[j*n+i]=distance;
        }
    }
    let mut selected=Vec::with_capacity(k);
    let mut chosen=vec![false;n];
    let mut nearest=vec![f64::INFINITY;n];
    for _ in 0..k {
        let mut best=None;
        for candidate in 0..n {
            if chosen[candidate] {continue;}
            let mut cost=0.0_f64;
            for i in 0..n {
                cost+=nearest[i].min(distances[i*n+candidate]);
                if !cost.is_finite(){return Err("objective sum overflow");}
            }
            if best.is_none_or(|(best_cost,_): (f64,usize)|cost<best_cost) {
                best=Some((cost,candidate));
            }
        }
        let center=best.ok_or("no candidate left")?.1;
        selected.push(center);
        chosen[center]=true;
        for i in 0..n {
            nearest[i]=nearest[i].min(distances[i*n+center]);
        }
    }
    Ok(selected)
}

#[cfg(test)]
mod tests {
    use super::greedy_k_medoids as select;
    #[test]
    fn examples_and_ties() {
        let x=[0.0,1.0,9.0,10.0];
        assert_eq!(select(&x,4,1,1),Ok(vec![1]));
        assert_eq!(select(&x,4,1,2),Ok(vec![1,2]));
        assert_eq!(select(&x,4,1,3),Ok(vec![1,2,0]));
        assert_eq!(select(&x,4,1,4),Ok(vec![1,2,0,3]));
        assert_eq!(select(&[0.0,0.0,0.0],3,1,3),Ok(vec![0,1,2]));
        assert_eq!(select(&[5.0],1,1,1),Ok(vec![0]));
    }
    #[test]
    fn invalid() {
        assert!(select(&[],0,1,1).is_err());
        assert!(select(&[0.0],1,1,0).is_err());
        assert!(select(&[0.0],1,1,2).is_err());
        assert!(select(&[0.0],1,2,1).is_err());
        assert!(select(&[f64::NAN,1.0],2,1,1).is_err());
        assert!(select(&[-1.7e308,1.7e308],2,1,1).is_err());
    }
}
