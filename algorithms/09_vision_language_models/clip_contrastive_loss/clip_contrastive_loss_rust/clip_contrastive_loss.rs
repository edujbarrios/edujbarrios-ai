//! Symmetric image-to-text and text-to-image contrastive cross-entropy.

/// Calculate the stable CLIP objective for flat row-major paired embeddings.
pub fn clip_contrastive_loss(images: &[f64], texts: &[f64],
                             n: usize, dim: usize, logit_scale: f64)
    -> Result<f64, &'static str> {
    if !(2..=64).contains(&n) || !(1..=128).contains(&dim) ||
       images.len() != n*dim || texts.len() != n*dim ||
       !logit_scale.is_finite() || !(0.0 < logit_scale && logit_scale <= 100.0) {
        return Err("invalid dimensions, lengths, or logit scale");
    }
    fn normalized(values: &[f64], n: usize, dim: usize)
        -> Result<Vec<f64>, &'static str> {
        if values.iter().any(|v| !v.is_finite() || v.abs() > 1000.0) {
            return Err("nonfinite or out-of-bounds embedding");
        }
        let mut result = vec![0.0; n*dim];
        for i in 0..n {
            let row = &values[i*dim..(i+1)*dim];
            let magnitude = row.iter().map(|x| x*x).sum::<f64>().sqrt();
            if magnitude <= 0.0 {
                return Err("zero embedding vector");
            }
            for j in 0..dim {
                result[i*dim+j] = row[j]/magnitude;
            }
        }
        Ok(result)
    }
    let visual = normalized(images,n,dim)?;
    let language = normalized(texts,n,dim)?;
    let mut logits = vec![0.0; n*n];
    for i in 0..n {
        for j in 0..n {
            let dot: f64 = (0..dim).map(|d|
                visual[i*dim+d]*language[j*dim+d]).sum();
            logits[i*n+j] = logit_scale*dot;
        }
    }
    let mut total = 0.0;
    for i in 0..n {
        for orientation in 0..2 {
            let score = |j:usize| -> f64 {
                if orientation == 0 {logits[i*n+j]} else {logits[j*n+i]}
            };
            let maximum = (0..n).map(score).fold(f64::NEG_INFINITY,f64::max);
            let sumexp: f64 = (0..n).map(|j|(score(j)-maximum).exp()).sum();
            total += maximum + sumexp.ln() - logits[i*n+i];
        }
    }
    let loss = total/(2*n) as f64;
    if !loss.is_finite() {
        return Err("nonfinite loss");
    }
    Ok(loss.max(0.0))
}

#[cfg(test)]
mod tests {
    use super::clip_contrastive_loss as clip;
    fn close(a:f64,b:f64) {assert!((a-b).abs()<1e-12,"{a} != {b}");}
    #[test]
    fn reference_cases() {
        let image=[1.,0.,0.,1.];
        close(clip(&image,&image,2,2,1.).unwrap(),(1.+(-1.0f64).exp()).ln());
        close(clip(&image,&[0.,1.,1.,0.],2,2,1.).unwrap(),(1.+1.0f64.exp()).ln());
        close(clip(&[1.,0.,1.,0.],&[1.,0.,1.,0.],2,2,1.).unwrap(),2.0f64.ln());
        assert!(clip(&image,&image,2,2,100.).unwrap()<1e-12);
        close(clip(&[2.,0.,0.,3.],&image,2,2,1.).unwrap(),(1.+(-1.0f64).exp()).ln());
    }
    #[test]
    fn invalid() {
        let image=[1.,0.,0.,1.];
        assert!(clip(&image,&image,1,2,1.).is_err());
        assert!(clip(&image,&image,2,0,1.).is_err());
        assert!(clip(&image,&image,2,129,1.).is_err());
        assert!(clip(&image,&image,2,2,0.).is_err());
        assert!(clip(&image,&image,2,2,101.).is_err());
        assert!(clip(&image,&[0.,0.,0.,1.],2,2,1.).is_err());
        assert!(clip(&image,&[1.,0.,f64::INFINITY,1.],2,2,1.).is_err());
        assert!(clip(&image,&[1.,0.,1001.,1.],2,2,1.).is_err());
        assert!(clip(&image,&[1.,0.,1.],2,2,1.).is_err());
    }
}
