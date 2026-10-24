//! Sinusoidal Transformer position encoding, from scratch.

/// Construct row-major positions-by-width encoding, 0-based positions.
pub fn positional_encoding(length: usize, width: usize)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=4096).contains(&length) || !(1..=1024).contains(&width) {
        return Err("length must be 1..4096 and width 1..1024");
    }
    let count=length.checked_mul(width).ok_or("dimension overflow")?;
    let mut result=Vec::with_capacity(count);
    for p in 0..length {
        for j in 0..width {
            let pair=j/2;
            let exponent=2.0*pair as f64/width as f64;
            let angle=p as f64/10000.0_f64.powf(exponent);
            result.push(if j%2==0 {angle.sin()} else {angle.cos()});
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::positional_encoding as pe;
    #[test]
    fn reference_values() {
        let v=pe(2,4).unwrap();
        assert_eq!(&v[..4],&[0.0,1.0,0.0,1.0]);
        assert!((v[4]-1.0_f64.sin()).abs()<1e-12);
        assert!((v[5]-1.0_f64.cos()).abs()<1e-12);
        assert!((v[6]-0.01_f64.sin()).abs()<1e-12);
        assert!((v[7]-0.01_f64.cos()).abs()<1e-12);
        assert_eq!(pe(1,1),Ok(vec![0.0]));
        assert_eq!(&pe(2,3).unwrap()[..3],&[0.0,1.0,0.0]);
    }
    #[test]
    fn invalid() {
        assert!(pe(0,4).is_err());
        assert!(pe(1,0).is_err());
        assert!(pe(4097,1).is_err());
        assert!(pe(1,1025).is_err());
        assert!(pe(4,7).unwrap().iter().all(|x|x.is_finite() && *x>=-1.0 && *x<=1.0));
    }
}
