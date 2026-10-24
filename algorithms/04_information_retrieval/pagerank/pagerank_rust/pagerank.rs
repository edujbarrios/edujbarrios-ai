//! Dense directed PageRank with dangling redistribution and fixed power steps.

/// Return rank scores in node order after exactly 100 synchronous iterations.
pub fn pagerank(weights:&[f64],n:usize)->Result<Vec<f64>,&'static str>{
    if !(1..=64).contains(&n) ||
       n.checked_mul(n).ok_or("dimension overflow")?!=weights.len() ||
       weights.iter().any(|w| !w.is_finite() || !(0.0..=1_000_000.0).contains(w)) {
        return Err("invalid square weighted adjacency matrix");
    }
    let mut out=vec![0.0_f64;n];
    for i in 0..n {
        for j in 0..n {out[i]+=weights[i*n+j];}
        if !out[i].is_finite(){return Err("row sum overflow");}
    }
    let mut rank=vec![1.0/n as f64;n];
    for _ in 0..100 {
        let mut next=vec![0.15/n as f64;n];
        for i in 0..n {
            if out[i]==0.0 {
                let share=0.85*rank[i]/n as f64;
                if !share.is_finite(){return Err("dangling redistribution overflow");}
                for score in &mut next {*score+=share;}
            } else {
                let factor=0.85*rank[i]/out[i];
                if !factor.is_finite(){return Err("transition update overflow");}
                for j in 0..n {
                    let contribution=factor*weights[i*n+j];
                    if !contribution.is_finite(){return Err("transition contribution overflow");}
                    next[j]+=contribution;
                }
            }
        }
        if next.iter().any(|score|!score.is_finite() || *score<0.0){
            return Err("PageRank score overflow");
        }
        rank=next;
    }
    Ok(rank)
}

#[cfg(test)]
mod tests {
    use super::pagerank;
    #[test]
    fn fixtures() {
        let p=pagerank(&[0.0,1.0,0.0,0.0],2).unwrap();
        assert!((p[0]-1.0/2.85).abs()<1e-12);
        assert!((p[1]-(1.0-1.0/2.85)).abs()<1e-12);
        let cycle=pagerank(&[0.0,1.0,0.0,0.0,0.0,1.0,1.0,0.0,0.0],3).unwrap();
        assert!(cycle.iter().all(|v|(v-1.0/3.0).abs()<1e-12));
        assert_eq!(pagerank(&[0.0;4],2),Ok(vec![0.5,0.5]));
        assert!((pagerank(&[0.0],1).unwrap()[0]-1.0).abs()<1e-12);
        assert!((pagerank(&[0.0,2.0,1.0,0.0],2).unwrap().iter().sum::<f64>()-1.0).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        assert!(pagerank(&[],0).is_err());
        assert!(pagerank(&[0.0,1.0],2).is_err());
        assert!(pagerank(&[0.0,-1.0,0.0,0.0],2).is_err());
        assert!(pagerank(&[0.0,f64::NAN,0.0,0.0],2).is_err());
        assert!(pagerank(&[0.0,1000001.0,0.0,0.0],2).is_err());
    }
}
