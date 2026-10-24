//! Stable single-query scaled attention with an explicit binary key mask.

/// All inputs are finite and row-major; mask 1=allowed, 0=excluded.
pub fn masked_attention(
    query: &[f64], keys: &[f64], values: &[f64],
    mask: &[u8], value_dim: usize
) -> Result<Vec<f64>, &'static str> {
    let d=query.len();
    let m=mask.len();
    if d==0 || m==0 || value_dim==0 ||
       m.checked_mul(d).ok_or("key dimension overflow")?!=keys.len() ||
       m.checked_mul(value_dim).ok_or("value dimension overflow")?!=values.len() {
        return Err("invalid attention dimensions");
    }
    if !query.iter().chain(keys.iter()).chain(values.iter()).all(|x|x.is_finite()) ||
       mask.iter().any(|&v|v>1) || !mask.contains(&1) {
        return Err("invalid finite data or binary mask");
    }
    let scale=(d as f64).sqrt();
    let mut scores=vec![0.0_f64;m];
    let mut maximum=f64::NEG_INFINITY;
    for j in 0..m {
        if mask[j]==0 {continue;}
        let mut dot=0.0_f64;
        for i in 0..d {
            let product=query[i]*keys[j*d+i];
            if !product.is_finite() {return Err("dot product overflow");}
            dot+=product;
            if !dot.is_finite() {return Err("dot sum overflow");}
        }
        scores[j]=dot/scale;
        if !scores[j].is_finite(){return Err("score overflow");}
        maximum=maximum.max(scores[j]);
    }
    let mut denominator=0.0_f64;
    for j in 0..m {
        if mask[j]==0 {continue;}
        let delta=scores[j]-maximum;
        if !delta.is_finite(){return Err("shift overflow");}
        scores[j]=delta.exp();
        denominator+=scores[j];
        if !denominator.is_finite(){return Err("softmax denominator overflow");}
    }
    let mut output=vec![0.0_f64;value_dim];
    for j in 0..m {
        if mask[j]==0 {continue;}
        let probability=scores[j]/denominator;
        for i in 0..value_dim {
            let term=probability*values[j*value_dim+i];
            if !term.is_finite(){return Err("weighted product overflow");}
            output[i]+=term;
            if !output[i].is_finite(){return Err("output overflow");}
        }
    }
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::masked_attention as attention;
    #[test]
    fn subset_and_single() {
        let keys=[1.0,0.0,0.0,1.0,1.0,1.0];
        let values=[10.0,0.0,0.0,20.0,4.0,6.0];
        let result=attention(&[1.0,0.0],&keys,&values,&[1,0,1],2).unwrap();
        assert!((result[0]-7.0).abs()<1e-12);
        assert!((result[1]-3.0).abs()<1e-12);
        assert_eq!(attention(&[1.0,0.0],&keys,&values,&[0,1,0],2),
                   Ok(vec![0.0,20.0]));
    }
    #[test]
    fn invalid_and_excluded_overflow() {
        assert_eq!(attention(&[1e308],&[1e308,1.0],&[0.0,7.0],&[0,1],1),
                   Ok(vec![7.0]));
        assert!(attention(&[1e308],&[1e308,1.0],&[0.0,7.0],&[1,0],1).is_err());
        assert!(attention(&[],&[1.0],&[1.0],&[1],1).is_err());
        assert!(attention(&[1.0],&[1.0],&[1.0],&[0],1).is_err());
        assert!(attention(&[1.0],&[1.0],&[1.0],&[2],1).is_err());
        assert!(attention(&[1.0],&[1.0],&[f64::NAN],&[1],1).is_err());
        assert!(attention(&[1.0],&[1.0],&[1.0],&[1],0).is_err());
    }
}
