//! HITS hubs and authorities from a dense weighted link graph.

/// Run 100 normalized HITS updates; return (authority scores, hub scores).
pub fn hits_hubs_authorities(weights:&[f64],n:usize)
    ->Result<(Vec<f64>,Vec<f64>),&'static str>{
    if !(1..=64).contains(&n) ||
       n.checked_mul(n).ok_or("shape overflow")?!=weights.len() ||
       weights.iter().any(|x|!x.is_finite() || !(0.0..=1.0).contains(x)){
        return Err("invalid link adjacency graph");
    }
    let mut hubs=vec![1.0/(n as f64).sqrt();n];
    let mut authorities=vec![0.0;n];
    for _ in 0..100 {
        let mut raw=vec![0.0_f64;n];
        for j in 0..n {
            for i in 0..n {raw[j]+=weights[i*n+j]*hubs[i];}
        }
        if raw.iter().any(|x|!x.is_finite()){return Err("authority update overflow");}
        let norm=raw.iter().fold(0.0_f64,|acc,&x|acc.hypot(x));
        if !norm.is_finite(){return Err("authority norm overflow");}
        for j in 0..n {authorities[j]=if norm>0.0 {raw[j]/norm} else {0.0};}
        raw.fill(0.0);
        for i in 0..n {
            for j in 0..n {raw[i]+=weights[i*n+j]*authorities[j];}
        }
        if raw.iter().any(|x|!x.is_finite()){return Err("hub update overflow");}
        let norm=raw.iter().fold(0.0_f64,|acc,&x|acc.hypot(x));
        if !norm.is_finite(){return Err("hub norm overflow");}
        for i in 0..n {hubs[i]=if norm>0.0 {raw[i]/norm} else {0.0};}
    }
    Ok((authorities,hubs))
}

#[cfg(test)]
mod tests {
    use super::hits_hubs_authorities as hits;
    #[test]
    fn star_cycle_and_zero() {
        let (a,h)=hits(&[0.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0],3).unwrap();
        assert_eq!(a[0],0.0);
        assert!((a[1]-1.0/2.0_f64.sqrt()).abs()<1e-12);
        assert!((a[2]-1.0/2.0_f64.sqrt()).abs()<1e-12);
        assert_eq!(h,vec![1.0,0.0,0.0]);
        let (a,h)=hits(&[0.0,1.0,0.0,0.0,0.0,1.0,1.0,0.0,0.0],3).unwrap();
        assert!(a.iter().chain(h.iter()).all(|v|(v-1.0/3.0_f64.sqrt()).abs()<1e-12));
        assert_eq!(hits(&[0.0;4],2),Ok((vec![0.0;2],vec![0.0;2])));
        assert_eq!(hits(&[1.0],1),Ok((vec![1.0],vec![1.0])));
    }
    #[test]
    fn invalid() {
        assert!(hits(&[],0).is_err());
        assert!(hits(&[0.0,1.0],2).is_err());
        assert!(hits(&[0.0,-1.0,0.0,0.0],2).is_err());
        assert!(hits(&[0.0,f64::NAN,0.0,0.0],2).is_err());
        assert!(hits(&[0.0,2.0,0.0,0.0],2).is_err());
    }
}
