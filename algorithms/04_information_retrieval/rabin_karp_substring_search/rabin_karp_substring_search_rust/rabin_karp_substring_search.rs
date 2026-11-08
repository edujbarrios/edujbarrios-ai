//! Rabin-Karp exact matching with collision verification.
const BASE: u64 = 257;
const MODULUS: u64 = 1_000_000_007;

fn checked(s: &str) -> Result<(), &'static str> {
    if s.len() > 512 || !s.bytes().all(|x| (32..=126).contains(&x)) {
        return Err("expected printable ASCII string of length at most 512");
    }
    Ok(())
}

/// All zero-based start offsets, including overlaps.
pub fn rabin_karp_substring_search(
    text: &str, pattern: &str,
) -> Result<Vec<usize>, &'static str> {
    checked(text)?;
    checked(pattern)?;
    let t = text.as_bytes();
    let p = pattern.as_bytes();
    let (n, m) = (t.len(), p.len());
    if m == 0 { return Ok((0..=n).collect()); }
    if m > n { return Ok(vec![]); }
    let mut high_power = 1u64;
    for _ in 1..m { high_power = high_power * BASE % MODULUS; }
    let mut ph = 0u64;
    let mut th = 0u64;
    for i in 0..m {
        ph = (ph * BASE + u64::from(p[i])) % MODULUS;
        th = (th * BASE + u64::from(t[i])) % MODULUS;
    }
    let mut result = Vec::new();
    for start in 0..=n - m {
        if ph == th && t[start..start + m] == p[..] {
            result.push(start);
        }
        if start < n - m {
            let outgoing = u64::from(t[start]) * high_power % MODULUS;
            th = ((th + MODULUS - outgoing) % MODULUS * BASE
                  + u64::from(t[start + m])) % MODULUS;
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::rabin_karp_substring_search as search;
    #[test]
    fn examples() {
        assert_eq!(search("aaaa", "aa"), Ok(vec![0, 1, 2]));
        assert_eq!(search("abababa", "aba"), Ok(vec![0, 2, 4]));
        assert_eq!(search("ABABDABACDABABCABAB", "ABABCABAB"), Ok(vec![10]));
        assert_eq!(search("abc", "ABC"), Ok(vec![]));
        assert_eq!(search("ab", "abc"), Ok(vec![]));
    }
    #[test]
    fn boundaries() {
        assert_eq!(search("abc", ""), Ok(vec![0, 1, 2, 3]));
        assert_eq!(search("", ""), Ok(vec![0]));
        assert_eq!(search("", "a"), Ok(vec![]));
        assert_eq!(search(&"a".repeat(512), "aa"), Ok((0..511).collect()));
        assert_eq!(search(&"a".repeat(512), ""), Ok((0..=512).collect()));
    }
    #[test]
    fn invalid() {
        assert!(search(&"a".repeat(513), "a").is_err());
        assert!(search("a", &"a".repeat(513)).is_err());
        assert!(search("é", "e").is_err());
        assert!(search("a\tb", "a").is_err());
        assert!(search("a\0b", "a").is_err());
        assert!(search("a", "\n").is_err());
    }
}
