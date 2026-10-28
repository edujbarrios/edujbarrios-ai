//! Single-query cross-modal scaled dot-product attention.
pub fn cross_modal_attention(query: &[f64], keys: &[f64], values: &[f64],
                             patches: usize, dim: usize)
    -> Result<(Vec<f64>,Vec<f64>), &'static str> {
    if !(1..=64).contains(&patches) || !(1..=64).contains(&dim) ||
       query.len()!=dim || keys.len()!=patches*dim || values.len()!=patches*dim ||
       query.iter().chain(keys.iter()).chain(values.iter())
           .any(|x|!x.is_finite() || x.abs()>1000.0) {
        return Err("invalid dimensions or multimodal embeddings");
    }
    let mut scores=vec![0.0;patches];
    for p in 0..patches {
        let mut dot=0.0;
        for d in 0..dim {dot+=query[d]*keys[p*dim+d];}
        scores[p]=dot/(dim as f64).sqrt();
    }
    let maximum=scores.iter().copied().fold(f64::NEG_INFINITY,f64::max);
    let mut weights:Vec<f64>=scores.iter().map(|s|(s-maximum).exp()).collect();
    let total:f64=weights.iter().sum();
    for x in &mut weights {*x/=total;}
    let mut context=vec![0.0;dim];
    for p in 0..patches {
        for d in 0..dim {context[d]+=weights[p]*values[p*dim+d];}
    }
    Ok((weights,context))
}

#[cfg(test)]
mod tests {
    use super::cross_modal_attention as attend;
    #[test]
    fn reference() {
        let q=[1.0,0.0];
        let k=[1.0,0.0,0.0,1.0];
        let v=[10.0,0.0,0.0,20.0];
        let (w,c)=attend(&q,&k,&v,2,2).unwrap();
        let a=(1.0/2.0f64.sqrt()).exp();
        let expected=a/(1.0+a);
        assert!((w[0]-expected).abs()<1e-12);
        assert!((w[1]-(1.0-expected)).abs()<1e-12);
        assert!((c[0]-10.0*expected).abs()<1e-12);
        assert!((c[1]-20.0*(1.0-expected)).abs()<1e-12);
        assert_eq!(attend(&[0.0,0.0],&k,&v,2,2),
                   Ok((vec![0.5,0.5],vec![5.0,10.0])));
        assert_eq!(attend(&q,&k[..2],&v[..2],1,2),
                   Ok((vec![1.0],vec![10.0,0.0])));
    }
    #[test]
    fn invalid() {
        let q=[1.0,0.0];
        let k=[1.0,0.0,0.0,1.0];
        let v=[10.0,0.0,0.0,20.0];
        assert!(attend(&q,&k,&v,0,2).is_err());
        assert!(attend(&q,&k,&v,65,2).is_err());
        assert!(attend(&q,&k,&v,2,0).is_err());
        assert!(attend(&[f64::NAN,0.0],&k,&v,2,2).is_err());
        assert!(attend(&q,&[1.0,1001.0,0.0,1.0],&v,2,2).is_err());
        assert!(attend(&q,&k,&v[..3],2,2).is_err());
    }
}
