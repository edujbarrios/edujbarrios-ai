//! Dense deterministic Kahn topological sorting for a directed 0/1 graph.

/// Returns the minimum-index-first topological order or a validation/cycle error.
pub fn topological_sort(weights: &[u8], n: usize)
    -> Result<Vec<usize>, &'static str>
{
    if !(1..=128).contains(&n) || weights.len() != n*n ||
       weights.iter().any(|&edge| edge > 1) {
        return Err("invalid square 0/1 adjacency matrix");
    }
    let mut indegree = vec![0_usize; n];
    for u in 0..n {
        for v in 0..n {
            indegree[v] += weights[u*n+v] as usize;
        }
    }
    let mut used = vec![false; n];
    let mut output = Vec::with_capacity(n);
    for _ in 0..n {
        let u = (0..n).find(|&v| !used[v] && indegree[v] == 0)
            .ok_or("directed cycle detected")?;
        output.push(u);
        used[u] = true;
        for v in 0..n {
            indegree[v] -= weights[u*n+v] as usize;
        }
    }
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::topological_sort as sort;
    #[test]
    fn dag_examples() {
        let dag = [
            0,1,1,0,
            0,0,0,1,
            0,0,0,1,
            0,0,0,0
        ];
        assert_eq!(sort(&dag,4),Ok(vec![0,1,2,3]));
        assert_eq!(sort(&[0,0,0,0,0,0,0,0,0],3),Ok(vec![0,1,2]));
        assert_eq!(sort(&[0,0,0,0,0,0,1,0,0],3),Ok(vec![1,2,0]));
        assert_eq!(sort(&[0],1),Ok(vec![0]));
    }
    #[test]
    fn invalid_examples() {
        assert!(sort(&[],0).is_err());
        assert!(sort(&[0,1],2).is_err());
        assert!(sort(&[0,1,1,0],2).is_err());
        assert!(sort(&[1],1).is_err());
        assert!(sort(&[0,2,0,0],2).is_err());
        assert!(sort(&[0],129).is_err());
    }
}
