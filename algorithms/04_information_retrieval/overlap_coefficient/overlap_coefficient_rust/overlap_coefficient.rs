//! Exact overlap coefficient for sets of signed 32-bit token IDs.

/// Return intersection / smaller set size. Both empty sets score 1.
pub fn overlap_coefficient(first: &[i32], second: &[i32]) -> f64 {
    let mut a = first.to_vec();
    let mut b = second.to_vec();
    a.sort_unstable();
    b.sort_unstable();
    a.dedup();
    b.dedup();
    if a.is_empty() && b.is_empty() { return 1.0; }
    if a.is_empty() || b.is_empty() { return 0.0; }
    let (mut i, mut j, mut common) = (0, 0, 0);
    while i < a.len() && j < b.len() {
        if a[i] == b[j] { i += 1; j += 1; common += 1; }
        else if a[i] < b[j] { i += 1; }
        else { j += 1; }
    }
    common as f64 / a.len().min(b.len()) as f64
}

#[cfg(test)]
mod tests {
    use super::overlap_coefficient as oc;
    #[test]
    fn reference() {
        assert_eq!(oc(&[1,1,2], &[1,2,3,4]), 1.0);
        assert_eq!(oc(&[1,2,3,4], &[1,1,2]), 1.0);
        assert_eq!(oc(&[1,2], &[2,3]), 0.5);
        assert_eq!(oc(&[], &[]), 1.0);
        assert_eq!(oc(&[], &[7]), 0.0);
        assert_eq!(oc(&[7], &[]), 0.0);
        assert_eq!(oc(&[i32::MIN], &[i32::MAX,i32::MIN]), 1.0);
    }
}
