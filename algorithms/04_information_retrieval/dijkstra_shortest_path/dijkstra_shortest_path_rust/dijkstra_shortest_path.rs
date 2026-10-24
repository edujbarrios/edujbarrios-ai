//! Dense Dijkstra shortest paths on nonnegative directed weighted graphs.

/// Return distance to each node from source; unreachable nodes use -1.0.
pub fn dijkstra_shortest_path(weights:&[f64],n:usize,source:usize)
    ->Result<Vec<f64>,&'static str> {
    if !(1..=128).contains(&n) || source>=n ||
       n.checked_mul(n).ok_or("dimension overflow")?!=weights.len() ||
       weights.iter().any(|w|!w.is_finite() || !(0.0..=1_000_000.0).contains(w)) {
        return Err("invalid adjacency, source, or finite edge weights");
    }
    let mut distance=vec![f64::INFINITY;n];
    let mut visited=vec![false;n];
    distance[source]=0.0;
    for _ in 0..n {
        let mut u=None;
        for i in 0..n {
            if !visited[i] && distance[i].is_finite() &&
               u.is_none_or(|j:usize|distance[i]<distance[j]) {u=Some(i);}
        }
        let Some(current)=u else {break;};
        visited[current]=true;
        for v in 0..n {
            let w=weights[current*n+v];
            if w==0.0 || visited[v] {continue;}
            let candidate=distance[current]+w;
            if !candidate.is_finite(){return Err("distance overflow");}
            if candidate<distance[v] {distance[v]=candidate;}
        }
    }
    for value in &mut distance {
        if !value.is_finite(){*value=-1.0;}
    }
    Ok(distance)
}

#[cfg(test)]
mod tests {
    use super::dijkstra_shortest_path as shortest;
    #[test]
    fn paths() {
        let w=[0.0,2.0,5.0,0.0,0.0,0.0,1.0,0.0,
               0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0];
        assert_eq!(shortest(&w,4,0),Ok(vec![0.0,2.0,3.0,-1.0]));
        assert_eq!(shortest(&w,4,3),Ok(vec![-1.0,-1.0,-1.0,0.0]));
        assert_eq!(shortest(&[0.0,1.0,1.0,0.0],2,0),Ok(vec![0.0,1.0]));
        assert_eq!(shortest(&[0.0],1,0),Ok(vec![0.0]));
    }
    #[test]
    fn invalid() {
        assert!(shortest(&[],0,0).is_err());
        assert!(shortest(&[0.0,1.0],2,0).is_err());
        assert!(shortest(&[0.0],1,1).is_err());
        assert!(shortest(&[0.0,-1.0,0.0,0.0],2,0).is_err());
        assert!(shortest(&[0.0,f64::NAN,0.0,0.0],2,0).is_err());
        assert!(shortest(&[0.0,1_000_001.0,0.0,0.0],2,0).is_err());
    }
}
