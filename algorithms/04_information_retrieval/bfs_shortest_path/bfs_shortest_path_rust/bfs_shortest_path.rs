//! FIFO breadth-first shortest-hop distances for a directed unweighted graph.
use std::collections::VecDeque;

/// Return -1 for unreachable vertices; accept a row-major binary matrix.
pub fn bfs_shortest_path(weights: &[u8], n: usize, source: usize)
    -> Result<Vec<i32>, &'static str>
{
    if !(1..=128).contains(&n) || source >= n ||
       weights.len() != n*n || weights.iter().any(|&v| v > 1) {
        return Err("invalid graph or source");
    }
    let mut distances = vec![-1; n];
    let mut queue = VecDeque::new();
    distances[source] = 0;
    queue.push_back(source);
    while let Some(u) = queue.pop_front() {
        for v in 0..n {
            if weights[u*n+v] == 1 && distances[v] == -1 {
                distances[v] = distances[u] + 1;
                queue.push_back(v);
            }
        }
    }
    Ok(distances)
}

#[cfg(test)]
mod tests {
    use super::bfs_shortest_path as bfs;
    #[test]
    fn reference() {
        let graph = [
            0,1,1,0,0,0, 0,0,0,1,0,0, 0,0,0,1,0,0,
            0,0,0,0,1,0, 0,0,0,0,0,0, 0,0,0,0,0,0
        ];
        assert_eq!(bfs(&graph,6,0),Ok(vec![0,1,1,2,3,-1]));
        assert_eq!(bfs(&graph,6,5),Ok(vec![-1,-1,-1,-1,-1,0]));
        assert_eq!(bfs(&[1,1,1,0],2,0),Ok(vec![0,1]));
        assert_eq!(bfs(&[1],1,0),Ok(vec![0]));
        assert_eq!(bfs(&[0],1,0),Ok(vec![0]));
    }
    #[test]
    fn invalid() {
        assert!(bfs(&[],0,0).is_err());
        assert!(bfs(&[0,1],2,0).is_err());
        assert!(bfs(&[0],1,1).is_err());
        assert!(bfs(&[0,2,0,0],2,0).is_err());
        assert!(bfs(&[0,3,0,0],2,0).is_err());
        assert!(bfs(&[0],129,0).is_err());
    }
}
