//! Warshall's all-pairs reflexive reachability of a directed binary graph.

/// Return row-major reachability including length-zero self paths.
pub fn transitive_closure(adjacency: &[u8], n: usize)
    -> Result<Vec<u8>, &'static str> {
    if !(1..=64).contains(&n) || adjacency.len()!=n*n ||
       adjacency.iter().any(|&x| x>1) {
        return Err("square binary graph of size 1..64 required");
    }
    let mut reach=adjacency.to_vec();
    for i in 0..n { reach[i*n+i]=1; }
    for k in 0..n {
        for i in 0..n {
            for j in 0..n {
                if reach[i*n+k]==1 && reach[k*n+j]==1 {
                    reach[i*n+j]=1;
                }
            }
        }
    }
    Ok(reach)
}

#[cfg(test)]
mod tests {
    use super::transitive_closure as closure;
    #[test]
    fn reference_cases() {
        let chain=[0,1,0,0,0,1,0,0,0];
        assert_eq!(closure(&chain,3),Ok(vec![1,1,1,0,1,1,0,0,1]));
        assert_eq!(chain,[0,1,0,0,0,1,0,0,0]);
        assert_eq!(closure(&[0,1,1,0],2),Ok(vec![1,1,1,1]));
        assert_eq!(closure(&[0,0,0,0],2),Ok(vec![1,0,0,1]));
        assert_eq!(closure(&[1],1),Ok(vec![1]));
        assert_eq!(closure(&[0],1),Ok(vec![1]));
    }
    #[test]
    fn invalid() {
        assert!(closure(&[],0).is_err());
        assert!(closure(&[0,1],2).is_err());
        assert!(closure(&[0,2,0,0],2).is_err());
        assert!(closure(&[0],65).is_err());
    }
}
