//! Exhaustive Euclidean radius search, ordered by distance then row index.

/// Retrieve candidates with L2 distance <= radius from a flat n×d matrix.
pub fn euclidean_radius_search(
    query:&[f64],candidates:&[f64],n:usize,d:usize,radius:f64,
)->Result<Vec<usize>,&'static str>{
    if !(1..=128).contains(&n) || !(1..=32).contains(&d) ||
        query.len()!=d || candidates.len()!=n*d ||
        !radius.is_finite() || !(0.0..=10000.0).contains(&radius) ||
        query.iter().chain(candidates.iter()).any(|x| !x.is_finite() || x.abs()>1000.0) {
        return Err("invalid shape, finite coordinates or radius");
    }
    let mut matches=Vec::new();
    for i in 0..n {
        let mut distance=0.0f64;
        for j in 0..d {
            distance=distance.hypot(query[j]-candidates[i*d+j]);
        }
        if distance<=radius { matches.push((distance,i)); }
    }
    matches.sort_by(|a,b|a.0.total_cmp(&b.0).then(a.1.cmp(&b.1)));
    Ok(matches.into_iter().map(|(_,i)|i).collect())
}

#[cfg(test)]
mod tests {
    use super::euclidean_radius_search as search;
    #[test]
    fn examples(){
        let q=[0.,0.],x=[1.,0.,0.,1.,3.,4.,0.,0.,-1.,0.];
        assert_eq!(search(&q,&x,5,2,1.),Ok(vec![3,0,1,4]));
        assert_eq!(search(&q,&x,5,2,0.),Ok(vec![3]));
        assert_eq!(search(&q,&x,5,2,5.),Ok(vec![3,0,1,4,2]));
        assert_eq!(search(&q,&x,5,2,0.99),Ok(vec![3]));
        assert_eq!(search(&q,&[1.,0.],1,2,0.5),Ok(vec![]));
        assert_eq!(search(&[2.],&[2.,2.,3.],3,1,0.),Ok(vec![0,1]));
        assert_eq!(search(&[0.],&[-1.,1.,0.],3,1,1.),Ok(vec![2,0,1]));
    }
    #[test]
    fn maximum(){
        assert_eq!(search(&[0.;32],&[0.;4096],128,32,0.),Ok((0..128).collect()));
        assert_eq!(search(&[1000.;32],&[-1000.;32],1,32,10000.),Ok(vec![0]));
    }
    #[test]
    fn invalid(){
        assert!(search(&[],&[0.],1,1,0.).is_err());
        assert!(search(&[0.],&[],1,1,0.).is_err());
        assert!(search(&[0.],&[0.],0,1,0.).is_err());
        assert!(search(&[0.],&[0.],129,1,0.).is_err());
        assert!(search(&[0.],&[0.],1,0,0.).is_err());
        assert!(search(&[0.],&[0.],1,1,-1.).is_err());
        assert!(search(&[0.],&[0.],1,1,f64::NAN).is_err());
        assert!(search(&[0.],&[0.],1,1,10001.).is_err());
        assert!(search(&[f64::INFINITY],&[0.],1,1,1.).is_err());
        assert!(search(&[0.],&[1001.],1,1,1.).is_err());
    }
}
