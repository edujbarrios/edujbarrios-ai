//! Tarjan DFS cut vertices in a simple undirected graph.

fn visit(
    u: usize, n: usize, a: &[u8], clock: &mut usize,
    disc: &mut [usize], low: &mut [usize], parent: &mut [usize], cut: &mut [bool],
) {
    *clock += 1;
    disc[u] = *clock;
    low[u] = *clock;
    let mut children = 0usize;
    for v in 0..n {
        if a[u*n+v] == 0 { continue; }
        if disc[v] == 0 {
            parent[v] = u;
            children += 1;
            visit(v,n,a,clock,disc,low,parent,cut);
            low[u] = low[u].min(low[v]);
            if parent[u] != usize::MAX && low[v] >= disc[u] { cut[u] = true; }
        } else if v != parent[u] {
            low[u] = low[u].min(disc[v]);
        }
    }
    if parent[u] == usize::MAX && children > 1 { cut[u] = true; }
}

/// Sorted zero-based articulation vertices in a validated row-major n×n 0/1 graph.
pub fn articulation_points(a: &[u8], n: usize) -> Result<Vec<usize>, &'static str> {
    if !(1..=64).contains(&n) || a.len() != n*n || a.iter().any(|&x| x>1) {
        return Err("invalid graph size or adjacency values");
    }
    for u in 0..n {
        if a[u*n+u] != 0 { return Err("self-loop is unsupported"); }
        for v in 0..u {
            if a[u*n+v] != a[v*n+u] { return Err("adjacency must be symmetric"); }
        }
    }
    let mut disc = vec![0usize;n];
    let mut low = vec![0usize;n];
    let mut parent = vec![usize::MAX;n];
    let mut cut = vec![false;n];
    let mut clock = 0usize;
    for u in 0..n {
        if disc[u]==0 {
            visit(u,n,a,&mut clock,&mut disc,&mut low,&mut parent,&mut cut);
        }
    }
    Ok((0..n).filter(|&u| cut[u]).collect())
}

#[cfg(test)]
mod tests {
    use super::articulation_points as cuts;
    fn graph(n:usize,edges:&[(usize,usize)]) -> Vec<u8> {
        let mut a=vec![0;n*n];
        for &(u,v) in edges {
            a[u*n+v]=1;a[v*n+u]=1;
        }
        a
    }
    #[test]
    fn examples() {
        assert_eq!(cuts(&graph(5,&[(0,1),(1,2),(2,3),(3,4)]),5),Ok(vec![1,2,3]));
        assert_eq!(cuts(&graph(4,&[(0,1),(1,2),(2,3),(3,0)]),4),Ok(vec![]));
        assert_eq!(cuts(&graph(5,&[(0,1),(0,2),(0,3),(0,4)]),5),Ok(vec![0]));
        assert_eq!(cuts(&graph(6,&[(0,1),(1,2),(3,4)]),6),Ok(vec![1]));
        assert_eq!(cuts(&graph(4,&[(0,1),(0,2),(0,3),(1,2),(1,3),(2,3)]),4),Ok(vec![]));
        assert_eq!(cuts(&graph(1,&[]),1),Ok(vec![]));
        assert_eq!(cuts(&graph(2,&[(0,1)]),2),Ok(vec![]));
        assert_eq!(cuts(&graph(3,&[]),3),Ok(vec![]));
    }
    #[test]
    fn maximum() {
        let edges: Vec<(usize,usize)>=(0..63).map(|i|(i,i+1)).collect();
        assert_eq!(cuts(&graph(64,&edges),64),Ok((1..63).collect()));
    }
    #[test]
    fn invalid() {
        assert!(cuts(&[],0).is_err());
        assert!(cuts(&[0],65).is_err());
        assert!(cuts(&[],1).is_err());
        assert!(cuts(&[0,1],1).is_err());
        assert!(cuts(&[1],1).is_err());
        assert!(cuts(&[0,1,0,0],2).is_err());
        assert!(cuts(&[0,2,2,0],2).is_err());
    }
}
