//! Floyd-Warshall all-pairs shortest distances over directed weighted graphs.

/// Return row-major all-pairs shortest distances; +Infinity means unreachable.
pub fn floyd_warshall(adjacency: &[f64], n: usize)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=64).contains(&n) ||
       n.checked_mul(n).ok_or("dimension overflow")? != adjacency.len() ||
       adjacency.iter().any(|&v| v.is_nan() || v==f64::NEG_INFINITY ||
          (v.is_finite() && !(-1_000_000.0..=1_000_000.0).contains(&v))) {
        return Err("invalid adjacency matrix");
    }
    let mut d=adjacency.to_vec();
    for i in 0..n {
        if d[i*n+i]!=0.0 { return Err("nonzero matrix diagonal"); }
    }
    for k in 0..n {
        for i in 0..n {
            if !d[i*n+k].is_finite(){continue;}
            for j in 0..n {
                if !d[k*n+j].is_finite(){continue;}
                let via=d[i*n+k]+d[k*n+j];
                if !via.is_finite(){return Err("shortest path overflow");}
                if via<d[i*n+j] {d[i*n+j]=via;}
            }
        }
    }
    if (0..n).any(|i| d[i*n+i]<0.0) {
        return Err("negative-weight cycle");
    }
    Ok(d)
}

#[cfg(test)]
mod tests {
    use super::floyd_warshall as paths;
    #[test]
    fn example_and_disconnected() {
        let inf=f64::INFINITY;
        let w=[0.0,3.0,inf,inf,0.0,-2.0,4.0,inf,0.0];
        assert_eq!(paths(&w,3),Ok(vec![0.0,3.0,1.0,2.0,0.0,-2.0,4.0,7.0,0.0]));
        assert_eq!(paths(&[0.0],1),Ok(vec![0.0]));
        assert_eq!(paths(&[0.0,inf,inf,0.0],2),Ok(vec![0.0,inf,inf,0.0]));
    }
    #[test]
    fn invalid() {
        let inf=f64::INFINITY;
        assert!(paths(&[],0).is_err());
        assert!(paths(&[0.0,1.0],2).is_err());
        assert!(paths(&[0.0,-2.0,-2.0,0.0],2).is_err());
        assert!(paths(&[1.0,inf,inf,0.0],2).is_err());
        assert!(paths(&[0.0,f64::NAN,inf,0.0],2).is_err());
        assert!(paths(&[0.0,f64::NEG_INFINITY,inf,0.0],2).is_err());
        assert!(paths(&[0.0,1_000_001.0,inf,0.0],2).is_err());
    }
}
