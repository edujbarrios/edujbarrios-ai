//! Directed weighted single-source shortest paths with negative-cycle detection.

#[derive(Clone, Copy, Debug)]
pub struct Edge {
    pub from: usize,
    pub to: usize,
    pub weight: f64,
}

/// Return +infinity for unreachable nodes; error on reachable negative cycles.
pub fn bellman_ford_shortest_path(
    edges: &[Edge], n: usize, source: usize,
) -> Result<Vec<f64>, &'static str> {
    if !(1..=128).contains(&n) || source >= n || edges.len() > 16_384 ||
       edges.iter().any(|e| {
           e.from >= n || e.to >= n ||
           !e.weight.is_finite() || e.weight.abs() > 1_000_000.0
       }) {
        return Err("invalid graph, vertex or edge weight");
    }
    let mut distances = vec![f64::INFINITY; n];
    distances[source] = 0.0;
    for _ in 1..n {
        let mut changed = false;
        for e in edges {
            if !distances[e.from].is_finite() {
                continue;
            }
            let candidate = distances[e.from] + e.weight;
            if !candidate.is_finite() {
                return Err("distance overflow");
            }
            if candidate < distances[e.to] {
                distances[e.to] = candidate;
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
    for e in edges {
        if distances[e.from].is_finite() &&
           distances[e.from] + e.weight < distances[e.to] {
            return Err("reachable negative cycle");
        }
    }
    Ok(distances)
}

#[cfg(test)]
mod tests {
    use super::{bellman_ford_shortest_path as shortest, Edge};

    #[test]
    fn distances_and_disconnection() {
        let graph = [
            Edge { from: 0, to: 1, weight: 4.0 },
            Edge { from: 0, to: 2, weight: 5.0 },
            Edge { from: 1, to: 2, weight: -2.0 },
            Edge { from: 2, to: 3, weight: 3.0 },
        ];
        assert_eq!(shortest(&graph, 5, 0), Ok(vec![0.0, 4.0, 2.0, 5.0, f64::INFINITY]));
        assert_eq!(shortest(&[], 1, 0), Ok(vec![0.0]));
        let remote = [
            Edge { from: 2, to: 3, weight: -3.0 },
            Edge { from: 3, to: 2, weight: 1.0 },
        ];
        assert_eq!(shortest(&remote, 4, 0),
                   Ok(vec![0.0, f64::INFINITY, f64::INFINITY, f64::INFINITY]));
        let zero = [
            Edge { from: 0, to: 1, weight: 0.0 },
            Edge { from: 0, to: 1, weight: 5.0 },
            Edge { from: 1, to: 2, weight: -1.0 },
        ];
        assert_eq!(shortest(&zero, 3, 0), Ok(vec![0.0, 0.0, -1.0]));
    }

    #[test]
    fn negative_cycles_and_invalid_input() {
        let neg = [
            Edge { from: 0, to: 1, weight: 1.0 },
            Edge { from: 1, to: 2, weight: -3.0 },
            Edge { from: 2, to: 1, weight: 1.0 },
        ];
        assert!(shortest(&neg, 3, 0).is_err());
        assert!(shortest(&[Edge { from: 0, to: 0, weight: -1.0 }], 1, 0).is_err());
        assert!(shortest(&[], 0, 0).is_err());
        assert!(shortest(&[], 2, 2).is_err());
        assert!(shortest(&[Edge { from: 0, to: 3, weight: 1.0 }], 3, 0).is_err());
        assert!(shortest(&[Edge { from: 0, to: 1, weight: f64::NAN }], 2, 0).is_err());
        assert!(shortest(&[Edge { from: 0, to: 1, weight: 1_000_001.0 }], 2, 0).is_err());
        assert!(shortest(&vec![Edge { from: 0, to: 1, weight: 1.0 }; 16_385], 2, 0).is_err());
    }
}
