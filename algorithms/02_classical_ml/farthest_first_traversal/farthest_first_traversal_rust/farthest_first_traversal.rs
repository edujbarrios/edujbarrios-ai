//! Deterministic farthest-first k-center selection over Euclidean vectors.

/// Select k center indices from row-major points[n*d], starting with row 0.
pub fn farthest_first_traversal(points:&[f64],n:usize,d:usize,k:usize)
    -> Result<Vec<usize>,&'static str> {
    if n==0 || d==0 || k==0 || k>n ||
       n.checked_mul(d).ok_or("dimension overflow")?!=points.len() ||
       !points.iter().all(|x|x.is_finite()) {
        return Err("invalid points, dimensions or k");
    }
    let mut chosen=Vec::with_capacity(k);
    let mut used=vec![false;n];
    let mut nearest=vec![f64::INFINITY;n];
    for step in 0..k {
        let next=if step==0 {
            0
        } else {
            let mut best=None;
            for i in 0..n {
                if used[i] {continue;}
                if best.is_none_or(|j| nearest[i]>nearest[j]) {
                    best=Some(i);
                }
            }
            best.ok_or("no unselected center")?
        };
        chosen.push(next);
        used[next]=true;
        for i in 0..n {
            if used[i] {continue;}
            let mut distance=0.0_f64;
            for j in 0..d {
                let delta=points[i*d+j]-points[next*d+j];
                if !delta.is_finite(){return Err("difference overflow");}
                distance=distance.hypot(delta);
                if !distance.is_finite(){return Err("distance overflow");}
            }
            nearest[i]=nearest[i].min(distance);
        }
    }
    Ok(chosen)
}

#[cfg(test)]
mod tests {
    use super::farthest_first_traversal as select;
    #[test]
    fn selection_and_ties() {
        let points=[0.0,0.0,1.0,0.0,9.0,0.0,10.0,0.0];
        assert_eq!(select(&points,4,2,3),Ok(vec![0,3,1]));
        assert_eq!(select(&points,4,2,4),Ok(vec![0,3,1,2]));
        assert_eq!(select(&[0.0,0.0,0.0,0.0,2.0,0.0,2.0,0.0],4,2,3),
                   Ok(vec![0,2,1]));
        assert_eq!(select(&[0.0],1,1,1),Ok(vec![0]));
        assert_eq!(select(&[0.0,0.0],2,1,2),Ok(vec![0,1]));
    }
    #[test]
    fn invalid() {
        assert!(select(&[],0,1,1).is_err());
        assert!(select(&[0.0],1,1,0).is_err());
        assert!(select(&[0.0],1,1,2).is_err());
        assert!(select(&[0.0],1,2,1).is_err());
        assert!(select(&[0.0,f64::NAN],2,1,1).is_err());
        assert!(select(&[1.7e308,-1.7e308],2,1,2).is_err());
    }
}
