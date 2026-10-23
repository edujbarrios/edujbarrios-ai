//! Single-query scaled dot-product attention with stable softmax.

pub fn scaled_dot_product_attention(
    query: &[f64], keys: &[f64], values: &[f64],
    m: usize, v: usize
) -> Result<Vec<f64>, &'static str> {
    let d=query.len();
    if d==0 || m==0 || v==0 ||
       m.checked_mul(d).ok_or("key shape overflow")?!=keys.len() ||
       m.checked_mul(v).ok_or("value shape overflow")?!=values.len() {
        return Err("invalid attention dimensions");
    }
    if !query.iter().chain(keys.iter()).chain(values.iter()).all(|x|x.is_finite()) {
        return Err("non-finite input");
    }
    let scale=(d as f64).sqrt();
    let mut scores=Vec::with_capacity(m);
    let mut maximum=f64::NEG_INFINITY;
    for j in 0..m {
        let mut dot=0.0_f64;
        for i in 0..d {
            let product=query[i]*keys[j*d+i];
            if !product.is_finite(){return Err("product overflow");}
            dot+=product;
            if !dot.is_finite(){return Err("dot overflow");}
        }
        let score=dot/scale;
        if !score.is_finite(){return Err("score overflow");}
        maximum=maximum.max(score);
        scores.push(score);
    }
    let mut denominator=0.0_f64;
    for score in &mut scores {
        let delta=*score-maximum;
        if !delta.is_finite(){return Err("softmax shift overflow");}
        *score=delta.exp();
        denominator+=*score;
        if !denominator.is_finite(){return Err("softmax denominator overflow");}
    }
    let mut result=vec![0.0_f64;v];
    for j in 0..m {
        let weight=scores[j]/denominator;
        for k in 0..v {
            let term=weight*values[j*v+k];
            if !term.is_finite(){return Err("weighted product overflow");}
            result[k]+=term;
            if !result[k].is_finite(){return Err("weighted sum overflow");}
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::scaled_dot_product_attention as attn;
    #[test]
    fn known_and_equal() {
        let e=(1.0_f64/2.0_f64.sqrt()).exp();
        let weight=e/(e+1.0);
        let output=attn(&[1.0,0.0],&[1.0,0.0,0.0,1.0],&[10.0,0.0,0.0,20.0],2,2).unwrap();
        assert!((output[0]-10.0*weight).abs()<1e-12);
        assert!((output[1]-20.0*(1.0-weight)).abs()<1e-12);
        assert_eq!(attn(&[1.0,1.0],&[1.0,0.0,0.0,1.0],&[2.0,4.0,6.0,8.0],2,2),
                   Ok(vec![4.0,6.0]));
        assert_eq!(attn(&[1.0],&[1.0],&[7.0],1,1),Ok(vec![7.0]));
    }
    #[test]
    fn stable_softmax_and_invalid() {
        assert_eq!(attn(&[1000.0],&[-1000.0,1000.0],&[0.0,1.0],2,1),Ok(vec![1.0]));
        assert!(attn(&[],&[1.0],&[1.0],1,1).is_err());
        assert!(attn(&[1.0],&[],&[],0,1).is_err());
        assert!(attn(&[1.0],&[1.0],&[],1,1).is_err());
        assert!(attn(&[f64::NAN],&[1.0],&[1.0],1,1).is_err());
        assert!(attn(&[1e308],&[1e308],&[1.0],1,1).is_err());
    }
}
