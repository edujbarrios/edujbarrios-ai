//! Full Damerau-Levenshtein edit distance for ordered byte token IDs.

/// Return the unrestricted unit-cost Damerau distance, or None for lengths >256.
pub fn damerau_levenshtein_distance(first: &[u8], second: &[u8]) -> Option<usize> {
    let (n, m) = (first.len(), second.len());
    if n > 256 || m > 256 {
        return None;
    }
    let upper = n + m;
    let mut h = vec![vec![0_usize; m + 2]; n + 2];
    h[0][0] = upper;
    for i in 0..=n {
        h[i + 1][0] = upper;
        h[i + 1][1] = i;
    }
    for j in 0..=m {
        h[0][j + 1] = upper;
        h[1][j + 1] = j;
    }
    let mut seen = [0_usize; 256];
    for i in 1..=n {
        let mut last_match = 0;
        for j in 1..=m {
            let prev_a = seen[second[j - 1] as usize];
            let prev_b = last_match;
            let cost = usize::from(first[i - 1] != second[j - 1]);
            if cost == 0 {
                last_match = j;
            }
            h[i + 1][j + 1] = (h[i][j] + cost)
                .min(h[i + 1][j] + 1)
                .min(h[i][j + 1] + 1)
                .min(h[prev_a][prev_b] + (i - prev_a - 1) + 1 + (j - prev_b - 1));
        }
        seen[first[i - 1] as usize] = i;
    }
    Some(h[n + 1][m + 1])
}

#[cfg(test)]
mod tests {
    use super::damerau_levenshtein_distance as dl;

    #[test]
    fn examples() {
        assert_eq!(dl(&[67, 65], &[65, 66, 67]), Some(2));
        assert_eq!(dl(&[65, 66, 67], &[67, 65]), Some(2));
        assert_eq!(dl(&[1, 2], &[2, 1]), Some(1));
        assert_eq!(dl(&[1, 1, 2], &[1, 2, 1]), Some(1));
        assert_eq!(dl(&[0, 255], &[255, 0]), Some(1));
        assert_eq!(dl(&[1, 2, 3], &[1, 4, 3]), Some(1));
        assert_eq!(dl(&[1, 2, 3], &[1, 2, 3]), Some(0));
        assert_eq!(dl(&[], &[0, 255]), Some(2));
        assert_eq!(dl(&[0, 255], &[]), Some(2));
        assert_eq!(dl(&[], &[]), Some(0));
        assert_eq!(dl(&vec![0; 256], &vec![0; 256]), Some(0));
        assert_eq!(dl(&vec![0; 256], &vec![1; 256]), Some(256));
    }

    #[test]
    fn invalid() {
        assert_eq!(dl(&vec![0; 257], &[]), None);
        assert_eq!(dl(&[], &vec![0; 257]), None);
    }
}
