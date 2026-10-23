"""Raw-count TF-IDF term weight using unsmoothed natural-log IDF."""
import math

def tf_idf(term_frequency: int, n_documents: int,
           document_frequency: int) -> float:
    """Return tf * ln(N/df) for valid nonnegative integer counts."""
    values = (term_frequency, n_documents, document_frequency)
    if any(isinstance(v, bool) or not isinstance(v, int) for v in values):
        raise ValueError("counts must be integers")
    if (term_frequency < 0 or n_documents < 1 or document_frequency < 1
            or document_frequency > n_documents):
        raise ValueError("invalid term or document frequencies")
    if term_frequency == 0 or document_frequency == n_documents:
        return 0.0
    try:
        n = float(n_documents)
        df = float(document_frequency)
        tf = float(term_frequency)
    except OverflowError as exc:
        raise ValueError("counts cannot be represented") from exc
    idf = math.log(n / df)
    result = tf * idf
    if not math.isfinite(result) or result < 0:
        raise ValueError("non-finite TF-IDF result")
    return result
