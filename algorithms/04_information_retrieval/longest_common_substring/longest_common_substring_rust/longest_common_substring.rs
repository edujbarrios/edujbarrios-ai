//! Longest common contiguous ASCII substring with deterministic ties.

fn checked(s: &str) -> Result<(), &'static str> {
    if s.len() > 512 || !s.bytes().all(|v| (32..=126).contains(&v)) {
        return Err("expected printable ASCII <=512 characters");
    }
    Ok(())
}

/// (length, zero-based a start, zero-based b start). Empty match => (0,0,0).
pub fn longest_common_substring(a: &str, b: &str)
    -> Result<(usize, usize, usize), &'static str> {
    checked(a)?;
    checked(b)?;
    let a = a.as_bytes();
    let b = b.as_bytes();
    let mut prev = vec![0usize; b.len() + 1];
    let mut curr = vec![0usize; b.len() + 1];
    let (mut best, mut best_a, mut best_b) = (0usize, 0usize, 0usize);
    for (i, &left) in a.iter().enumerate() {
        curr[0] = 0;
        for (j, &right) in b.iter().enumerate() {
            let length = if left == right { prev[j] + 1 } else { 0 };
            curr[j + 1] = length;
            let sa = i + 1 - length;
            let sb = j + 1 - length;
            if length > best || (length != 0 && length == best
                && (sa, sb) < (best_a, best_b)) {
                (best, best_a, best_b) = (length, sa, sb);
            }
        }
        std::mem::swap(&mut prev, &mut curr);
    }
    Ok((best, best_a, best_b))
}

#[cfg(test)]
mod tests {
    use super::longest_common_substring as match_text;
    #[test]
    fn examples() {
        assert_eq!(match_text("banana", "ananas"), Ok((5, 1, 0)));
        assert_eq!(match_text("abcdef", "zabcf"), Ok((3, 0, 1)));
        assert_eq!(match_text("ababa", "babab"), Ok((4, 0, 1)));
        assert_eq!(match_text("abcabc", "abc"), Ok((3, 0, 0)));
        assert_eq!(match_text("aa", "baaa"), Ok((2, 0, 1)));
        assert_eq!(match_text("ABC", "abc"), Ok((0, 0, 0)));
    }
    #[test]
    fn boundaries() {
        assert_eq!(match_text("", ""), Ok((0, 0, 0)));
        assert_eq!(match_text("", "a"), Ok((0, 0, 0)));
        assert_eq!(match_text("a", ""), Ok((0, 0, 0)));
        assert_eq!(match_text(&"a".repeat(512), &"a".repeat(512)), Ok((512, 0, 0)));
    }
    #[test]
    fn invalid() {
        assert!(match_text(&"a".repeat(513), "a").is_err());
        assert!(match_text("a", &"a".repeat(513)).is_err());
        assert!(match_text("é", "e").is_err());
        assert!(match_text("a\tb", "a").is_err());
        assert!(match_text("a\0b", "a").is_err());
        assert!(match_text("a", "\n").is_err());
    }
}
