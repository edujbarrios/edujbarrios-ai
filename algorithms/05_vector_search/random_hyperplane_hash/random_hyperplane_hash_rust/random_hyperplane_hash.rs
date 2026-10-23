//! Deterministic sign hash of projections onto supplied hyperplane normals.

/// Packs positive-or-zero projection signs into p low-order bits.
pub fn random_hyperplane_hash(
    vector: &[f64], normals: &[f64], p: usize
) -> Result<u32, &'static str> {
    let d = vector.len();
    if d == 0 || p == 0 || p > 30 ||
       p.checked_mul(d).ok_or("dimension overflow")? != normals.len() {
        return Err("invalid dimensions or plane count");
    }
    if !vector.iter().all(|v| v.is_finite()) ||
       !vector.iter().any(|&v| v != 0.0) {
        return Err("query vector must be finite and nonzero");
    }
    let mut signature = 0_u32;
    for j in 0..p {
        let mut nonzero_normal = false;
        let mut dot = 0.0_f64;
        for i in 0..d {
            let normal = normals[j*d+i];
            if !normal.is_finite() {return Err("non-finite normal");}
            nonzero_normal |= normal != 0.0;
            let product = normal * vector[i];
            if !product.is_finite() {return Err("product overflow");}
            dot += product;
            if !dot.is_finite() {return Err("dot overflow");}
        }
        if !nonzero_normal {return Err("zero normal");}
        if dot >= 0.0 {signature |= 1_u32 << j;}
    }
    Ok(signature)
}

#[cfg(test)]
mod tests {
    use super::random_hyperplane_hash as hash;
    #[test]
    fn known_signs() {
        let planes = [1.0,0.0,0.0,1.0,-1.0,-1.0];
        assert_eq!(hash(&[1.0,2.0],&planes,3),Ok(3));
        assert_eq!(hash(&[0.0,1.0],&planes,3),Ok(3));
        assert_eq!(hash(&[-1.0,-2.0],&planes,3),Ok(4));
        assert_eq!(hash(&[1.0],&[1.0;30],30),Ok((1<<30)-1));
    }
    #[test]
    fn invalid() {
        assert!(hash(&[],&[],1).is_err());
        assert!(hash(&[0.0],&[1.0],1).is_err());
        assert!(hash(&[1.0],&[0.0],1).is_err());
        assert!(hash(&[1.0],&[1.0],0).is_err());
        assert!(hash(&[1.0],&[1.0],31).is_err());
        assert!(hash(&[f64::NAN],&[1.0],1).is_err());
        assert!(hash(&[1e308],&[1e308],1).is_err());
    }
}
