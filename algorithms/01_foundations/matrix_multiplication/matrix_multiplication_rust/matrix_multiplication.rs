//! Dense row-major triple-loop matrix multiplication.

/// Multiply A(rows_a × shared) and B(shared × cols_b), returning row-major C.
pub fn matrix_multiplication(
    a: &[f64], rows_a: usize, shared: usize, b: &[f64], cols_b: usize,
) -> Result<Vec<f64>, &'static str> {
    if rows_a == 0 || shared == 0 || cols_b == 0 {
        return Err("matrix dimensions must be positive");
    }
    let a_len = rows_a.checked_mul(shared).ok_or("dimension overflow")?;
    let b_len = shared.checked_mul(cols_b).ok_or("dimension overflow")?;
    let out_len = rows_a.checked_mul(cols_b).ok_or("dimension overflow")?;
    if a.len() != a_len || b.len() != b_len {
        return Err("matrix data length does not match dimensions");
    }
    if !a.iter().chain(b.iter()).all(|x| x.is_finite()) {
        return Err("matrix entries must be finite");
    }
    let mut out = vec![0.0; out_len];
    for i in 0..rows_a {
        for j in 0..cols_b {
            let mut total: f64 = 0.0;
            for k in 0..shared {
                let product = a[i * shared + k] * b[k * cols_b + j];
                total += product;
                if !product.is_finite() || !total.is_finite() {
                    return Err("matrix multiplication overflow");
                }
            }
            out[i * cols_b + j] = total;
        }
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::matrix_multiplication as multiply;

    #[test]
    fn rectangle_and_scalar() {
        assert_eq!(multiply(&[1.,2.,3.,4.,5.,6.], 2, 3,
                            &[7.,8.,9.,10.,11.,12.], 2),
                   Ok(vec![58.,64.,139.,154.]));
        assert_eq!(multiply(&[-3.],1,1,&[2.],1), Ok(vec![-6.]));
    }

    #[test]
    fn invalid_dimensions_and_values() {
        assert!(multiply(&[],0,1,&[1.],1).is_err());
        assert!(multiply(&[1.],1,2,&[1.],1).is_err());
        assert!(multiply(&[f64::NAN],1,1,&[1.],1).is_err());
        assert!(multiply(&[1e308],1,1,&[1e308],1).is_err());
    }
}
