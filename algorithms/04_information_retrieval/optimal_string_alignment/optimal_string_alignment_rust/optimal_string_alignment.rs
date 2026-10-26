//! Restricted Damerau (optimal string alignment) edit distance.

/// Minimum single-use insert/delete/substitute/adjacent-swap edits on i32 IDs.
pub fn optimal_string_alignment(first: &[i32], second: &[i32]) -> usize {
    let (a, b) = if first.len() >= second.len() {
        (first, second)
    } else {
        (second, first)
    };
    let (n, m) = (a.len(), b.len());
    let mut older = vec![0_usize; m + 1];
    let mut previous: Vec<usize> = (0..=m).collect();
    let mut current = vec![0_usize; m + 1];
    for i in 1..=n {
        current[0] = i;
        for j in 1..=m {
            let cost = usize::from(a[i - 1] != b[j - 1]);
            let mut best = (previous[j] + 1)
                .min(current[j - 1] + 1)
                .min(previous[j - 1] + cost);
            if i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1] {
                best = best.min(older[j - 2] + 1);
            }
            current[j] = best;
        }
        std::mem::swap(&mut older, &mut previous);
        std::mem::swap(&mut previous, &mut current);
    }
    previous[m]
}

#[cfg(test)]
mod tests {
    use super::optimal_string_alignment as osa;

    #[test]
    fn fixtures() {
        assert_eq!(osa(&[1, 2], &[2, 1]), 1);
        assert_eq!(osa(&[1, 2, 3], &[1, 3, 2]), 1);
        assert_eq!(osa(&[3, 1], &[1, 2, 3]), 3);
        assert_eq!(osa(&[], &[1, 2]), 2);
        assert_eq!(osa(&[1, 2], &[]), 2);
        assert_eq!(osa(&[], &[]), 0);
        assert_eq!(osa(&[1, 2, 3], &[1, 2, 3]), 0);
        assert_eq!(osa(&[1, 2, 3], &[1, 4, 3]), 1);
        assert_eq!(osa(&[1, 1], &[1]), 1);
        assert_eq!(osa(&[i32::MIN], &[i32::MAX]), 1);
    }
}
