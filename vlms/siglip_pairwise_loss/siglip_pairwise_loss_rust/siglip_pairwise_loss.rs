//! SigLIP-style stable pairwise logistic loss for matched image/text embeddings.

fn softplus(x: f64) -> f64 {
    x.max(0.0) + (-x.abs()).exp().ln_1p()
}

/// Return mean N^2 pairwise sigmoid loss, diagonal positive and others negative.
pub fn siglip_pairwise_loss(images: &[f64], texts: &[f64],
                            n: usize, dim: usize, logit_scale: f64,
                            bias: f64) -> Result<f64, &'static str> {
    if !(2..=64).contains(&n) || !(1..=128).contains(&dim) ||
       images.len()!=n*dim || texts.len()!=n*dim ||
       !logit_scale.is_finite() || logit_scale<=0.0 || logit_scale>100.0 ||
       !bias.is_finite() || bias.abs()>20.0 ||
       images.iter().chain(texts.iter()).any(|v|
           !v.is_finite() || v.abs()>1000.0) {
        return Err("invalid shape, vectors or hyperparameters");
    }
    fn normalize(flat: &[f64], n: usize, dim: usize)
        -> Result<Vec<f64>, &'static str> {
        let mut values=vec![0.0;n*dim];
        for i in 0..n {
            let row=&flat[i*dim..(i+1)*dim];
            let norm=row.iter().map(|x|x*x).sum::<f64>().sqrt();
            if norm<=0.0 {return Err("zero embedding");}
            for d in 0..dim {values[i*dim+d]=row[d]/norm;}
        }
        Ok(values)
    }
    let visual=normalize(images,n,dim)?;
    let language=normalize(texts,n,dim)?;
    let mut total=0.0;
    for i in 0..n {
        for j in 0..n {
            let dot: f64=(0..dim).map(|d|
                visual[i*dim+d]*language[j*dim+d]).sum();
            let label=if i==j {1.0} else {-1.0};
            total+=softplus(-label*(logit_scale*dot+bias));
        }
    }
    Ok(total/(n*n) as f64)
}

#[cfg(test)]
mod tests {
    use super::siglip_pairwise_loss as loss;
    #[test]
    fn reference() {
        let id=[1.0,0.0,0.0,1.0];
        let swapped=[0.0,1.0,1.0,0.0];
        let expected=((-1.0f64).exp().ln_1p()+2.0f64.ln())/2.0;
        assert!((loss(&id,&id,2,2,1.0,0.0).unwrap()-expected).abs()<1e-12);
        assert!((loss(&[3.0,0.0,0.0,2.0],&id,2,2,1.0,0.0).unwrap()-expected).abs()<1e-12);
        assert!(loss(&id,&swapped,2,2,1.0,0.0).unwrap()>expected);
        assert!(loss(&id,&id,2,2,100.0,-10.0).unwrap().is_finite());
    }
    #[test]
    fn invalid() {
        let id=[1.0,0.0,0.0,1.0];
        assert!(loss(&id,&id,1,2,1.0,0.0).is_err());
        assert!(loss(&id,&id,65,2,1.0,0.0).is_err());
        assert!(loss(&id,&id,2,0,1.0,0.0).is_err());
        assert!(loss(&id,&id,2,129,1.0,0.0).is_err());
        assert!(loss(&id,&id,2,2,0.0,0.0).is_err());
        assert!(loss(&id,&id,2,2,101.0,0.0).is_err());
        assert!(loss(&id,&id,2,2,1.0,21.0).is_err());
        assert!(loss(&id,&[0.0,0.0,0.0,1.0],2,2,1.0,0.0).is_err());
        assert!(loss(&id,&[1.0,0.0,f64::INFINITY,1.0],2,2,1.0,0.0).is_err());
        assert!(loss(&id,&[1.0,0.0,1001.0,1.0],2,2,1.0,0.0).is_err());
        assert!(loss(&id,&[1.0,0.0,1.0],2,2,1.0,0.0).is_err());
    }
}
