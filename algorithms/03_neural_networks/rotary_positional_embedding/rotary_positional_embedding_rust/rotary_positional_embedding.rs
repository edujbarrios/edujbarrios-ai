//! Adjacent-pair RoPE rotations for one query and one key vector.

/// Rotate query and key at positions in 0..=1_000_000, with even d 2..1024.
pub fn rotary_positional_embedding(
    query: &[f64],key: &[f64],query_position: usize,key_position: usize
) -> Result<(Vec<f64>,Vec<f64>),&'static str> {
    let d=query.len();
    if d<2 || d>1024 || d%2!=0 || key.len()!=d ||
       query_position>1_000_000 || key_position>1_000_000 {
        return Err("invalid dimensions or positions");
    }
    if !query.iter().chain(key.iter()).all(|x|x.is_finite()) {
        return Err("non-finite input");
    }
    let mut qr=Vec::with_capacity(d);
    let mut kr=Vec::with_capacity(d);
    for j in (0..d).step_by(2) {
        let frequency=10_000.0_f64.powf(j as f64/d as f64);
        for (vec,pos,out) in [
            (query,query_position,&mut qr),
            (key,key_position,&mut kr)
        ] {
            let angle=pos as f64/frequency;
            let (s,c)=angle.sin_cos();
            let a=vec[j]*c-vec[j+1]*s;
            let b=vec[j]*s+vec[j+1]*c;
            if !a.is_finite() || !b.is_finite() {
                return Err("rotary coordinate overflow");
            }
            out.extend([a,b]);
        }
    }
    Ok((qr,kr))
}

#[cfg(test)]
mod tests {
    use super::rotary_positional_embedding as rope;
    #[test]
    fn examples_and_dimensions() {
        let (q,k)=rope(&[1.0,0.0],&[0.0,1.0],1,0).unwrap();
        assert!((q[0]-1.0_f64.cos()).abs()<1e-12);
        assert!((q[1]-1.0_f64.sin()).abs()<1e-12);
        assert!(k[0].abs()<1e-12 && (k[1]-1.0).abs()<1e-12);
        let (q,k)=rope(&[3.0,4.0],&[5.0,6.0],0,0).unwrap();
        assert_eq!(q,vec![3.0,4.0]);
        assert_eq!(k,vec![5.0,6.0]);
        let (q,k)=rope(&[1.0,0.0,1.0,0.0],&[1.0,0.0,1.0,0.0],1,1).unwrap();
        assert!((q[2]-0.01_f64.cos()).abs()<1e-12);
        assert!((q[3]-0.01_f64.sin()).abs()<1e-12);
        assert_eq!(q,k);
    }
    #[test]
    fn invalid() {
        assert!(rope(&[1.0],&[0.0],1,0).is_err());
        assert!(rope(&[1.0,0.0,2.0],&[0.0,1.0,2.0],1,0).is_err());
        assert!(rope(&[1.0,0.0],&[0.0],1,0).is_err());
        assert!(rope(&[1.0,0.0],&[0.0,1.0],1_000_001,0).is_err());
        assert!(rope(&[f64::NAN,0.0],&[0.0,1.0],1,0).is_err());
        assert!(rope(&[1.7e308,1.7e308],&[0.0,1.0],1,0).is_err());
    }
}
