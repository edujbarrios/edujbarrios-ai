//! Mean reciprocal rank in both cosine image/text retrieval directions.

/// Returns [image_to_text_MRR,text_to_image_MRR].
pub fn image_text_mrr(images: &[f64], texts: &[f64], n: usize, dim: usize)
    -> Result<[f64;2], &'static str> {
    if !(2..=64).contains(&n) || !(1..=128).contains(&dim) ||
       images.len()!=n*dim || texts.len()!=n*dim ||
       images.iter().chain(texts.iter()).any(|x|!x.is_finite() || x.abs()>1000.0) {
        return Err("invalid dimensions or embeddings");
    }
    fn normalized(flat:&[f64],n:usize,dim:usize)
        -> Result<Vec<f64>, &'static str> {
        let mut values=vec![0.0;n*dim];
        for i in 0..n {
            let row=&flat[i*dim..(i+1)*dim];
            let norm=row.iter().map(|x|x*x).sum::<f64>().sqrt();
            if norm<=0.0 {return Err("zero embedding");}
            for d in 0..dim {values[i*dim+d]=row[d]/norm;}
        }
        Ok(values)
    }
    let visual=normalized(images,n,dim)?;
    let language=normalized(texts,n,dim)?;
    let mut scores=vec![0.0;n*n];
    for i in 0..n {
        for j in 0..n {
            scores[i*n+j]=(0..dim).map(|d|
                visual[i*dim+d]*language[j*dim+d]).sum();
        }
    }
    let mut image_total=0.0;
    let mut text_total=0.0;
    for i in 0..n {
        let diagonal=scores[i*n+i];
        let mut row_rank=1_usize;
        let mut col_rank=1_usize;
        for j in 0..n {
            let row=scores[i*n+j];
            let col=scores[j*n+i];
            if row>diagonal || (row==diagonal && j<i) {row_rank+=1;}
            if col>diagonal || (col==diagonal && j<i) {col_rank+=1;}
        }
        image_total+=1.0/row_rank as f64;
        text_total+=1.0/col_rank as f64;
    }
    Ok([image_total/n as f64,text_total/n as f64])
}

#[cfg(test)]
mod tests {
    use super::image_text_mrr as mrr;
    #[test]
    fn reference() {
        let id=[1.0,0.0,0.0,1.0];
        assert_eq!(mrr(&id,&id,2,2),Ok([1.0,1.0]));
        assert_eq!(mrr(&id,&[0.0,1.0,1.0,0.0],2,2),Ok([0.5,0.5]));
        assert_eq!(mrr(&[1.0,0.0,1.0,0.0],&[1.0,0.0,1.0,0.0],2,2),
                   Ok([0.75,0.75]));
        assert_eq!(mrr(&[2.0,0.0,0.0,3.0],&id,2,2),Ok([1.0,1.0]));
    }
    #[test]
    fn invalid() {
        let id=[1.0,0.0,0.0,1.0];
        assert!(mrr(&id,&id,1,2).is_err());
        assert!(mrr(&id,&id,65,2).is_err());
        assert!(mrr(&id,&id,2,0).is_err());
        assert!(mrr(&id,&id,2,129).is_err());
        assert!(mrr(&id,&[0.0,0.0,0.0,1.0],2,2).is_err());
        assert!(mrr(&id,&[1.0,f64::NAN,0.0,1.0],2,2).is_err());
        assert!(mrr(&id,&[1.0,1001.0,0.0,1.0],2,2).is_err());
        assert!(mrr(&id,&[1.0,0.0,1.0],2,2).is_err());
    }
}
