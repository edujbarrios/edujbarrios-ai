# Random-Hyperplane Sign Hash

## Why it belongs in the AI hub

**Locality-sensitive hashing (LSH)** compresses vectors into signatures
whose collisions can help identify similar items. Random-hyperplane hashing,
closely related to SimHash, uses the signs of vector projections to produce
binary signatures that can be compared by Hamming distance (Charikar, 2002).
This algorithm computes an exact sign signature for **caller-provided**
hyperplanes. It does **not** sample normals, build an index, or perform
approximate-neighbor retrieval.

## Mathematical definition

For vector `x ∈ R^d` and `p` nonzero normal vectors `r_j ∈ R^d`:

```text
bit[j] = 1 if dot(x, r[j]) >= 0 else 0
hash = sum(bit[j] * 2^j, j = 0..p-1)
```

Bit 0 represents the **first** hyperplane (least significant bit).
For a projection equal to zero, the bit is **1**. All languages return the
same nonnegative integer signature.

Example:

```text
x = [1, 2]
planes = [[1, 0], [0, 1], [-1, -1]]
signs = [+, +, -]
hash = 0b011 = 3
```

A true LSH family samples hyperplane normals randomly, commonly from an
isotropic Gaussian. Deterministic normals here make tests reproducible;
the **randomness requirement and angular collision guarantees do not
apply** to arbitrarily selected planes.

## Shared contract

- Vector dimension `d >= 1`; number of planes `1 <= p <= 30`. Input is a
  finite, **nonzero** real vector. Every plane must be a finite nonzero
  vector of exactly `d` components.
- Reject zero vectors, zero normals, NaN, infinity, malformed dimensions,
  non-finite products or accumulated dot products.
- Return an unsigned-looking **nonnegative 30-bit integer**. Limiting the
  signature to 30 bits makes it representable by signed base-R integers
  and standard C/Rust integer types without additional dependencies.
- C takes a row-major `p × d` normals buffer and a caller-owned `uint32_t`
  output pointer; output is unchanged on any error. Rust also accepts
  flattened row-major planes and returns `Result<u32, ...>`.
  Python takes nested rows and raises `ValueError`; R takes a base
  numeric matrix and raises errors.
- A query may lie exactly on a hyperplane; its corresponding bit is 1.
  Use the same ordered normals for all signatures intended for comparison.

## Complexity

**O(p × d)** time and O(1) auxiliary space for C and Rust; Python copies
the input and planes for validation (O(pd) additional memory).
The output is a single 30-bit integer.

## Implementations

| Language | Folder |
| --- | --- |
| C | [random_hyperplane_hash_c](random_hyperplane_hash_c/) |
| Python | [random_hyperplane_hash_python](random_hyperplane_hash_python/) |
| Rust | [random_hyperplane_hash_rust](random_hyperplane_hash_rust/) |
| R | [random_hyperplane_hash_r](random_hyperplane_hash_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include known signatures, exact-plane ties, invalid/zero normals,
non-finite inputs, mismatched dimensions, maximum 30-plane signatures,
and dot-product overflow.

## Limitations

This reference does not generate random planes, estimate collision
probabilities, train embedding models, build hash buckets, compute Hamming
nearest neighbors, or compare recall and speed against exact search.
Zero-vector signatures are deliberately undefined. GPU implementation is
deferred until a device-tested batch hashing workload exists.

## References

Charikar, M. S. (2002). Similarity estimation techniques from rounding
algorithms. In *Proceedings of the Thirty-Fourth Annual ACM Symposium on
Theory of Computing* (pp. 380–388).
https://doi.org/10.1145/509907.509965
