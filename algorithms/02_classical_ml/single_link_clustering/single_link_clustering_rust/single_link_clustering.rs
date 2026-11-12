//! Deterministic single-link agglomerative clustering of integer 2D points.

/// Return k contiguous cluster labels, or None for invalid dimensions/coordinates.
pub fn single_link_clustering(points: &[i32], n: usize, k: usize) -> Option<Vec<usize>> {
    if n == 0 || n > 32 || k == 0 || k > n || points.len() != 2*n ||
        points.iter().any(|x| !(-1000..=1000).contains(x)) {
        return None;
    }
    let mut groups: Vec<usize> = (0..n).collect();
    for _ in 0..(n-k) {
        let mut best = (i64::MAX, 0_usize, 0_usize);
        for i in 0..n {
            for j in (i+1)..n {
                if groups[i] == groups[j] {
                    continue;
                }
                let dx = i64::from(points[2*i]) - i64::from(points[2*j]);
                let dy = i64::from(points[2*i+1]) - i64::from(points[2*j+1]);
                let candidate = (dx*dx + dy*dy, i, j);
                if candidate < best {
                    best = candidate;
                }
            }
        }
        let old = groups[best.2];
        let new = groups[best.1];
        for group in &mut groups {
            if *group == old {
                *group = new;
            }
        }
    }
    let mut renumber = vec![usize::MAX; n];
    let mut next = 0;
    let mut result = Vec::with_capacity(n);
    for group in groups {
        if renumber[group] == usize::MAX {
            renumber[group] = next;
            next += 1;
        }
        result.push(renumber[group]);
    }
    Some(result)
}

#[cfg(test)]
mod tests {
    use super::single_link_clustering as cluster;

    #[test]
    fn fixtures() {
        let p = [0,0, 0,1, 10,10, 10,11];
        assert_eq!(cluster(&p,4,2), Some(vec![0,0,1,1]));
        assert_eq!(cluster(&p,4,3), Some(vec![0,0,1,2]));
        assert_eq!(cluster(&p,4,1), Some(vec![0,0,0,0]));
        assert_eq!(cluster(&p,4,4), Some(vec![0,1,2,3]));
        assert_eq!(cluster(&[0,0, 0,2, 2,0, 2,2],4,2), Some(vec![0,0,0,1]));
        assert_eq!(cluster(&[0,0, 0,0, 10,10],3,2), Some(vec![0,0,1]));
        assert_eq!(cluster(&[-1000,-1000, 1000,1000],2,1), Some(vec![0,0]));
        assert_eq!(cluster(&[0,0],1,1), Some(vec![0]));
        let mut big = Vec::new();
        for i in 0..32 { big.extend([i, 0]); }
        assert_eq!(cluster(&big,32,1), Some(vec![0;32]));
        assert_eq!(cluster(&big,32,32), Some((0..32).collect()));
    }

    #[test]
    fn invalid() {
        let p = [0,0, 0,1, 10,10, 10,11];
        assert!(cluster(&p,0,1).is_none());
        assert!(cluster(&p,33,2).is_none());
        assert!(cluster(&p,4,0).is_none());
        assert!(cluster(&p,4,5).is_none());
        assert!(cluster(&p[..7],4,2).is_none());
        assert!(cluster(&[0,0, 1001,0],2,1).is_none());
        assert!(cluster(&[0,0, -1001,0],2,1).is_none());
    }
}
