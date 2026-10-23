"""One-term BM25 contribution with explicit corpus statistics."""
import math

def bm25(n_documents: int, doc_frequency: int, term_frequency: int,
         document_length: int, avg_document_length: float,
         k1: float = 1.2, b: float = 0.75) -> float:
    """Return the finite BM25 contribution for one query term and document."""
    counts = (n_documents, doc_frequency, term_frequency, document_length)
    if any(isinstance(v, bool) or not isinstance(v, int) or v < 0 for v in counts):
        raise ValueError("counts must be nonnegative integers")
    if n_documents < 1 or doc_frequency > n_documents or term_frequency > document_length:
        raise ValueError("invalid document and term counts")

    parameters = (avg_document_length, k1, b)
    if any(isinstance(v, bool) or not isinstance(v, (float, int)) for v in parameters):
        raise ValueError("parameters must be finite real numbers")
    try:
        avg, saturation, length_weight = map(float, parameters)
    except OverflowError as exc:
        raise ValueError("parameters must be finite") from exc
    if not all(math.isfinite(v) for v in (avg, saturation, length_weight)):
        raise ValueError("parameters must be finite")
    if avg <= 0 or saturation <= 0 or not 0 <= length_weight <= 1:
        raise ValueError("invalid BM25 parameters")
    if term_frequency == 0:
        return 0.0

    # int-to-float conversion may overflow for unrealistically large integers.
    try:
        ratio = float(document_length) / avg
        tf = float(term_frequency)
        rare = float(n_documents - doc_frequency)
        df = float(doc_frequency)
    except OverflowError as exc:
        raise ValueError("count is not representable") from exc
    normalization = saturation * (1.0 - length_weight + length_weight * ratio)
    idf = math.log1p((rare + 0.5) / (df + 0.5))
    numerator = idf * tf * (saturation + 1.0)
    denominator = tf + normalization
    result = numerator / denominator
    if not all(math.isfinite(v) for v in
               (ratio, normalization, idf, numerator, denominator, result)):
        raise ValueError("BM25 score overflow")
    if denominator <= 0 or result < 0:
        raise ValueError("invalid BM25 score")
    return result
