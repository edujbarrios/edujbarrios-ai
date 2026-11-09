//! Edmonds-Karp directed integer maximum flow with BFS augmenting paths.
use std::collections::VecDeque;

/// Maximum source-to-sink flow on n=2..16 directed integer-capacity vertices.
pub fn edmonds_karp_max_flow(
    capacities: &[u16], n: usize, source: usize, sink: usize,
) -> Result<u32, &'static str> {
    if !(2..=16).contains(&n) || source>=n || sink>=n || source==sink ||
        capacities.len()!=n*n ||
        capacities.iter().any(|&x|x>1000) ||
        (0..n).any(|i|capacities[i*n+i]!=0) {
        return Err("invalid directed capacity graph, source or sink");
    }
    let mut residual:Vec<u32>=capacities.iter().map(|&v|u32::from(v)).collect();
    let mut flow=0u32;
    loop {
        let mut parent=vec![usize::MAX;n];
        parent[source]=source;
        let mut queue=VecDeque::from([source]);
        while let Some(u)=queue.pop_front() {
            if parent[sink]!=usize::MAX {break;}
            for v in 0..n {
                if parent[v]==usize::MAX && residual[u*n+v]>0 {
                    parent[v]=u;
                    queue.push_back(v);
                }
            }
        }
        if parent[sink]==usize::MAX {return Ok(flow);}
        let mut augment=1000u32;
        let mut v=sink;
        while v!=source {
            let u=parent[v];
            augment=augment.min(residual[u*n+v]);
            v=u;
        }
        v=sink;
        while v!=source {
            let u=parent[v];
            residual[u*n+v]-=augment;
            residual[v*n+u]+=augment;
            v=u;
        }
        flow+=augment;
    }
}
#[cfg(test)]
mod tests {
    use super::edmonds_karp_max_flow as max_flow;
    fn network(n:usize,edges:&[(usize,usize,u16)])->Vec<u16> {
        let mut a=vec![0;n*n];
        for &(u,v,c) in edges {a[u*n+v]=c;}
        a
    }
    #[test]
    fn examples() {
        let classic=network(6,&[
            (0,1,16),(0,2,13),(1,2,10),(2,1,4),
            (1,3,12),(2,4,14),(3,2,9),(4,3,7),
            (3,5,20),(4,5,4),
        ]);
        assert_eq!(max_flow(&classic,6,0,5),Ok(23));
        assert_eq!(max_flow(&network(2,&[(0,1,7)]),2,0,1),Ok(7));
        assert_eq!(max_flow(&network(2,&[(0,1,7)]),2,1,0),Ok(0));
        assert_eq!(max_flow(&network(3,&[(0,1,4),(1,2,2),(0,2,1)]),3,0,2),Ok(3));
        assert_eq!(max_flow(&network(3,&[(0,1,4),(1,0,3),(1,2,2),(0,2,1)]),3,0,2),Ok(3));
        assert_eq!(max_flow(&network(4,&[(0,1,6),(2,3,7)]),4,0,3),Ok(0));
        assert_eq!(max_flow(&network(3,&[]),3,0,2),Ok(0));
    }
    #[test]
    fn maximum() {
        let mut edges=Vec::new();
        for i in 1..15 {edges.push((0,i,1000));edges.push((i,15,1000));}
        assert_eq!(max_flow(&network(16,&edges),16,0,15),Ok(14000));
    }
    #[test]
    fn invalid() {
        assert!(max_flow(&[],1,0,0).is_err());
        assert!(max_flow(&[],17,0,1).is_err());
        assert!(max_flow(&[],2,0,1).is_err());
        assert!(max_flow(&[0,1,0,0],2,0,0).is_err());
        assert!(max_flow(&[0,1,0,0],2,0,2).is_err());
        assert!(max_flow(&[1,0,0,0],2,0,1).is_err());
        assert!(max_flow(&[0,1001,0,0],2,0,1).is_err());
    }
}
