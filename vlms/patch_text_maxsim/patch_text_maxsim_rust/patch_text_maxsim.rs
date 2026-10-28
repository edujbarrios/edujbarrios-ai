//! Late interaction between text tokens and image patches using cosine MaxSim.
pub fn patch_text_maxsim(patches: &[f64], n_patches: usize,
                         tokens: &[f64], n_tokens: usize, dim: usize)
    -> Result<(Vec<usize>,f64), &'static str> {
    if !(1..=32).contains(&n_patches) || !(1..=32).contains(&n_tokens) ||
       !(1..=64).contains(&dim) || patches.len()!=n_patches*dim ||
       tokens.len()!=n_tokens*dim ||
       patches.iter().chain(tokens.iter()).any(|v|!v.is_finite() || v.abs()>1000.0) {
        return Err("invalid embedding shapes or values");
    }
    fn normalize(x: &[f64], count: usize, dim: usize)
        -> Result<Vec<f64>, &'static str> {
        let mut out=vec![0.0;count*dim];
        for i in 0..count {
            let row=&x[i*dim..(i+1)*dim];
            let norm=row.iter().map(|x|x*x).sum::<f64>().sqrt();
            if norm<=0.0 {return Err("zero embedding");}
            for d in 0..dim {out[i*dim+d]=row[d]/norm;}
        }
        Ok(out)
    }
    let visual=normalize(patches,n_patches,dim)?;
    let language=normalize(tokens,n_tokens,dim)?;
    let mut ids=vec![0_usize;n_tokens];
    let mut total=0.0;
    for t in 0..n_tokens {
        let mut best=f64::NEG_INFINITY;
        let mut best_id=0;
        for p in 0..n_patches {
            let mut dot=0.0;
            for d in 0..dim {dot+=language[t*dim+d]*visual[p*dim+d];}
            if dot>best {best=dot;best_id=p;}
        }
        ids[t]=best_id;
        total+=best;
    }
    Ok((ids,total/n_tokens as f64))
}

#[cfg(test)]
mod tests {
    use super::patch_text_maxsim as maxsim;
    #[test]
    fn reference() {
        let p=[1.0,0.0,0.0,1.0];
        let t=[1.0,0.0,0.6,0.8];
        let (ids,score)=maxsim(&p,2,&t,2,2).unwrap();
        assert_eq!(ids,vec![0,1]);
        assert!((score-0.9).abs()<1e-12);
        let (ids,score)=maxsim(&[1.0,0.0,1.0,0.0],2,&[1.0,0.0],1,2).unwrap();
        assert_eq!(ids,vec![0]);
        assert!((score-1.0).abs()<1e-12);
        let (ids,score)=maxsim(&[0.0,1.0],1,&[0.0,-1.0],1,2).unwrap();
        assert_eq!(ids,vec![0]);
        assert!((score+1.0).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        let p=[1.0,0.0,0.0,1.0];
        let t=[1.0,0.0,0.6,0.8];
        assert!(maxsim(&p,0,&t,2,2).is_err());
        assert!(maxsim(&p,33,&t,2,2).is_err());
        assert!(maxsim(&p,2,&t,0,2).is_err());
        assert!(maxsim(&p,2,&t,2,65).is_err());
        assert!(maxsim(&[0.0,0.0,0.0,1.0],2,&t,2,2).is_err());
        assert!(maxsim(&[f64::NAN,0.0,0.0,1.0],2,&t,2,2).is_err());
        assert!(maxsim(&[1001.0,0.0,0.0,1.0],2,&t,2,2).is_err());
        assert!(maxsim(&p,2,&t[..3],2,2).is_err());
    }
}
