//! Online multifeature binary perceptron training.

/// Return a binary decision for score = weights dot features + bias.
pub fn perceptron_predict(weights: &[f64], features: &[f64],
                          bias: f64) -> Result<u8,&'static str> {
    if weights.is_empty() || weights.len()!=features.len() || !bias.is_finite()
       || !weights.iter().chain(features.iter()).all(|x|x.is_finite()) {
        return Err("invalid inference inputs");
    }
    let mut score=0.0_f64;
    for (&w,&x) in weights.iter().zip(features.iter()) {
        let term=w*x;
        if !term.is_finite() {return Err("linear product overflow");}
        score+=term;
        if !score.is_finite(){return Err("linear score overflow");}
    }
    score+=bias;
    if !score.is_finite(){return Err("linear score overflow");}
    Ok(if score>0.0 {1}else{0})
}

/// Train on row-major n*d features, aligned binary labels and fixed epochs.
pub fn perceptron(features: &[f64], labels: &[u8], d: usize,
                  learning_rate: f64, epochs: usize)
    -> Result<(Vec<f64>,f64),&'static str> {
    if d==0 || labels.is_empty() || epochs==0 ||
       labels.len().checked_mul(d).ok_or("dimension overflow")?!=features.len()
       || !learning_rate.is_finite() || learning_rate<=0.0 {
        return Err("invalid dimensions or hyperparameters");
    }
    if labels.iter().any(|&v|v>1) ||
       !features.iter().all(|v|v.is_finite()) {
        return Err("invalid labels or features");
    }
    let mut w=vec![0.0_f64;d];
    let mut bias=0.0_f64;
    for _ in 0..epochs {
        for (i,&label) in labels.iter().enumerate() {
            let row=&features[i*d..(i+1)*d];
            let pred=perceptron_predict(&w,row,bias)?;
            if pred==label {continue;}
            let delta=if label==1 {learning_rate} else {-learning_rate};
            for j in 0..d {
                let update=delta*row[j];
                if !update.is_finite(){return Err("weight product overflow");}
                w[j]+=update;
                if !w[j].is_finite(){return Err("weight update overflow");}
            }
            bias+=delta;
            if !bias.is_finite(){return Err("bias overflow");}
        }
    }
    Ok((w,bias))
}

#[cfg(test)]
mod tests {
    use super::{perceptron as train,perceptron_predict as predict};
    #[test]
    fn examples() {
        let data=[-2.0,-1.0,-1.0,-1.0,1.0,1.0,2.0,1.0];
        let (w,b)=train(&data,&[0,0,1,1],2,1.0,1).unwrap();
        assert_eq!((w.clone(),b),(vec![1.0,1.0],1.0));
        assert_eq!(predict(&w,&[-2.0,-1.0],b),Ok(0));
        assert_eq!(predict(&w,&[2.0,1.0],b),Ok(1));
        assert_eq!(train(&[0.0,0.0],&[1],2,1.0,1),Ok((vec![0.0,0.0],1.0)));
        assert_eq!(predict(&[0.0,0.0],&[0.0,0.0],0.0),Ok(0));
    }
    #[test]
    fn invalid() {
        assert!(train(&[],&[],1,1.0,1).is_err());
        assert!(train(&[1.0],&[0],0,1.0,1).is_err());
        assert!(train(&[1.0,1.0],&[0],1,1.0,1).is_err());
        assert!(train(&[1.0],&[2],1,1.0,1).is_err());
        assert!(train(&[f64::NAN],&[0],1,1.0,1).is_err());
        assert!(train(&[1.0],&[1],1,0.0,1).is_err());
        assert!(train(&[1.0],&[1],1,1.0,0).is_err());
        assert!(train(&[1e308],&[1],1,1e308,2).is_err());
        assert!(predict(&[1.0],&[f64::INFINITY],0.0).is_err());
    }
}
