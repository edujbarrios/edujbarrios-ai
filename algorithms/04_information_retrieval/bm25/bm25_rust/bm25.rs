//! BM25 contribution for one term and document.

/// Compute Robertson-style positive-IDF BM25 with explicit corpus statistics.
pub fn bm25(
    n_documents: usize, doc_frequency: usize, term_frequency: usize,
    document_length: usize, avg_document_length: f64,
    k1: f64, b: f64,
) -> Result<f64, &'static str> {
    if n_documents == 0 || doc_frequency > n_documents ||
       term_frequency > document_length ||
       !avg_document_length.is_finite() || avg_document_length <= 0.0 ||
       !k1.is_finite() || k1 <= 0.0 || !b.is_finite() ||
       !(0.0..=1.0).contains(&b) {
        return Err("invalid counts or BM25 parameters");
    }
    if term_frequency == 0 {
        return Ok(0.0);
    }

    let length_ratio = document_length as f64 / avg_document_length;
    let normalization = k1 * (1.0 - b + b * length_ratio);
    let idf = ((n_documents - doc_frequency) as f64 + 0.5)
        .div_euclid(1.0);
    // Compute log(1 + corpus rarity ratio), avoiding log on a negative value.
    let idf = (idf / (doc_frequency as f64 + 0.5)).ln_1p();
    let tf = term_frequency as f64;
    let numerator = idf * tf * (k1 + 1.0);
    let denominator = tf + normalization;
    let score = numerator / denominator;
    if [length_ratio, normalization, idf, numerator, denominator, score]
        .iter().any(|x| !x.is_finite()) ||
        denominator <= 0.0 || score < 0.0 {
        return Err("non-finite BM25 result");
    }
    Ok(score)
}

#[cfg(test)]
mod tests {
    use super::bm25;

    #[test]
    fn score_and_missing_term() {
        let expected = (90.5_f64 / 10.5_f64).ln_1p() * (3.0 * 2.2 / 4.2);
        let result = bm25(100, 10, 3, 100, 100.0, 1.2, 0.75).unwrap();
        assert!((result - expected).abs() < 1e-12);
        assert_eq!(bm25(100, 10, 0, 0, 100.0, 1.2, 0.75), Ok(0.0));
    }

    #[test]
    fn document_length_and_invalid_inputs() {
        assert!(bm25(100, 10, 3, 50, 100.0, 1.2, 0.75).unwrap() >
                bm25(100, 10, 3, 200, 100.0, 1.2, 0.75).unwrap());
        assert!(bm25(0, 0, 0, 0, 100.0, 1.2, 0.75).is_err());
        assert!(bm25(100, 101, 1, 10, 100.0, 1.2, 0.75).is_err());
        assert!(bm25(100, 10, 11, 10, 100.0, 1.2, 0.75).is_err());
        assert!(bm25(100, 10, 1, 10, 0.0, 1.2, 0.75).is_err());
        assert!(bm25(100, 10, 1, 10, 100.0, 1.2, f64::NAN).is_err());
    }
}
