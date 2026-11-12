//! Equal-weight univariate isotonic regression with pooled adjacent blocks.

/// Nondecreasing least-squares fit, or None for invalid observations.
pub fn isotonic_regression(values: &[f64]) -> Option<Vec<f64>> {
    if values.is_empty() || values.len() > 512 ||
        values.iter().any(|x| !x.is_finite() || x.abs() > 1_000_000.0) {
        return None;
    }
    let mut means: Vec<f64> = Vec::new();
    let mut counts: Vec<usize> = Vec::new();
    for &value in values {
        means.push(value);
        counts.push(1);
        while means.len() > 1 && means[means.len() - 2] > means[means.len() - 1] {
            let last = means.len() - 1;
            let count = counts[last - 1] + counts[last];
            means[last - 1] = (means[last - 1] * counts[last - 1] as f64 +
                               means[last] * counts[last] as f64) / count as f64;
            counts[last - 1] = count;
            means.pop();
            counts.pop();
        }
    }
    let mut output = Vec::with_capacity(values.len());
    for (&mean, &count) in means.iter().zip(&counts) {
        output.extend(std::iter::repeat_n(mean, count));
    }
    Some(output)
}

#[cfg(test)]
mod tests {
    use super::isotonic_regression as fit;

    #[test]
    fn examples() {
        assert_eq!(fit(&[3.0, 1.0, 2.0, 4.0]), Some(vec![2.0, 2.0, 2.0, 4.0]));
        assert_eq!(fit(&[-2.0, 2.0, 0.0, 4.0]), Some(vec![-2.0, 1.0, 1.0, 4.0]));
        assert_eq!(fit(&[1.0, 2.0, 2.0, 3.0]), Some(vec![1.0, 2.0, 2.0, 3.0]));
        assert_eq!(fit(&[3.0, 2.0, 1.0]), Some(vec![2.0, 2.0, 2.0]));
        assert_eq!(fit(&[1.0]), Some(vec![1.0]));
        assert_eq!(fit(&[1_000_000.0, -1_000_000.0]), Some(vec![0.0, 0.0]));
        let descending: Vec<f64> = (1..=512).rev().map(|x| x as f64).collect();
        assert_eq!(fit(&descending), Some(vec![256.5; 512]));
    }

    #[test]
    fn invalid() {
        assert!(fit(&[]).is_none());
        assert!(fit(&vec![1.0; 513]).is_none());
        assert!(fit(&[f64::NAN]).is_none());
        assert!(fit(&[f64::INFINITY]).is_none());
        assert!(fit(&[1_000_001.0]).is_none());
    }
}
