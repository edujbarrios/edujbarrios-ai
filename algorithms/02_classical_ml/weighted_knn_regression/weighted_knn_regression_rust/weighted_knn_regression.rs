//! Inverse-distance-weighted k-NN regression, with exact-match averaging.

/// Predict a finite real target from k nearest flat-row-major training vectors.
pub fn weighted_knn_regression(
    train:&[f64],targets:&[f64],query:&[f64],n:usize,d:usize,k:usize,
)->Result<f64,&'static str>{
    if !(1..=128).contains(&n) || !(1..=16).contains(&d) ||
       !(1..=n).contains(&k) || train.len()!=n*d ||
       targets.len()!=n || query.len()!=d ||
       train.iter().chain(targets.iter()).chain(query.iter())
           .any(|x|!x.is_finite() || x.abs()>1000.0) {
        return Err("invalid training shape, finite values or k");
    }
    let mut neighbors=Vec::with_capacity(n);
    for i in 0..n {
        let mut distance=0.0f64;
        for j in 0..d {
            distance=distance.hypot(train[i*d+j]-query[j]);
        }
        neighbors.push((distance,i));
    }
    neighbors.sort_by(|a,b|a.0.total_cmp(&b.0).then(a.1.cmp(&b.1)));
    let selected=&neighbors[..k];
    let zeros=selected.iter().filter(|x|x.0==0.0).count();
    if zeros>0 {
        let result=selected.iter()
            .filter(|x|x.0==0.0)
            .map(|x|targets[x.1]/zeros as f64).sum::<f64>();
        return Ok(result);
    }
    let closest=selected[0].0;
    let mut numerator=0.0f64;
    let mut denominator=0.0f64;
    for &(distance,index) in selected {
        let weight=closest/distance;
        numerator+=weight*targets[index];
        denominator+=weight;
    }
    let result=numerator/denominator;
    if !result.is_finite(){return Err("nonfinite prediction");}
    Ok(result)
}

#[cfg(test)]
mod tests{
    use super::weighted_knn_regression as predict;
    fn near(x:&[f64],y:&[f64],q:&[f64],n:usize,d:usize,k:usize,want:f64){
        let result=predict(x,y,q,n,d,k).unwrap();
        assert!((result-want).abs()<=1e-11f64*want.abs().max(1.0));
    }
    #[test]
    fn examples(){
        near(&[0.,4.],&[0.,20.],&[1.],2,1,2,5.);
        near(&[0.,2.,4.],&[0.,10.,20.],&[1.],3,1,2,5.);
        near(&[0.,2.,4.],&[0.,10.,20.],&[1.],3,1,3,50./7.);
        near(&[0.,0.,2.],&[3.,7.,20.],&[0.],3,1,3,5.);
        near(&[0.,0.,2.],&[3.,7.,20.],&[0.],3,1,1,3.);
        near(&[0.,0.,2.],&[3.,7.,20.],&[0.],3,1,2,5.);
        near(&[-1.,1.],&[2.,8.],&[0.],2,1,1,2.);
        near(&[-1.,1.],&[2.,8.],&[0.],2,1,2,5.);
        near(&[0.,3.,5.],&[-2.,10.,20.],&[1.],3,1,2,2.);
        near(&[4.],&[12.],&[-3.],1,1,1,12.);
        near(&[0.,0.,2.,0.,0.,2.],&[2.,4.,6.],&[1.,0.],3,2,2,3.);
        near(&[0.;2048],&[7.;128],&[0.;16],128,16,128,7.);
        near(&[0.,0.,1e-300],&[5.,15.,100.],&[0.],3,1,3,10.);
    }
    #[test]
    fn invalid(){
        assert!(predict(&[],&[],&[0.],0,1,1).is_err());
        assert!(predict(&[0.],&[1.],&[],1,1,1).is_err());
        assert!(predict(&[0.],&[1.],&[0.],1,1,0).is_err());
        assert!(predict(&[0.],&[1.],&[0.],1,1,2).is_err());
        assert!(predict(&[0.],&[1.],&[0.],1,17,1).is_err());
        assert!(predict(&[f64::NAN],&[1.],&[0.],1,1,1).is_err());
        assert!(predict(&[0.],&[f64::INFINITY],&[0.],1,1,1).is_err());
        assert!(predict(&[1001.],&[1.],&[0.],1,1,1).is_err());
        assert!(predict(&[0.],&[1001.],&[0.],1,1,1).is_err());
    }
}
