//! Deterministic Euclidean k-nearest neighbor regression on finite targets.

/// Predict unweighted average target of closest k row-major training rows.
pub fn knn_regression(training:&[f64],targets:&[f64],query:&[f64],k:usize)
    ->Result<f64,&'static str>{
    let n=targets.len();
    let d=query.len();
    if !(1..=4096).contains(&n) || !(1..=64).contains(&d) ||
       k==0 || k>n ||
       n.checked_mul(d).ok_or("dimension overflow")?!=training.len() ||
       !training.iter().chain(targets.iter()).chain(query.iter())
           .all(|v|v.is_finite()) {
        return Err("invalid training data, targets, query or cutoff");
    }
    let mut neighbors=Vec::with_capacity(n);
    for (i,row) in training.chunks_exact(d).enumerate() {
        let mut distance=0.0_f64;
        for (&a,&b) in row.iter().zip(query) {
            let delta=a-b;
            if !delta.is_finite(){return Err("coordinate difference overflow");}
            distance=distance.hypot(delta);
            if !distance.is_finite(){return Err("distance overflow");}
        }
        neighbors.push((distance,i));
    }
    neighbors.sort_by(|a,b|{
        a.0.partial_cmp(&b.0).expect("finite distances")
            .then(a.1.cmp(&b.1))
    });
    let mut prediction=0.0_f64;
    for (_,index) in neighbors.iter().take(k) {
        let term=targets[*index]/k as f64;
        if !term.is_finite(){return Err("target contribution overflow");}
        prediction+=term;
        if !prediction.is_finite(){return Err("prediction overflow");}
    }
    Ok(prediction)
}

#[cfg(test)]
mod tests {
    use super::knn_regression as predict;
    #[test]
    fn nearest_and_ties() {
        let data=[0.0,0.0,2.0,0.0,0.0,2.0,5.0,5.0];
        let y=[2.0,4.0,6.0,10.0];
        assert_eq!(predict(&data,&y,&[0.0,0.0],1),Ok(2.0));
        assert_eq!(predict(&data,&y,&[0.0,0.0],2),Ok(3.0));
        assert_eq!(predict(&data,&y,&[0.0,0.0],3),Ok(4.0));
        assert_eq!(predict(&data,&y,&[0.0,0.0],4),Ok(5.5));
        assert_eq!(predict(&[0.0,0.0,0.0,0.0],&[-5.0,7.0],&[0.0,0.0],2),Ok(1.0));
        assert_eq!(predict(&[0.0,0.0],&[2.0],&[0.0,0.0],1),Ok(2.0));
        assert!((predict(&[0.0,0.0,0.0,0.0],&[1e308,1e308],&[0.0,0.0],2)
            .unwrap()/1e308-1.0).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        assert!(predict(&[],&[],&[0.0],1).is_err());
        assert!(predict(&[0.0],&[1.0],&[0.0],0).is_err());
        assert!(predict(&[0.0],&[1.0],&[0.0],2).is_err());
        assert!(predict(&[0.0],&[1.0],&[0.0,0.0],1).is_err());
        assert!(predict(&[f64::NAN],&[1.0],&[0.0],1).is_err());
        assert!(predict(&[-1.7e308,1.7e308],&[0.0,0.0],&[-1.7e308],1).is_err());
        assert!(predict(&[0.0],&[f64::NAN],&[0.0],1).is_err());
    }
}
