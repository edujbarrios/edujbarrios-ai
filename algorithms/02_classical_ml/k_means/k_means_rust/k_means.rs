//! Deterministic dense Lloyd K-means on row-major finite data.

/// Return (row-major centroids, zero-based labels), using first-k seeds.
pub fn k_means(
    points: &[f64], n: usize, d: usize, k: usize, max_iterations: usize
) -> Result<(Vec<f64>, Vec<usize>), &'static str> {
    if n == 0 || d == 0 || k == 0 || k > n || max_iterations == 0 {
        return Err("invalid dimensions or iteration limit");
    }
    let count = n.checked_mul(d).ok_or("dimension overflow")?;
    let center_count = k.checked_mul(d).ok_or("dimension overflow")?;
    if points.len() != count || !points.iter().all(|v| v.is_finite()) {
        return Err("expected finite row-major samples");
    }
    for i in 0..k {
        for j in 0..i {
            if points[i*d..(i+1)*d] == points[j*d..(j+1)*d] {
                return Err("first k samples must be distinct");
            }
        }
    }

    let mut centers = points[..center_count].to_vec();
    let mut labels = vec![usize::MAX; n];
    for _ in 0..max_iterations {
        let mut sums = vec![0.0_f64; center_count];
        let mut counts = vec![0_usize; k];
        let mut next_labels = Vec::with_capacity(n);

        for i in 0..n {
            let mut best_index = 0;
            let mut best_distance = f64::INFINITY;
            for cluster in 0..k {
                let mut distance = 0.0_f64;
                for col in 0..d {
                    let delta = points[i*d+col] - centers[cluster*d+col];
                    if !delta.is_finite() {
                        return Err("distance overflow");
                    }
                    distance = distance.hypot(delta);
                    if !distance.is_finite() {
                        return Err("distance overflow");
                    }
                }
                if distance < best_distance {
                    best_distance = distance;
                    best_index = cluster;
                }
            }
            next_labels.push(best_index);
            counts[best_index] += 1;
            for col in 0..d {
                let offset = best_index*d+col;
                sums[offset] += points[i*d+col];
                if !sums[offset].is_finite() {
                    return Err("centroid sum overflow");
                }
            }
        }

        for cluster in 0..k {
            if counts[cluster] > 0 {
                for col in 0..d {
                    let offset = cluster*d+col;
                    centers[offset] = sums[offset] / counts[cluster] as f64;
                    if !centers[offset].is_finite() {
                        return Err("centroid overflow");
                    }
                }
            }
        }
        let unchanged = labels == next_labels;
        labels = next_labels;
        if unchanged {
            break;
        }
    }
    Ok((centers, labels))
}

#[cfg(test)]
mod tests {
    use super::k_means;

    #[test]
    fn known_two_clusters() {
        let (centers, labels) = k_means(
            &[0.0,0.0,10.0,10.0,0.0,2.0,10.0,12.0],
            4, 2, 2, 10
        ).unwrap();
        assert_eq!(centers, vec![0.0,1.0,10.0,11.0]);
        assert_eq!(labels, vec![0,1,0,1]);
    }
    #[test]
    fn single_cluster() {
        let (centers, labels) = k_means(&[0.0,0.0,10.0,10.0],2,2,1,10).unwrap();
        assert_eq!(centers,vec![5.0,5.0]);
        assert_eq!(labels,vec![0,0]);
    }
    #[test]
    fn invalid_data() {
        assert!(k_means(&[],0,1,1,10).is_err());
        assert!(k_means(&[1.0,2.0],2,1,0,10).is_err());
        assert!(k_means(&[1.0,2.0],2,1,3,10).is_err());
        assert!(k_means(&[0.0,0.0,1.0],3,1,2,10).is_err());
        assert!(k_means(&[0.0,1.0],2,1,1,0).is_err());
        assert!(k_means(&[f64::NAN,1.0],2,1,1,10).is_err());
        assert!(k_means(&[1e308,1e308],2,1,1,10).is_err());
    }
}
