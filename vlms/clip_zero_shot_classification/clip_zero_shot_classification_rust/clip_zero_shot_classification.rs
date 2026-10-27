//! CLIP-style normalized image/text similarity and stable softmax.

/// Return class probabilities in input text embedding order.
pub fn clip_zero_shot_classification(image: &[f64], texts: &[f64],
                                     classes: usize, dim: usize,
                                     logit_scale: f64)
    -> Result<Vec<f64>, &'static str> {
    if !(2..=64).contains(&classes) || !(1..=128).contains(&dim) ||
       image.len() != dim || texts.len() != classes*dim ||
       !logit_scale.is_finite() ||
       !(logit_scale > 0.0 && logit_scale <= 100.0) ||
       image.iter().chain(texts.iter()).any(|v|
           !v.is_finite() || v.abs() > 1000.0) {
        return Err("invalid dimensions, embeddings or logit scale");
    }
    let visual_norm = image.iter().map(|x| x*x).sum::<f64>().sqrt();
    if visual_norm <= 0.0 {
        return Err("zero image embedding");
    }
    let mut logits = Vec::with_capacity(classes);
    for k in 0..classes {
        let row = &texts[k*dim..(k+1)*dim];
        let text_norm = row.iter().map(|x| x*x).sum::<f64>().sqrt();
        if text_norm <= 0.0 {
            return Err("zero text embedding");
        }
        let dot: f64 = image.iter().zip(row).map(|(a,b)|a*b).sum();
        logits.push(logit_scale * dot/(visual_norm*text_norm));
    }
    let maximum = logits.iter().copied().fold(f64::NEG_INFINITY,f64::max);
    let weights: Vec<f64> = logits.iter().map(|x|(x-maximum).exp()).collect();
    let total: f64 = weights.iter().sum();
    if !total.is_finite() || total <= 0.0 {
        return Err("nonfinite normalization");
    }
    Ok(weights.iter().map(|w|w/total).collect())
}

#[cfg(test)]
mod tests {
    use super::clip_zero_shot_classification as classify;
    fn close(x:f64,y:f64) {assert!((x-y).abs()<1e-12,"{x} != {y}");}
    #[test]
    fn reference() {
        let image=[1.0,0.0];
        let texts=[1.0,0.0,0.0,1.0];
        let expected=1.0f64.exp()/(1.0f64.exp()+1.0);
        close(classify(&image,&texts,2,2,1.0).unwrap()[0],expected);
        close(classify(&[0.0,1.0],&texts,2,2,1.0).unwrap()[1],expected);
        assert_eq!(classify(&image,&[1.0,0.0,1.0,0.0],2,2,1.0),
                   Ok(vec![0.5,0.5]));
        close(classify(&[2.0,0.0],&[3.0,0.0,0.0,5.0],2,2,1.0).unwrap()[0],expected);
        assert!(classify(&image,&texts,2,2,100.0).unwrap()[0]>0.99999);
    }
    #[test]
    fn invalid() {
        let image=[1.0,0.0];
        let texts=[1.0,0.0,0.0,1.0];
        assert!(classify(&image,&texts,1,2,1.0).is_err());
        assert!(classify(&image,&texts,2,0,1.0).is_err());
        assert!(classify(&image,&texts,2,129,1.0).is_err());
        assert!(classify(&image,&texts,2,2,0.0).is_err());
        assert!(classify(&image,&texts,2,2,f64::NAN).is_err());
        assert!(classify(&[0.0,0.0],&texts,2,2,1.0).is_err());
        assert!(classify(&image,&[0.0,0.0,0.0,1.0],2,2,1.0).is_err());
        assert!(classify(&image,&[1.0,0.0,1001.0,1.0],2,2,1.0).is_err());
        assert!(classify(&image,&[1.0,0.0,1.0],2,2,1.0).is_err());
    }
}
