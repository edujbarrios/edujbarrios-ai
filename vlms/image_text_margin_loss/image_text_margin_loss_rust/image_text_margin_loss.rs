//! Bidirectional cosine max-margin loss with hardest in-batch negatives.

/// Mean hardest-negative hinge ranking loss in both retrieval directions.
pub fn image_text_margin_loss(images: &[f64], texts: &[f64],
                              n: usize, dim: usize, margin: f64)
    -> Result<f64, &'static str> {
    if !(2..=64).contains(&n) || !(1..=128).contains(&dim) ||
       images.len()!=n*dim || texts.len()!=n*dim ||
       !margin.is_finite() || !(0.0..=2.0).contains(&margin) ||
       images.iter().chain(texts.iter()).any(|x|
           !x.is_finite() || x.abs()>1000.0) {
        return Err("invalid embeddings, dimension, or margin");
    }
    fn normalize(flat: &[f64], n: usize, dim: usize)
        -> Result<Vec<f64>, &'static str> {
        let mut normalized=vec![0.0;n*dim];
        for i in 0..n {
            let row=&flat[i*dim..(i+1)*dim];
            let norm=row.iter().map(|x|x*x).sum::<f64>().sqrt();
            if norm<=0.0 {return Err("zero embedding row");}
            for d in 0..dim {normalized[i*dim+d]=row[d]/norm;}
        }
        Ok(normalized)
    }
    let visual=normalize(images,n,dim)?;
    let language=normalize(texts,n,dim)?;
    let mut scores=vec![0.0;n*n];
    for i in 0..n {
        for j in 0..n {
            scores[i*n+j]=(0..dim)
                .map(|d| visual[i*dim+d]*language[j*dim+d]).sum();
        }
    }
    let mut total=0.0;
    for i in 0..n {
        let positive=scores[i*n+i];
        let mut hardest_row=f64::NEG_INFINITY;
        let mut hardest_col=f64::NEG_INFINITY;
        for j in 0..n {
            if j==i {continue;}
            hardest_row=hardest_row.max(scores[i*n+j]);
            hardest_col=hardest_col.max(scores[j*n+i]);
        }
        total+=(margin-positive+hardest_row).max(0.0);
        total+=(margin-positive+hardest_col).max(0.0);
    }
    Ok(total/(2*n) as f64)
}

#[cfg(test)]
mod tests {
    use super::image_text_margin_loss as loss;
    fn close(x:f64,y:f64){assert!((x-y).abs()<1e-12,"{x} != {y}");}
    #[test]
    fn reference() {
        let a=[1.0,0.0,0.0,1.0];
        let swapped=[0.0,1.0,1.0,0.0];
        let same=[1.0,0.0,1.0,0.0];
        close(loss(&a,&a,2,2,0.25).unwrap(),0.0);
        close(loss(&a,&swapped,2,2,0.25).unwrap(),1.25);
        close(loss(&same,&same,2,2,0.25).unwrap(),0.25);
        close(loss(&[2.0,0.0,0.0,3.0],&a,2,2,0.25).unwrap(),0.0);
        close(loss(&a,&a,2,2,2.0).unwrap(),1.0);
    }
    #[test]
    fn invalid() {
        let a=[1.0,0.0,0.0,1.0];
        assert!(loss(&a,&a,1,2,0.25).is_err());
        assert!(loss(&a,&a,2,0,0.25).is_err());
        assert!(loss(&a,&a,2,129,0.25).is_err());
        assert!(loss(&a,&a,2,2,-1.0).is_err());
        assert!(loss(&a,&a,2,2,2.1).is_err());
        assert!(loss(&a,&a,2,2,f64::NAN).is_err());
        assert!(loss(&[0.0,0.0,0.0,1.0],&a,2,2,0.25).is_err());
        assert!(loss(&[1.0,0.0,f64::INFINITY,1.0],&a,2,2,0.25).is_err());
        assert!(loss(&[1.0,0.0,1001.0,1.0],&a,2,2,0.25).is_err());
        assert!(loss(&[1.0,0.0,1.0],&a,2,2,0.25).is_err());
    }
}
