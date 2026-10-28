//! BFS two-coloring for undirected binary graphs.
use std::collections::VecDeque;

/// Return bipartiteness or an error for malformed adjacency.
pub fn bipartite_graph_check(adjacency: &[u8], n: usize)
    -> Result<bool,&'static str> {
    if !(1..=128).contains(&n) || adjacency.len()!=n*n ||
       adjacency.iter().any(|&v|v>1) {
        return Err("binary square graph of size 1..128 required");
    }
    for u in 0..n {
        for v in 0..u {
            if adjacency[u*n+v]!=adjacency[v*n+u] {
                return Err("undirected matrix must be symmetric");
            }
        }
    }
    let mut color=vec![-1_i8;n];
    for start in 0..n {
        if color[start]!=-1 {continue;}
        let mut queue=VecDeque::new();
        color[start]=0;
        queue.push_back(start);
        while let Some(u)=queue.pop_front() {
            for v in 0..n {
                if adjacency[u*n+v]==0 {continue;}
                if color[v]==-1 {
                    color[v]=1-color[u];
                    queue.push_back(v);
                } else if color[v]==color[u] {
                    return Ok(false);
                }
            }
        }
    }
    Ok(true)
}

#[cfg(test)]
mod tests {
    use super::bipartite_graph_check as check;
    #[test]
    fn examples() {
        let path=[0,1,0,1,0,1,0,1,0];
        let triangle=[0,1,1,1,0,1,1,1,0];
        let four=[0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0];
        let remote=[0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,0];
        assert_eq!(check(&path,3),Ok(true));
        assert_eq!(check(&four,4),Ok(true));
        assert_eq!(check(&[0],1),Ok(true));
        assert_eq!(check(&triangle,3),Ok(false));
        assert_eq!(check(&remote,4),Ok(false));
        assert_eq!(check(&[1],1),Ok(false));
    }
    #[test]
    fn invalid() {
        assert!(check(&[],0).is_err());
        assert!(check(&[0,1],2).is_err());
        assert!(check(&[0,1,0,0],2).is_err());
        assert!(check(&[0,2,2,0],2).is_err());
        assert!(check(&[0],129).is_err());
    }
}
