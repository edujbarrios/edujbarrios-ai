//! Matched-pair cosine-similarity image-text retrieval recall@K.

/// Return [image->text, text->image] recall fractions for diagonal matches.
pub fn image_text_recall_at_k(images: &[f64], texts: &[f64],
                              n: usize, dim: usize, k: usize)
    -> Result<[f64; 2], &'static str> {
    if !(2..=64).contains(&n) || !(1..=128).contains(&dim) ||
       k < 1 || k > n || images.len() != n*dim || texts.len() != n*dim ||
       images.iter().chain(texts.iter()).any(|x|
           !x.is_finite() || x.abs() > 1000.0) {
        return Err("invalid dimensions, embeddings, or k");
    }
    fn normalized(flat: &[f64], n: usize, d: usize)
        -> Result<Vec<f64>, &'static str> {
        let mut result = vec![0.0; n*d];
        for i in 0..n {
            let row = &flat[i*d..(i+1)*d];
            let norm = row.iter().map(|x|x*x).sum::<f64>().sqrt();
            if norm <= 0.0 { return Err("zero embedding vector"); }
            for j in 0..d { result[i*d+j] = row[j]/norm; }
        }
        Ok(result)
    }
    let visual = normalized(images,n,dim)?;
    let language = normalized(texts,n,dim)?;
    let mut scores = vec![0.0; n*n];
    for i in 0..n {
        for j in 0..n {
            scores[i*n+j] = (0..dim).map(|d|
                visual[i*dim+d]*language[j*dim+d]).sum();
        }
    }
    let mut image_hits = 0_usize;
    let mut text_hits = 0_usize;
    for i in 0..n {
        let correct = scores[i*n+i];
        let mut row_rank = 0;
        let mut col_rank = 0;
        for j in 0..n {
            let row = scores[i*n+j];
            let col = scores[j*n+i];
            if row > correct || (row == correct && j < i) { row_rank += 1; }
            if col > correct || (col == correct && j < i) { col_rank += 1; }
        }
        if row_rank < k { image_hits += 1; }
        if col_rank < k { text_hits += 1; }
    }
    Ok([image_hits as f64 / n as f64, text_hits as f64 / n as f64])
}

#[cfg(test)]
mod tests {
    use super::image_text_recall_at_k as recall;
    #[test]
    fn reference_cases() {
        let aligned=[1.0,0.0,0.0,1.0];
        let swapped=[0.0,1.0,1.0,0.0];
        let duplicates=[1.0,0.0,1.0,0.0];
        assert_eq!(recall(&aligned,&aligned,2,2,1),Ok([1.0,1.0]));
        assert_eq!(recall(&aligned,&swapped,2,2,1),Ok([0.0,0.0]));
        assert_eq!(recall(&aligned,&swapped,2,2,2),Ok([1.0,1.0]));
        assert_eq!(recall(&duplicates,&duplicates,2,2,1),Ok([0.5,0.5]));
        assert_eq!(recall(&[2.0,0.0,0.0,3.0],&aligned,2,2,1),Ok([1.0,1.0]));
    }
    #[test]
    fn invalid_cases() {
        let a=[1.0,0.0,0.0,1.0];
        assert!(recall(&a,&a,1,2,1).is_err());
        assert!(recall(&a,&a,2,0,1).is_err());
        assert!(recall(&a,&a,2,129,1).is_err());
        assert!(recall(&a,&a,2,2,0).is_err());
        assert!(recall(&a,&a,2,2,3).is_err());
        assert!(recall(&a,&[0.0,0.0,0.0,1.0],2,2,1).is_err());
        assert!(recall(&a,&[1.0,0.0,f64::INFINITY,1.0],2,2,1).is_err());
        assert!(recall(&a,&[1.0,0.0,1001.0,1.0],2,2,1).is_err());
        assert!(recall(&a,&[1.0,0.0,1.0],2,2,1).is_err());
    }
}
