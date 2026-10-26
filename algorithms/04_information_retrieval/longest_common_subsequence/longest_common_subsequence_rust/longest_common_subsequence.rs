//! Length of a longest common token subsequence with two DP rows.

/// Return the LCS length for two sequences of signed 32-bit token IDs.
pub fn longest_common_subsequence(first: &[i32], second: &[i32]) -> usize {
    let (a, b) = if first.len() >= second.len() {
        (first, second)
    } else {
        (second, first)
    };
    let mut previous = vec![0_usize; b.len() + 1];
    let mut current = vec![0_usize; b.len() + 1];
    for &item in a {
        current[0] = 0;
        for (j, &other) in b.iter().enumerate() {
            current[j + 1] = if item == other {
                previous[j] + 1
            } else {
                previous[j + 1].max(current[j])
            };
        }
        std::mem::swap(&mut previous, &mut current);
    }
    previous[b.len()]
}

#[cfg(test)]
mod tests {
    use super::longest_common_subsequence as lcs;
    #[test]
    fn fixtures() {
        assert_eq!(lcs(&[1, 2, 3, 4], &[2, 4, 3]), 2);
        assert_eq!(lcs(&[2, 4, 3], &[1, 2, 3, 4]), 2);
        assert_eq!(lcs(&[1, 1, 2], &[1, 2, 1]), 2);
        assert_eq!(lcs(&[], &[1, 2]), 0);
        assert_eq!(lcs(&[1, 2], &[]), 0);
        assert_eq!(lcs(&[], &[]), 0);
        assert_eq!(lcs(&[1, 2], &[1, 2]), 2);
        assert_eq!(lcs(&[7, 7, 7], &[7, 7]), 2);
        assert_eq!(lcs(&[i32::MIN], &[i32::MIN]), 1);
        assert_eq!(lcs(&[i32::MAX], &[i32::MIN]), 0);
    }
}
