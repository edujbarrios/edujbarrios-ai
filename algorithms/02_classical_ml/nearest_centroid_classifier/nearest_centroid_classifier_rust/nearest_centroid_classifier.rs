//! Binary centroid classifier: training and Euclidean-distance inference.

/// Train two class-major d-dimensional centroid rows from row-major X[n*d].
pub fn nearest_centroid_classifier(x:&[f64],labels:&[u8],d:usize)
    ->Result<Vec<f64>,&'static str> {
    let n=labels.len();
    if !(2..=4096).contains(&n) || !(1..=64).contains(&d) ||
       n.checked_mul(d).ok_or("shape overflow")?!=x.len() ||
       labels.iter().any(|&v|v>1) || x.iter().any(|v|!v.is_finite()) {
        return Err("invalid finite data or binary labels");
    }
    let mut sums=vec![0.0_f64;2*d];
    let mut members=[0_usize;2];
    for i in 0..n {
        let c=labels[i] as usize;
        members[c]+=1;
        for j in 0..d {
            sums[c*d+j]+=x[i*d+j];
            if !sums[c*d+j].is_finite(){return Err("class feature sum overflow");}
        }
    }
    if members.contains(&0){return Err("both classes required");}
    for c in 0..2 {
        for j in 0..d {
            sums[c*d+j]/=members[c] as f64;
            if !sums[c*d+j].is_finite(){return Err("centroid overflow");}
        }
    }
    Ok(sums)
}

/// Classify one query by nearest centroid, tie toward class zero.
pub fn nearest_centroid_classifier_predict(centroids:&[f64],query:&[f64])
    ->Result<u8,&'static str> {
    let d=query.len();
    if !(1..=64).contains(&d) || d.checked_mul(2).ok_or("shape overflow")?!=centroids.len() ||
       !centroids.iter().chain(query.iter()).all(|v|v.is_finite()) {
        return Err("invalid fitted centroids or query");
    }
    let mut distances=[0.0_f64;2];
    for j in 0..d {
        for c in 0..2 {
            let diff=query[j]-centroids[c*d+j];
            if !diff.is_finite(){return Err("distance difference overflow");}
            distances[c]=distances[c].hypot(diff);
            if !distances[c].is_finite(){return Err("distance overflow");}
        }
    }
    Ok(u8::from(distances[1]<distances[0]))
}

#[cfg(test)]
mod tests {
    use super::{nearest_centroid_classifier as fit,
                nearest_centroid_classifier_predict as predict};
    #[test]
    fn example() {
        let centers=fit(&[0.0,0.0,2.0,0.0,8.0,0.0,10.0,0.0],&[0,0,1,1],2).unwrap();
        assert_eq!(centers,vec![1.0,0.0,9.0,0.0]);
        assert_eq!(predict(&centers,&[2.0,0.0]),Ok(0));
        assert_eq!(predict(&centers,&[8.0,0.0]),Ok(1));
        assert_eq!(predict(&centers,&[5.0,0.0]),Ok(0));
    }
    #[test]
    fn invalid() {
        assert!(fit(&[],&[],1).is_err());
        assert!(fit(&[0.0],&[0],1).is_err());
        assert!(fit(&[0.0,1.0],&[0,0],1).is_err());
        assert!(fit(&[0.0,1.0],&[0,2],1).is_err());
        assert!(fit(&[0.0,f64::NAN],&[0,1],1).is_err());
        assert!(predict(&[0.0,9.0],&[f64::NAN]).is_err());
        assert!(predict(&[0.0,9.0],&[1.0,2.0]).is_err());
        assert!(predict(&[-1.7e308,1.7e308],&[1.7e308]).is_err());
    }
}
