//! Find all exact substring occurrences in printable ASCII text with KMP.

fn validate(s: &str) -> Result<(), &'static str> {
    if s.len() > 512 || !s.bytes().all(|b| (32..=126).contains(&b)) {
        return Err("expected printable ASCII of length at most 512");
    }
    Ok(())
}

/// Zero-based offsets, including overlaps and each empty-pattern boundary.
pub fn kmp_substring_search(text: &str, pattern: &str) -> Result<Vec<usize>, &'static str> {
    validate(text)?;
    validate(pattern)?;
    let t = text.as_bytes();
    let p = pattern.as_bytes();
    if p.is_empty() {
        return Ok((0..=t.len()).collect());
    }
    let mut prefix = vec![0usize; p.len()];
    let mut j = 0;
    for i in 1..p.len() {
        while j > 0 && p[i] != p[j] {
            j = prefix[j - 1];
        }
        if p[i] == p[j] {
            j += 1;
        }
        prefix[i] = j;
    }
    let mut result = Vec::new();
    j = 0;
    for (i, &ch) in t.iter().enumerate() {
        while j > 0 && ch != p[j] {
            j = prefix[j - 1];
        }
        if ch == p[j] {
            j += 1;
        }
        if j == p.len() {
            result.push(i + 1 - p.len());
            j = prefix[j - 1];
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::kmp_substring_search as search;

    #[test]
    fn examples() {
        assert_eq!(search("aaaa", "aa"), Ok(vec![0, 1, 2]));
        assert_eq!(search("ABABDABACDABABCABAB", "ABABCABAB"), Ok(vec![10]));
        assert_eq!(search("abcabc", "abc"), Ok(vec![0, 3]));
        assert_eq!(search("abc", "z"), Ok(vec![]));
        assert_eq!(search("abc", "ABC"), Ok(vec![]));
        assert_eq!(search("!", "!"), Ok(vec![0]));
    }

    #[test]
    fn empty_and_limits() {
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
        assert!(search("a\tb", "a").is_err());
        assert!(search("a\nb", "a").is_err());
        assert!(search("a\0b", "a").is_err());
        assert!(search("é", "a").is_err());
        assert!(search("a", "\u{0001}").is_err());
    }
}
