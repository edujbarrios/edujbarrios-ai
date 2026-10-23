//! Backpropagation of an affine layer followed by ReLU.

/// Returns (input gradients, row-major weight gradients, bias gradients).
/// ReLU's subgradient at zero is defined to be zero.
pub fn dense_relu_backprop(
    x: &[f64], weights: &[f64], bias: &[f64], upstream: &[f64]
) -> Result<(Vec<f64>,Vec<f64>,Vec<f64>),&'static str> {
    let d=x.len();
    let o=bias.len();
    if d==0 || o==0 || upstream.len()!=o ||
       o.checked_mul(d).ok_or("shape overflow")?!=weights.len() {
        return Err("shape mismatch");
    }
    if !x.iter().chain(weights.iter()).chain(bias.iter()).chain(upstream.iter())
          .all(|v|v.is_finite()) {
        return Err("inputs must be finite");
    }
    let mut dx=vec![0.0_f64;d];
    let mut dw=vec![0.0_f64;o*d];
    let mut db=vec![0.0_f64;o];
    for j in 0..o {
        let mut z=0.0_f64;
        for i in 0..d {
            let product=weights[j*d+i]*x[i];
            if !product.is_finite() {return Err("multiplication overflow");}
            z+=product;
            if !z.is_finite() {return Err("affine summation overflow");}
        }
        z+=bias[j];
        if !z.is_finite() {return Err("bias addition overflow");}
        let dz=if z>0.0 {upstream[j]}else{0.0};
        db[j]=dz;
        for i in 0..d {
            let w_gradient=dz*x[i];
            let term=weights[j*d+i]*dz;
            if !w_gradient.is_finite() || !term.is_finite() {
                return Err("gradient product overflow");
            }
            dw[j*d+i]=w_gradient;
            dx[i]+=term;
            if !dx[i].is_finite() {return Err("input gradient overflow");}
        }
    }
    Ok((dx,dw,db))
}

#[cfg(test)]
mod tests {
    use super::dense_relu_backprop as backward;
    #[test]
    fn active_and_inactive_nodes() {
        assert_eq!(backward(&[2.0,-1.0],&[1.0,1.0,-1.0,2.0],&[0.0,1.0],&[3.0,4.0]),
                   Ok((vec![3.0,3.0],vec![6.0,-3.0,0.0,0.0],vec![3.0,0.0])));
    }
    #[test]
    fn zero_subgradient() {
        assert_eq!(backward(&[0.0],&[3.0],&[0.0],&[5.0]),
                   Ok((vec![0.0],vec![0.0],vec![0.0])));
    }
    #[test]
    fn invalid() {
        assert!(backward(&[],&[1.0],&[0.0],&[1.0]).is_err());
        assert!(backward(&[1.0],&[],&[],&[]).is_err());
        assert!(backward(&[1.0,2.0],&[1.0],&[0.0],&[1.0]).is_err());
        assert!(backward(&[f64::NAN],&[1.0],&[0.0],&[1.0]).is_err());
        assert!(backward(&[1e308],&[1e308],&[0.0],&[1.0]).is_err());
    }
}
