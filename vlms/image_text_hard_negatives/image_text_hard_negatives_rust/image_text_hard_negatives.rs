//! Bidirectional hard-negative index mining for paired image/text embeddings.

/// Return image->text IDs concatenated with text->image IDs (zero-based).
pub fn image_text_hard_negatives(images: &[f64], texts: &[f64],
                                 n: usize, dim: usize)
    -> Result<Vec<usize>, &'static str> {
    if !(2..=64).contains(&n) || !(1..=128).contains(&dim) ||
       images.len()!=n*dim || texts.len()!=n*dim ||
       images.iter().chain(texts.iter()).any(|x|!x.is_finite() || x.abs()>1000.0) {
        return Err("invalid pair count, dimension or embeddings");
    }
    fn normalize(flat: &[f64], n: usize, dim: usize)
        -> Result<Vec<f64>, &'static str> {
        let mut result=vec![0.0;n*dim];
        for i in 0..n {
            let row=&flat[i*dim..(i+1)*dim];
            let norm=row.iter().map(|x|x*x).sum::<f64>().sqrt();
            if norm<=0.0 {return Err("zero embedding");}
            for d in 0..dim {result[i*dim+d]=row[d]/norm;}
        }
        Ok(result)
    }
    let visual=normalize(images,n,dim)?;
    let language=normalize(texts,n,dim)?;
    let mut scores=vec![0.0;n*n];
    for i in 0..n {
        for j in 0..n {
            scores[i*n+j]=(0..dim).map(|d|
                visual[i*dim+d]*language[j*dim+d]).sum();
        }
    }
    let mut out=vec![0_usize;2*n];
    for i in 0..n {
        let mut row_best=None;
        let mut col_best=None;
        for j in 0..n {
            if j==i {continue;}
            if row_best.is_none_or(|best:usize|scores[i*n+j]>scores[i*n+best]) {
                row_best=Some(j);
            }
            if col_best.is_none_or(|best:usize|scores[j*n+i]>scores[best*n+i]) {
                col_best=Some(j);
            }
        }
        out[i]=row_best.ok_or("missing negative")?;
        out[n+i]=col_best.ok_or("missing negative")?;
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::image_text_hard_negatives as mine;
    #[test]
    fn reference() {
        let id=[1.0,0.0,0.0,1.0];
        let triple=[1.0,0.0,0.0,1.0,0.6,0.8];
        let same=[1.0,0.0,1.0,0.0,1.0,0.0];
        assert_eq!(mine(&id,&id,2,2),Ok(vec![1,0,1,0]));
        assert_eq!(mine(&same,&same,3,2),Ok(vec![1,0,0,1,0,0]));
        assert_eq!(mine(&triple,&triple,3,2),Ok(vec![2,2,1,2,2,1]));
        assert_eq!(mine(&[2.0,0.0,0.0,3.0],&id,2,2),Ok(vec![1,0,1,0]));
    }
    #[test]
    fn invalid() {
        let id=[1.0,0.0,0.0,1.0];
        assert!(mine(&id,&id,1,2).is_err());
        assert!(mine(&id,&id,65,2).is_err());
        assert!(mine(&id,&id,2,0).is_err());
        assert!(mine(&id,&id,2,129).is_err());
        assert!(mine(&id,&[1.0,0.0,0.0,0.0],2,2).is_err());
        assert!(mine(&id,&[1.0,0.0,f64::NAN,1.0],2,2).is_err());
        assert!(mine(&id,&[1.0,0.0,1001.0,1.0],2,2).is_err());
        assert!(mine(&id,&[1.0,0.0,1.0],2,2).is_err());
    }
}
