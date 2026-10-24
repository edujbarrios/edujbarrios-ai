//! Deterministic DBSCAN over finite row-major Euclidean data.

/// Cluster n rows of width d; return -1 for noise and 0-based cluster IDs.
pub fn dbscan(points:&[f64],n:usize,d:usize,epsilon:f64,min_samples:usize)
    -> Result<Vec<i32>,&'static str> {
    if !(1..=1024).contains(&n) || !(1..=64).contains(&d) ||
       min_samples==0 || min_samples>n || !epsilon.is_finite() || epsilon<=0.0 ||
       n.checked_mul(d).ok_or("dimension overflow")?!=points.len() ||
       !points.iter().all(|v|v.is_finite()) {
        return Err("invalid dataset, radius, or minimum samples");
    }
    let mut adjacency=vec![false;n*n];
    let mut counts=vec![0_usize;n];
    for i in 0..n {
        for j in i..n {
            let mut distance=0.0_f64;
            for col in 0..d {
                let diff=points[i*d+col]-points[j*d+col];
                if !diff.is_finite(){return Err("difference overflow");}
                distance=distance.hypot(diff);
                if !distance.is_finite(){return Err("distance overflow");}
            }
            if distance<=epsilon {
                adjacency[i*n+j]=true;
                adjacency[j*n+i]=true;
                counts[i]+=1;
                if i!=j {counts[j]+=1;}
            }
        }
    }
    let mut visited=vec![false;n];
    let mut labels=vec![-1_i32;n];
    let mut cluster=0_i32;
    for i in 0..n {
        if visited[i] {continue;}
        visited[i]=true;
        if counts[i]<min_samples {continue;}
        labels[i]=cluster;
        let mut queued=vec![false;n];
        let mut queue=Vec::with_capacity(n);
        for j in 0..n {
            if j!=i && adjacency[i*n+j] {
                queue.push(j);queued[j]=true;
            }
        }
        let mut front=0;
        while front<queue.len() {
            let j=queue[front];
            front+=1;
            if !visited[j] {
                visited[j]=true;
                if counts[j]>=min_samples {
                    for h in 0..n {
                        if adjacency[j*n+h] && !visited[h] && !queued[h] {
                            queue.push(h);queued[h]=true;
                        }
                    }
                }
            }
            if labels[j]==-1 {labels[j]=cluster;}
        }
        cluster+=1;
    }
    Ok(labels)
}

#[cfg(test)]
mod tests {
    use super::dbscan;
    #[test]
    fn groups_and_border() {
        assert_eq!(dbscan(&[0.0,0.0,0.1,0.0,0.0,0.1,5.0,5.0,5.1,5.0,9.0,9.0],6,2,0.2,2),
                   Ok(vec![0,0,0,1,1,-1]));
        assert_eq!(dbscan(&[0.0,1.0,2.0,3.0],4,1,1.1,3),Ok(vec![0,0,0,0]));
        assert_eq!(dbscan(&[1.0,1.0,1.0],3,1,0.1,3),Ok(vec![0,0,0]));
        assert_eq!(dbscan(&[0.0,10.0,20.0],3,1,1.0,2),Ok(vec![-1,-1,-1]));
        assert_eq!(dbscan(&[0.0],1,1,1.0,1),Ok(vec![0]));
    }
    #[test]
    fn invalid() {
        assert!(dbscan(&[],0,1,1.0,1).is_err());
        assert!(dbscan(&[0.0],1,1,0.0,1).is_err());
        assert!(dbscan(&[0.0],1,1,1.0,0).is_err());
        assert!(dbscan(&[0.0],1,1,1.0,2).is_err());
        assert!(dbscan(&[0.0],1,2,1.0,1).is_err());
        assert!(dbscan(&[0.0,f64::NAN],2,1,1.0,1).is_err());
        assert!(dbscan(&[-1.7e308,1.7e308],2,1,1.0,1).is_err());
    }
}
