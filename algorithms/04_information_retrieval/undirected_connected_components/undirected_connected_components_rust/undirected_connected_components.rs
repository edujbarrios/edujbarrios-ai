//! Breadth-first undirected components with minimum-vertex labels.
use std::collections::VecDeque;

/// Returns zero-based minimum-vertex component IDs; validates symmetry.
pub fn undirected_connected_components(adjacency: &[u8], n: usize)
    -> Result<Vec<usize>, &'static str>
{
    if !(1..=128).contains(&n) || adjacency.len()!=n*n ||
       adjacency.iter().any(|&x| x>1) {
        return Err("square binary adjacency of size 1..128 required");
    }
    for u in 0..n {
        for v in 0..u {
            if adjacency[u*n+v]!=adjacency[v*n+u] {
                return Err("undirected graph must be symmetric");
            }
        }
    }
    let mut labels=vec![n;n];
    for start in 0..n {
        if labels[start]!=n {continue;}
        labels[start]=start;
        let mut queue=VecDeque::new();
        queue.push_back(start);
        while let Some(u)=queue.pop_front() {
            for v in 0..n {
                if adjacency[u*n+v]==1 && labels[v]==n {
                    labels[v]=start;
                    queue.push_back(v);
                }
            }
        }
    }
    Ok(labels)
}
#[cfg(test)]
mod tests {
    use super::undirected_connected_components as cc;
    #[test] fn examples() {
        let graph=[
            0,0,1,0,0,0, 0,0,0,0,1,0,
            1,0,0,1,0,0, 0,0,1,0,0,0,
            0,1,0,0,0,0, 0,0,0,0,0,0,
        ];
        assert_eq!(cc(&graph,6),Ok(vec![0,1,0,0,1,5]));
        assert_eq!(cc(&[1,1,1,0],2),Ok(vec![0,0]));
        assert_eq!(cc(&[0,0,0,0],2),Ok(vec![0,1]));
        assert_eq!(cc(&[1],1),Ok(vec![0]));
    }
    #[test] fn invalid() {
        assert!(cc(&[],0).is_err());
        assert!(cc(&[0,1],2).is_err());
        assert!(cc(&[0,1,0,0],2).is_err());
        assert!(cc(&[0,2,2,0],2).is_err());
        assert!(cc(&[0],129).is_err());
    }
}
