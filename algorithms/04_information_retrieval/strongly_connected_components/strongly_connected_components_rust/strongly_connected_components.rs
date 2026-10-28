//! Kosaraju SCC decomposition for a directed binary adjacency matrix.

fn finish_dfs(a: &[u8], n: usize, u: usize, seen: &mut [bool],
              order: &mut Vec<usize>) {
    seen[u] = true;
    for v in 0..n {
        if a[u*n+v] == 1 && !seen[v] { finish_dfs(a,n,v,seen,order); }
    }
    order.push(u);
}

fn reverse_dfs(a: &[u8], n: usize, u: usize, root: usize,
               component: &mut [usize]) {
    component[u] = root;
    for v in 0..n {
        if a[v*n+u] == 1 && component[v] == n {
            reverse_dfs(a,n,v,root,component);
        }
    }
}

/// Each output label is the minimum zero-based vertex ID in its SCC.
pub fn strongly_connected_components(adjacency: &[u8], n: usize)
    -> Result<Vec<usize>, &'static str> {
    if !(1..=128).contains(&n) || adjacency.len() != n*n ||
       adjacency.iter().any(|&x| x > 1) {
        return Err("invalid directed binary adjacency");
    }
    let mut seen = vec![false;n];
    let mut order = Vec::with_capacity(n);
    for u in 0..n {
        if !seen[u] { finish_dfs(adjacency,n,u,&mut seen,&mut order); }
    }
    let mut component = vec![n;n];
    for &u in order.iter().rev() {
        if component[u] == n { reverse_dfs(adjacency,n,u,u,&mut component); }
    }
    let mut minima = vec![n;n];
    for (u,&root) in component.iter().enumerate() {
        minima[root] = minima[root].min(u);
    }
    Ok(component.into_iter().map(|root| minima[root]).collect())
}

#[cfg(test)]
mod tests {
    use super::strongly_connected_components as scc;
    #[test]
    fn reference_cases() {
        let graph = [
            0,1,0,0,0,0, 0,0,1,0,0,0,
            1,0,0,1,0,0, 0,0,0,0,1,0,
            0,0,0,1,0,1, 0,0,0,0,0,0
        ];
        assert_eq!(scc(&graph,6),Ok(vec![0,0,0,3,3,5]));
        assert_eq!(scc(&[0,1,1,0],2),Ok(vec![0,0]));
        assert_eq!(scc(&[0,1,0,0],2),Ok(vec![0,1]));
        assert_eq!(scc(&[1],1),Ok(vec![0]));
        assert_eq!(scc(&[0],1),Ok(vec![0]));
        assert_eq!(scc(&[0;9],3),Ok(vec![0,1,2]));
    }
    #[test]
    fn invalid_cases() {
        assert!(scc(&[],0).is_err());
        assert!(scc(&[0,1],2).is_err());
        assert!(scc(&[0,2,0,0],2).is_err());
        assert!(scc(&[0;1],129).is_err());
    }
}
