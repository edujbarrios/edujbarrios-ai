# MinHash Signature

## Why it belongs in AI

MinHash-style signatures compress sets of token identifiers into a small
array of hash minima. Comparing the fraction of equal signature positions
offers a lightweight approximation to set resemblance, useful for duplicate
detection, retrieval and candidate generation (Leskovec et al., 2020).

This educational implementation uses a **fixed supplied family of
modular linear hashes**, rather than a random hash generator. That makes
examples and tests precisely reproducible across C, Python, Rust and R.

## Algorithm

Let P = 65521 (a prime). For a token x and hash coefficients (a_j, b_j),

    h_j(x) = (a_j * x + b_j) mod P
    signature[j] = min_{x in tokens} h_j(x)

A separate pair of coefficients is used for each of m signature entries.
For an empty set of tokens, the signature contains P in every slot,
a sentinel greater than all possible hash values.

For tokens [1,2,3], a=[1,3], b=[0,7] the signature is [1,10].
Token order and duplicates do not change the result.

## Contract

- Inputs: a vector of zero or more integer token IDs in [0,65520];
  two equally long coefficient vectors a and b with length m >= 1.
  Every a_j is in [1,65520], every b_j in [0,65520].
- Output: a signature of m nonnegative integer values in [0,65521].
  Only an empty input uses the 65521 sentinel.
- Reject noninteger, out-of-range, missing, boolean (Python) or invalid
  coefficient values, and mismatched/empty coefficient vectors.
- Multiplication uses exact arithmetic with intermediates at most
  4,292,935,920; this fits u64 and is exact in R double precision.
  C accepts `uint32_t` arrays and writes to a caller-owned, nonoverlapping
  output array of m entries; failure leaves it unchanged.
- Hash coefficients must be supplied by the caller; no implicit seed.

## Complexity

O(n*m) time for n token IDs and m hashes; O(m) returned signature memory.
The algorithm is order invariant and duplicate invariant. The source
does not depend on third-party ML libraries.

## Implementations

| Language | Directory |
| --- | --- |
| C | [minhash_signature_c](minhash_signature_c/) |
| Python | [minhash_signature_python](minhash_signature_python/) |
| Rust | [minhash_signature_rust](minhash_signature_rust/) |
| R | [minhash_signature_r](minhash_signature_r/) |

Run:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

Simple linear hash functions modulo one prime are **not truly minwise
independent**. Their collision frequency is not guaranteed to equal
Jaccard similarity exactly; larger, better-designed hash families and
empirical recall checks are appropriate for production indexing.
The chosen prime limits accepted IDs to 16-bit-scale values.
There is no document tokenization, hash-family generation or
automatic error-bound estimation here. The empty-set sentinel must not
be confused with a computed hash value.

## References

Leskovec, J., Rajaraman, A., & Ullman, J. D. (2020).
*Mining of massive datasets* (3rd ed.). Cambridge University Press.
http://www.mmds.org/
