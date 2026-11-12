//! Two-class Laplace-smoothed Bernoulli Naive Bayes for binary features.

/// Return predicted class and posterior P(class=1), or None for invalid inputs.
/// Samples are a flat n-by-d row-major binary matrix.
pub fn bernoulli_naive_bayes(
    samples: &[u8], labels: &[u8], n: usize, d: usize, query: &[u8],
) -> Option<(u8, f64)> {
    if !(2..=128).contains(&n) || !(1..=16).contains(&d) ||
        samples.len() != n * d || labels.len() != n || query.len() != d ||
        samples.iter().chain(labels).chain(query).any(|&x| x > 1) {
        return None;
    }
    let mut counts = [0_usize; 2];
    let mut ones = [[0_usize; 16]; 2];
    for i in 0..n {
        let c = labels[i] as usize;
        counts[c] += 1;
        for j in 0..d {
            ones[c][j] += samples[i * d + j] as usize;
        }
    }
    if counts.contains(&0) {
        return None;
    }
    let mut scores = [0.0_f64; 2];
    for c in 0..2 {
        scores[c] = (counts[c] as f64 / n as f64).ln();
        for j in 0..d {
            let p = (ones[c][j] as f64 + 1.0) / (counts[c] as f64 + 2.0);
            scores[c] += if query[j] == 1 { p.ln() } else { (-p).ln_1p() };
        }
    }
    let delta = scores[0] - scores[1];
    let posterior = if delta >= 0.0 {
        (-delta).exp() / (1.0 + (-delta).exp())
    } else {
        1.0 / (1.0 + delta.exp())
    };
    Some((u8::from(scores[1] > scores[0]), posterior))
}

#[cfg(test)]
mod tests {
    use super::bernoulli_naive_bayes as predict;

    #[test]
    fn examples() {
        let x = [1,0, 1,1, 0,1, 0,0];
        let labels = [0,0,1,1];
        let (a, p) = predict(&x, &labels, 4, 2, &[1, 0]).unwrap();
        assert_eq!(a, 0);
        assert!((p - 0.25).abs() < 1e-12);
        let (a, p) = predict(&x, &labels, 4, 2, &[0, 1]).unwrap();
        assert_eq!(a, 1);
        assert!((p - 0.75).abs() < 1e-12);
        assert_eq!(predict(&[0, 0], &[0, 1], 2, 1, &[0]), Some((0, 0.5)));
        assert_eq!(predict(&[0, 0, 0], &[0, 0, 1], 3, 1, &[0]).unwrap().0, 0);
        let mut big = vec![0; 64 * 16];
        big.extend(vec![1; 64 * 16]);
        let mut ys = vec![0; 64];
        ys.extend(vec![1; 64]);
        let (a, p) = predict(&big, &ys, 128, 16, &[1; 16]).unwrap();
        assert_eq!(a, 1);
        assert!(p > 0.999);
    }

    #[test]
    fn invalid() {
        let x = [1,0, 1,1, 0,1, 0,0];
        let ys = [0,0,1,1];
        assert!(predict(&x, &ys, 1, 2, &[1, 0]).is_none());
        assert!(predict(&x, &ys, 4, 0, &[1, 0]).is_none());
        assert!(predict(&x, &ys, 4, 17, &[1, 0]).is_none());
        assert!(predict(&x[..7], &ys, 4, 2, &[1, 0]).is_none());
        assert!(predict(&x, &ys[..3], 4, 2, &[1, 0]).is_none());
        assert!(predict(&x, &[0,0,0,0], 4, 2, &[1, 0]).is_none());
        assert!(predict(&x, &[0,0,1,2], 4, 2, &[1, 0]).is_none());
        assert!(predict(&[1,0,1,2,0,1,0,0], &ys, 4, 2, &[1, 0]).is_none());
        assert!(predict(&x, &ys, 4, 2, &[1,2]).is_none());
    }
}
