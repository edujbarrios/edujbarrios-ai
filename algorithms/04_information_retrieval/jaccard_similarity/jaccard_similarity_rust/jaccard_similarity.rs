//! Exact Jaccard similarity of integer token sets.

/// Compute intersection over union. By convention, J(empty, empty) == 1.
pub fn jaccard_similarity(left: &[i32], right: &[i32]) -> f64 {
    let mut a = left.to_vec();
    let mut b = right.to_vec();
    a.sort_unstable();
    b.sort_unstable();
    a.dedup();
    b.dedup();

    if a.is_empty() && b.is_empty() {
        return 1.0;
    }

    let (mut i, mut j, mut shared, mut union) = (0, 0, 0, 0);
    while i < a.len() || j < b.len() {
        if j == b.len() || (i < a.len() && a[i] < b[j]) {
            i += 1;
        } else if i == a.len() || b[j] < a[i] {
            j += 1;
        } else {
            shared += 1;
            i += 1;
            j += 1;
        }
        union += 1;
    }
    shared as f64 / union as f64
}

#[cfg(test)]
mod tests {
    use super::jaccard_similarity as j;
    #[test]
    fn overlap_empty_and_extremes() {
        assert_eq!(j(&[1,1,2,3], &[2,3,4]), 0.5);
        assert_eq!(j(&[], &[]), 1.0);
        assert_eq!(j(&[], &[3]), 0.0);
        assert_eq!(j(&[-5,3], &[3,-5]), 1.0);
        assert_eq!(j(&[i32::MIN,i32::MAX], &[i32::MAX,i32::MIN]),1.0);
    }
}
