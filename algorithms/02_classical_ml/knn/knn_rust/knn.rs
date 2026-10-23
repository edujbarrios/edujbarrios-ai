//! Brute-force Euclidean nearest-neighbor indices with deterministic tie rules.

/// Sample-major, row-major data; returns zero-based indices sorted by (distance, index).
pub fn knn(
    samples: &[f64], n_samples: usize, dimensions: usize,
    query: &[f64], k: usize,
) -> Result<Vec<usize>, &'static str> {
    if n_samples == 0 || dimensions == 0 || k == 0 || k > n_samples {
        return Err("invalid dataset dimensions or k");
    }
    let expected = n_samples.checked_mul(dimensions).ok_or("dimension overflow")?;
    if samples.len() != expected || query.len() != dimensions {
        return Err("invalid data length");
    }
    if !query.iter().chain(samples.iter()).all(|v| v.is_finite()) {
        return Err("all components must be finite");
    }
    let mut ranked = Vec::with_capacity(n_samples);
    for i in 0..n_samples {
        let mut distance: f64 = 0.0;
        for j in 0..dimensions {
            let delta = samples[i * dimensions + j] - query[j];
            if !delta.is_finite() {
                return Err("distance overflow");
            }
            distance = distance.hypot(delta);
            if !distance.is_finite() {
                return Err("distance overflow");
            }
        }
        ranked.push((distance, i));
    }
    ranked.sort_by(|a,b| a.0.total_cmp(&b.0).then(a.1.cmp(&b.1)));
    Ok(ranked.iter().take(k).map(|&(_,i)| i).collect())
}

#[cfg(test)]
mod tests {
    use super::knn;

    #[test]
    fn ties_and_all_samples() {
        let x = [0.,0., 5.,5., 1.,1., 0.,2.];
        assert_eq!(knn(&x, 4, 2, &[0.,1.], 2), Ok(vec![0,3]));
        assert_eq!(knn(&x, 4, 2, &[0.,1.], 4), Ok(vec![0,3,2,1]));
    }

    #[test]
    fn invalid() {
        assert!(knn(&[],0,1,&[0.],1).is_err());
        assert!(knn(&[1.],1,1,&[0.],0).is_err());
        assert!(knn(&[1.],1,1,&[0.],2).is_err());
        assert!(knn(&[f64::NAN],1,1,&[0.],1).is_err());
        assert!(knn(&[1e308],1,1,&[-1e308],1).is_err());
        assert!(knn(&[0.,1.,2.],2,2,&[0.,0.],1).is_err());
    }
}
