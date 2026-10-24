//! Kruskal's MST with union-by-size and path-compressed disjoint sets.

fn root(parent:&mut [usize], mut vertex:usize)->usize {
    while parent[vertex]!=vertex {
        parent[vertex]=parent[parent[vertex]];
        vertex=parent[vertex];
    }
    vertex
}

/// Return canonical undirected edges and total cost, rejecting disconnected graphs.
pub fn kruskal_mst(weights:&[f64],n:usize)
    ->Result<(Vec<(usize,usize)>,f64),&'static str>{
    if !(1..=64).contains(&n) ||
       n.checked_mul(n).ok_or("shape overflow")?!=weights.len() ||
       weights.iter().any(|&v|v.is_nan() || v==f64::NEG_INFINITY ||
          (v.is_finite() && !(0.0..=1_000_000.0).contains(&v))) {
        return Err("invalid adjacency matrix or edge weights");
    }
    let mut edges=Vec::new();
    for i in 0..n {
        if weights[i*n+i]!=0.0 {return Err("diagonal must be zero");}
        for j in i+1..n {
            let a=weights[i*n+j];
            if a!=weights[j*n+i] {return Err("asymmetric graph");}
            if a.is_finite(){edges.push((a,i,j));}
        }
    }
    edges.sort_by(|a,b| a.0.partial_cmp(&b.0).expect("finite weights")
       .then(a.1.cmp(&b.1)).then(a.2.cmp(&b.2)));
    let mut parent:Vec<usize>=(0..n).collect();
    let mut size=vec![1_usize;n];
    let mut selected=Vec::with_capacity(n-1);
    let mut total=0.0_f64;
    for (weight,i,j) in edges {
        let (mut a,mut b)=(root(&mut parent,i),root(&mut parent,j));
        if a==b {continue;}
        if size[a]<size[b] {std::mem::swap(&mut a,&mut b);}
        parent[b]=a;
        size[a]+=size[b];
        selected.push((i,j));
        total+=weight;
        if !total.is_finite(){return Err("MST cost overflow");}
        if selected.len()==n-1{break;}
    }
    if selected.len()!=n-1 {return Err("disconnected graph");}
    Ok((selected,total))
}

#[cfg(test)]
mod tests {
    use super::kruskal_mst as mst;
    #[test]
    fn examples() {
        let inf=f64::INFINITY;
        assert_eq!(mst(&[0.0,1.0,4.0,inf,1.0,0.0,2.0,5.0,4.0,2.0,0.0,3.0,inf,5.0,3.0,0.0],4),
                   Ok((vec![(0,1),(1,2),(2,3)],6.0)));
        assert_eq!(mst(&[0.0,1.0,1.0,1.0,0.0,1.0,1.0,1.0,0.0],3),
                   Ok((vec![(0,1),(0,2)],2.0)));
        assert_eq!(mst(&[0.0,0.0,2.0,0.0,0.0,1.0,2.0,1.0,0.0],3),
                   Ok((vec![(0,1),(1,2)],1.0)));
        assert_eq!(mst(&[0.0],1),Ok((vec![],0.0)));
    }
    #[test]
    fn invalid() {
        let inf=f64::INFINITY;
        assert!(mst(&[],0).is_err());
        assert!(mst(&[0.0,1.0],2).is_err());
        assert!(mst(&[0.0,inf,inf,0.0],2).is_err());
        assert!(mst(&[0.0,1.0,2.0,0.0],2).is_err());
        assert!(mst(&[0.0,-1.0,-1.0,0.0],2).is_err());
        assert!(mst(&[0.0,f64::NAN,f64::NAN,0.0],2).is_err());
        assert!(mst(&[1.0,1.0,1.0,0.0],2).is_err());
    }
}
