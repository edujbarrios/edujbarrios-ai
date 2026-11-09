# Exact Euclidean Radius Search

## Why it belongs in AI

Radius-neighbor search retrieves every candidate inside a distance threshold rather than requiring a fixed number of nearest neighbors. It is a useful exact baseline for embedding retrieval, outlier detection, and neighborhood-based learning (Manning et al., 2008).

## Mathematical definition

For query vector `q`, candidate row `x[i]`, and radius `r >= 0`, compute:

```text
d[i] = sqrt(sum_j (q[j] - x[i,j])^2)
matches = all i satisfying d[i] <= r
return matches ordered by (d[i], i)
```

Use successive `hypot` operations to accumulate the Euclidean norm without unnecessarily overflowing intermediate squares. Exact ties use the **smaller original zero-based row index**, even for duplicate candidates. The radius comparison is inclusive, and an exact match is returned when radius is zero.

## Shared contract

- Query is a flat vector of dimension **1..32** and candidates form a nonempty **n×d** matrix, with **1..128** rows.
- All coordinates must be finite real numbers in **[-1000,1000]**; `radius` must be a finite real number in **[0,10000]**. Every row is validated, even if outside the radius.
- Result is an ascending-by-(distance,index) sequence of **zero-based original row indices**, potentially empty; duplicate rows count independently.
- C takes `const double *query`, `const double *candidates` (flat row-major n×d), `size_t n,d`, `double radius`, caller-owned `size_t indices[128]` and distinct `size_t *count_out`. On invalid data C returns `false` leaving both outputs unchanged. Input and output buffers must not overlap.
- Python accepts a list/tuple query and a list/tuple of list/tuple rows; returns `list[int]`, rejecting booleans, invalid shapes and nonfinite values. Rust uses flat row-major candidate input and returns `Result<Vec<usize>,&str>`. R accepts a numeric query vector and a numeric matrix (rows are candidates), returning an integer vector.
- This is **exact exhaustive search**, not a spatial index. No approximate nearest-neighbor acceleration is claimed.

## Complexity

**O(nd)** distance evaluation plus sorting at most n matches in **O(n log n)** time, with **O(n)** additional working memory. Python also validates/copies input rows.

## Examples

```text
query = [0,0]
candidates = [[1,0],[0,1],[3,4],[0,0],[-1,0]]
radius=1 -> [3,0,1,4]  # distances 0,1,1,1; ties by index
radius=0 -> [3]
radius=5 -> [3,0,1,4,2]
radius=0.5 on [[1,0]] -> []
```

## Implementations and testing

| Language | Directory |
| --- | --- |
| C | [euclidean_radius_search_c](euclidean_radius_search_c/) |
| Python | [euclidean_radius_search_python](euclidean_radius_search_python/) |
| Rust | [euclidean_radius_search_rust](euclidean_radius_search_rust/) |
| R | [euclidean_radius_search_r](euclidean_radius_search_r/) |

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` at the repository root. Tests cover inclusive thresholds, zero radius, ties, duplicates, no matches, rectangular shapes, maximum dimensions, invalid coordinates and invalid radius.

## Limitations

This approach scans all candidates and is not suited to very large vector databases. Distances depend on feature scaling and floating-point precision; no cosine/angular radius or learned metric is implemented.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to information retrieval*. Cambridge University Press. https://nlp.stanford.edu/IR-book/
