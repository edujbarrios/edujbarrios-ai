//! Exact Sørensen–Dice coefficient for integer token sets.

/// Compare sequences as sets of signed 32-bit IDs, ignoring duplicates.
/// The coefficient of two empty sets is defined to be 1.
pub fn dice_similarity(first: &[i32], second: &[i32]) -> f64 {
    let mut a = first.to_vec();
    let mut b = second.to_vec();
    a.sort_unstable();
    b.sort_unstable();
    a.dedup();
    b.dedup();
    if a.is_empty() && b.is_empty() {
        return 1.0;
    }
    let (mut i, mut j, mut intersection) = (0, 0, 0);
    while i < a.len() && j < b.len() {
        if a[i] == b[j] {
            intersection += 1;
            i += 1;
            j += 1;
        } else if a[i] < b[j] {
            i += 1;
        } else {
            j += 1;
        }
    }
    2.0 * intersection as f64 / (a.len() as f64 + b.len() as f64)
}

#[cfg(test)]
mod tests {
    use super::dice_similarity;

    #[test]
    fn fixtures() {
        assert!((dice_similarity(&[1, 1, 2, 3], &[2, 3, 4]) - 2.0 / 3.0).abs() < 1e-12);
        assert_eq!(dice_similarity(&[], &[]), 1.0);
        assert_eq!(dice_similarity(&[], &[7]), 0.0);
        assert_eq!(dice_similarity(&[7], &[]), 0.0);
        assert_eq!(dice_similarity(&[-5, 3], &[3, -5]), 1.0);
        assert_eq!(dice_similarity(&[i32::MIN], &[i32::MIN]), 1.0);
        assert_eq!(dice_similarity(&[i32::MIN], &[i32::MAX]), 0.0);
    }
}
