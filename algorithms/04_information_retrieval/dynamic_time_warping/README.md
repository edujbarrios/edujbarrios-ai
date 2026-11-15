# Dynamic Time Warping (DTW)

## Why it belongs in AI

Dynamic time warping matches sequences that differ in speed or local timing.
It is useful for time-series classification, speech recognition, gesture
matching, and sequence-based retrieval. The dynamic-programming recurrence
and optional diagonal search window follow the alignment approach described
by Sakoe and Chiba (1978).

## Algorithm

For sequences `x[0..n-1]` and `y[0..m-1]`, the local cost is
`|x[i] - y[j]|`. Define `D[0,0] = 0`, and the first row and
column (apart from the origin) as infinity:

```text
D[i,j] = |x[i-1] - y[j-1]|
         + min(D[i-1,j], D[i,j-1], D[i-1,j-1])
```

The answer is `D[n,m]`. By default, all alignment cells are
allowed. With `window=w`, only cells satisfying `|i-j| <= w`
are considered (Sakoe-Chiba band); require `w >= |n-m|` so
the endpoint remains reachable.

## Contract

- `dynamic_time_warping(a, b, window=None)` takes two list/tuple
  sequences of lengths `1..512` with finite numeric (non-boolean)
  elements of absolute value at most `1_000_000`.
- `window` is `None` for unrestricted alignment, or an integer
  from `abs(len(a)-len(b))` through `512`; booleans are rejected.
- Return a nonnegative finite binary64 total **absolute-difference**
  cost. No input is modified. Invalid shapes, values, or windows
  raise `ValueError`.
- Duplicate or repeated values are allowed. This version returns
  **distance only**, not the alignment path or a length-normalized cost.

## Worked examples

```python
dynamic_time_warping([1, 2, 3], [1, 2, 2, 3])   # 0.0
dynamic_time_warping([1, 3], [2, 4], window=0) # 2.0
```

An inserted repetition can be matched to the same observation without
paying additional local distance.

## Complexity

With unrestricted alignment, **O(nm)** time and **O(min(n,m))**
auxiliary memory using two rows. A band of width `w` reduces time
to **O(n(2w+1))** after orienting the longer series as rows. Memory
remains O(min(n,m)).

## Implementations

| Language | Folder |
| --- | --- |
| Python | [dynamic_time_warping_python](dynamic_time_warping_python/) |

Only the Python reference implementation is declared.

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests include repetitions, diagonal-only alignment, a brute-force
DP oracle, endpoint-window constraints, invalid data, and immutability.

## Limitations

Absolute-difference DTW is not invariant to arbitrary amplitude changes
and need not satisfy the triangle inequality. This implementation handles
scalar, not multivariate, signals; it has no global slope constraints,
fast approximate variants, path reconstruction, or normalization.
A small window can exclude a useful alignment.

## References

Sakoe, H., & Chiba, S. (1978). Dynamic programming algorithm optimization
for spoken word recognition. *IEEE Transactions on Acoustics, Speech,
and Signal Processing, 26*(1), 43–49.
https://doi.org/10.1109/TASSP.1978.1163055
