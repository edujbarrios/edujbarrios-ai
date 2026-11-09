//! Tarjan DFS low-link bridges in undirected simple graphs.

struct BridgeState<'a> {
    a:&'a [u8], n:usize,
    disc:Vec<usize>, low:Vec<usize>, parent:Vec<usize>,
    marked:Vec<bool>, clock:usize,
}
impl BridgeState<'_> {
    fn visit(&mut self,u:usize) {
        self.clock+=1;
        self.disc[u]=self.clock;
        self.low[u]=self.clock;
        for v in 0..self.n {
            if self.a[u*self.n+v]==0 {continue;}
            if self.disc[v]==0 {
                self.parent[v]=u;
                self.visit(v);
                self.low[u]=self.low[u].min(self.low[v]);
                if self.low[v]>self.disc[u] {
                    let (lo,hi)=(u.min(v),u.max(v));
                    self.marked[lo*self.n+hi]=true;
                }
            } else if v!=self.parent[u] {
                self.low[u]=self.low[u].min(self.disc[v]);
            }
        }
    }
}

/// Return flattened, sorted zero-based endpoints for all undirected bridges.
pub fn undirected_graph_bridges(a:&[u8],n:usize)->Result<Vec<usize>,&'static str>{
    if !(1..=64).contains(&n) || a.len()!=n*n || a.iter().any(|&v|v>1){
        return Err("invalid adjacency size or binary values");
    }
    for u in 0..n {
        if a[u*n+u]!=0{return Err("self-loops unsupported");}
        for v in 0..u {
            if a[u*n+v]!=a[v*n+u]{return Err("matrix must be symmetric");}
        }
    }
    let mut state=BridgeState{
        a,n,disc:vec![0;n],low:vec![0;n],parent:vec![usize::MAX;n],
        marked:vec![false;n*n],clock:0
    };
    for u in 0..n {if state.disc[u]==0 {state.visit(u);}}
    let mut result=Vec::new();
    for u in 0..n {
        for v in u+1..n {
            if state.marked[u*n+v] {result.extend_from_slice(&[u,v]);}
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests{
    use super::undirected_graph_bridges as bridges;
    fn graph(n:usize,edges:&[(usize,usize)])->Vec<u8>{
        let mut a=vec![0;n*n];
        for &(u,v) in edges {a[u*n+v]=1;a[v*n+u]=1;}
        a
    }
    #[test]
    fn examples(){
        assert_eq!(bridges(&graph(4,&[(0,1),(1,2),(2,3)]),4),Ok(vec![0,1,1,2,2,3]));
        assert_eq!(bridges(&graph(4,&[(0,1),(1,2),(2,3),(3,0)]),4),Ok(vec![]));
        assert_eq!(bridges(&graph(4,&[(0,1),(0,2),(0,3)]),4),Ok(vec![0,1,0,2,0,3]));
        assert_eq!(bridges(&graph(5,&[(0,1),(1,2),(2,0),(2,3),(3,4)]),5),
                   Ok(vec![2,3,3,4]));
        assert_eq!(bridges(&graph(7,&[(0,1),(1,2),(3,4),(4,5),(5,3)]),7),
                   Ok(vec![0,1,1,2]));
        assert_eq!(bridges(&graph(4,&[(0,1),(0,2),(0,3),(1,2),(1,3),(2,3)]),4),Ok(vec![]));
        assert_eq!(bridges(&graph(1,&[]),1),Ok(vec![]));
        assert_eq!(bridges(&graph(2,&[(0,1)]),2),Ok(vec![0,1]));
        assert_eq!(bridges(&graph(3,&[]),3),Ok(vec![]));
    }
    #[test]
    fn maximum(){
        let edges:Vec<(usize,usize)>=(0..63).map(|i|(i,i+1)).collect();
        let want:Vec<usize>=(0..63).flat_map(|i|[i,i+1]).collect();
        assert_eq!(bridges(&graph(64,&edges),64),Ok(want));
    }
    #[test]
    fn invalid(){
        assert!(bridges(&[],0).is_err());
        assert!(bridges(&[0],65).is_err());
        assert!(bridges(&[],1).is_err());
        assert!(bridges(&[1],1).is_err());
        assert!(bridges(&[0,1,0,0],2).is_err());
        assert!(bridges(&[0,2,2,0],2).is_err());
    }
}
