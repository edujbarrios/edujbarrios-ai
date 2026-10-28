//! Prim's minimum spanning tree weight, with dense symmetric adjacency.

/// Compute total MST weight, rejecting disconnected or invalid graphs.
/// Row-major matrix; zero is absent edge (diagonal must be zero).
pub fn prim_mst(weights: &[f64], n: usize) -> Result<f64, &'static str> {
    if !(1..=128).contains(&n) || weights.len() != n * n {
        return Err("square graph of size 1..128 required");
    }
    for u in 0..n {
        if weights[u * n + u] != 0.0 {
            return Err("diagonal must be zero");
        }
        for v in 0..n {
            let w = weights[u * n + v];
            if !w.is_finite() || !(0.0..=1_000_000.0).contains(&w) ||
               w != weights[v * n + u] {
                return Err("finite symmetric nonnegative weights required");
            }
        }
    }
    let mut inside = vec![false; n];
    let mut best = vec![f64::INFINITY; n];
    best[0] = 0.0;
    let mut total = 0.0;
    for _ in 0..n {
        let mut candidate: Option<usize> = None;
        for v in 0..n {
            if !inside[v] && best[v].is_finite() &&
               candidate.is_none_or(|u| best[v] < best[u]) {
                candidate = Some(v);
            }
        }
        let u = candidate.ok_or("graph is disconnected")?;
        inside[u] = true;
        total += best[u];
        for v in 0..n {
            let w = weights[u * n + v];
            if !inside[v] && w > 0.0 && w < best[v] {
                best[v] = w;
            }
        }
    }
    Ok(total)
}

#[cfg(test)]
mod tests {
    use super::prim_mst;

    #[test]
    fn reference_cases() {
        let graph = [
            0.0,1.0,4.0,0.0,
            1.0,0.0,2.0,5.0,
            4.0,2.0,0.0,3.0,
            0.0,5.0,3.0,0.0,
        ];
        assert_eq!(prim_mst(&graph,4),Ok(6.0));
        assert_eq!(prim_mst(&[0.0,1.0,1.0,1.0,0.0,1.0,1.0,1.0,0.0],3),Ok(2.0));
        assert_eq!(prim_mst(&[0.0],1),Ok(0.0));
    }

    #[test]
    fn invalid() {
        assert!(prim_mst(&[],0).is_err());
        assert!(prim_mst(&[0.0,1.0],2).is_err());
        assert!(prim_mst(&[0.0,0.0,0.0,0.0],2).is_err());
        assert!(prim_mst(&[0.0,1.0,2.0,0.0],2).is_err());
        assert!(prim_mst(&[1.0,1.0,1.0,0.0],2).is_err());
        assert!(prim_mst(&[0.0,-1.0,-1.0,0.0],2).is_err());
        assert!(prim_mst(&[0.0,f64::NAN,f64::NAN,0.0],2).is_err());
        assert!(prim_mst(&[0.0,f64::INFINITY,f64::INFINITY,0.0],2).is_err());
        assert!(prim_mst(&[0.0,1_000_001.0,1_000_001.0,0.0],2).is_err());
    }
}
