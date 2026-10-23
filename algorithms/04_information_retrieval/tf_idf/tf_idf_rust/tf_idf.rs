//! Unsmoothed, natural-log term-frequency / inverse-document-frequency.

pub fn tf_idf(
    term_frequency: usize, n_documents: usize, document_frequency: usize,
) -> Result<f64, &'static str> {
    if n_documents == 0 || document_frequency == 0 ||
       document_frequency > n_documents {
        return Err("invalid corpus document counts");
    }
    if term_frequency == 0 || document_frequency == n_documents {
        return Ok(0.0);
    }
    let idf = (n_documents as f64 / document_frequency as f64).ln();
    let value = term_frequency as f64 * idf;
    if !idf.is_finite() || idf < 0.0 || !value.is_finite() || value < 0.0 {
        return Err("non-finite TF-IDF score");
    }
    Ok(value)
}

#[cfg(test)]
mod tests {
    use super::tf_idf;

    #[test]
    fn known_and_common_terms() {
        let expected = 3.0 * 10.0_f64.ln();
        assert!((tf_idf(3, 100, 10).unwrap() - expected).abs() < 1e-12);
        assert_eq!(tf_idf(0, 100, 10), Ok(0.0));
        assert_eq!(tf_idf(12, 100, 100), Ok(0.0));
    }

    #[test]
    fn invalid() {
        assert!(tf_idf(1, 0, 0).is_err());
        assert!(tf_idf(1, 100, 0).is_err());
        assert!(tf_idf(1, 100, 101).is_err());
    }
}
