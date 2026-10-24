//! Autoregressive single-head scaled attention for all sequence positions.

/// Queries/keys are row-major T x d; values row-major T x v.
/// Return flattened row-major T x v output.
pub fn causal_attention(
    queries:&[f64], keys:&[f64], values:&[f64],
    t:usize, d:usize, v:usize,
)->Result<Vec<f64>,&'static str>{
    if t==0 || d==0 || v==0 ||
       t.checked_mul(d).ok_or("query shape overflow")?!=queries.len() ||
       t.checked_mul(d).ok_or("key shape overflow")?!=keys.len() ||
       t.checked_mul(v).ok_or("value shape overflow")?!=values.len(){
        return Err("incompatible or empty attention matrices");
    }
    if !queries.iter().chain(keys.iter()).chain(values.iter())
         .all(|x|x.is_finite()) {
        return Err("non-finite matrix entries");
    }
    let scale=(d as f64).sqrt();
    let mut output=vec![0.0_f64;t*v];
    let mut scores=vec![0.0_f64;t];
    for row in 0..t {
        let mut maximum=f64::NEG_INFINITY;
        for j in 0..=row {
            let mut dot=0.0_f64;
            for i in 0..d {
                let term=queries[row*d+i]*keys[j*d+i];
                if !term.is_finite(){return Err("dot product overflow");}
                dot+=term;
                if !dot.is_finite(){return Err("dot accumulation overflow");}
            }
            scores[j]=dot/scale;
            if !scores[j].is_finite(){return Err("logit overflow");}
            maximum=maximum.max(scores[j]);
        }
        let mut denominator=0.0_f64;
        for item in scores.iter_mut().take(row+1) {
            let shifted=*item-maximum;
            if !shifted.is_finite(){return Err("softmax shift overflow");}
            *item=shifted.exp();
            denominator+=*item;
            if !denominator.is_finite(){return Err("softmax sum overflow");}
        }
        for j in 0..=row {
            let weight=scores[j]/denominator;
            for col in 0..v {
                let term=weight*values[j*v+col];
                if !term.is_finite(){return Err("weighted product overflow");}
                output[row*v+col]+=term;
                if !output[row*v+col].is_finite(){return Err("output overflow");}
            }
        }
    }
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::causal_attention as attention;
    #[test]
    fn uniform_prefixes_and_single() {
        let out=attention(&[0.0,0.0,0.0],&[1.0,2.0,3.0],
                          &[2.0,4.0,6.0,8.0,10.0,12.0],3,1,2).unwrap();
        assert_eq!(out,vec![2.0,4.0,4.0,6.0,6.0,8.0]);
        assert_eq!(attention(&[3.0],&[3.0],&[7.0],1,1,1),Ok(vec![7.0]));
    }
    #[test]
    fn stable_scores_and_invalid() {
        assert_eq!(attention(&[1000.0,1000.0],&[-1000.0,1000.0],
                             &[0.0,7.0],2,1,1),Ok(vec![0.0,7.0]));
        assert!(attention(&[],&[],&[],0,1,1).is_err());
        assert!(attention(&[1.0],&[],&[1.0],1,1,1).is_err());
        assert!(attention(&[f64::NAN],&[1.0],&[1.0],1,1,1).is_err());
        assert!(attention(&[1e308],&[1e308],&[1.0],1,1,1).is_err());
        assert!(attention(&[1.0],&[1.0],&[1.0],1,1,0).is_err());
    }
}
