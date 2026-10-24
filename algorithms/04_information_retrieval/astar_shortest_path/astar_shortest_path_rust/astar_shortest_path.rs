//! A* shortest path in a small weighted directed graph, consistency-checked.

/// Return shortest distance and zero-based path; (-1,empty) when unreachable.
pub fn astar_shortest_path(weights:&[f64],h:&[f64],n:usize,
                           source:usize,target:usize)
    ->Result<(f64,Vec<usize>),&'static str>{
    if !(1..=64).contains(&n) || source>=n || target>=n ||
       h.len()!=n || n.checked_mul(n).ok_or("shape overflow")?!=weights.len() ||
       weights.iter().chain(h.iter()).any(|w|!w.is_finite() || !(0.0..=1_000_000.0).contains(w)) ||
       h[target]!=0.0 {
        return Err("invalid graph, heuristic, source, or target");
    }
    for u in 0..n {
        for v in 0..n {
            let w=weights[u*n+v];
            if w>0.0 && h[u]>w+h[v]+1e-9 {
                return Err("inconsistent heuristic");
            }
        }
    }
    let mut g=vec![f64::INFINITY;n];
    let mut parent=vec![None;n];
    let mut closed=vec![false;n];
    g[source]=0.0;
    for _ in 0..n {
        let mut best=None;
        for i in 0..n {
            if closed[i] || !g[i].is_finite(){continue;}
            let priority=g[i]+h[i];
            if !priority.is_finite(){return Err("priority overflow");}
            if best.is_none_or(|(score,_): (f64,usize)|priority<score) {
                best=Some((priority,i));
            }
        }
        let current=match best {Some((_,i))=>i,None=>return Ok((-1.0,vec![]))};
        if current==target {
            let mut path=vec![target];
            let mut node=target;
            while node!=source {
                node=parent[node].ok_or("missing predecessor")?;
                if path.len()>=n {return Err("cyclic predecessor path");}
                path.push(node);
            }
            path.reverse();
            return Ok((g[target],path));
        }
        closed[current]=true;
        for v in 0..n {
            let w=weights[current*n+v];
            if w==0.0 || closed[v] {continue;}
            let tentative=g[current]+w;
            if !tentative.is_finite(){return Err("cost overflow");}
            if tentative<g[v] {
                g[v]=tentative;
                parent[v]=Some(current);
            }
        }
    }
    Ok((-1.0,vec![]))
}

#[cfg(test)]
mod tests {
    use super::astar_shortest_path as astar;
    #[test]
    fn paths_and_unreachable() {
        let graph=[0.0,2.0,5.0,0.0,0.0,0.0,1.0,10.0,
                   0.0,0.0,0.0,2.0,0.0,0.0,0.0,0.0];
        let h=[4.0,3.0,2.0,0.0];
        assert_eq!(astar(&graph,&h,4,0,3),Ok((5.0,vec![0,1,2,3])));
        assert_eq!(astar(&graph,&[0.0;4],4,0,3),Ok((5.0,vec![0,1,2,3])));
        assert_eq!(astar(&graph,&h,4,3,3),Ok((0.0,vec![3])));
        assert_eq!(astar(&[0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0],
                         &[0.0;3],3,0,2),Ok((-1.0,vec![])));
    }
    #[test]
    fn invalid() {
        let graph=[0.0,2.0,5.0,0.0,0.0,0.0,1.0,10.0,
                   0.0,0.0,0.0,2.0,0.0,0.0,0.0,0.0];
        assert!(astar(&graph,&[10.0,3.0,2.0,0.0],4,0,3).is_err());
        assert!(astar(&graph,&[4.0,3.0,2.0,0.0],4,0,2).is_err());
        assert!(astar(&graph,&[4.0,3.0,2.0,0.0],4,4,3).is_err());
        assert!(astar(&[0.0,-1.0,0.0,0.0],&[0.0;2],2,0,1).is_err());
        assert!(astar(&[],&[],0,0,0).is_err());
    }
}
