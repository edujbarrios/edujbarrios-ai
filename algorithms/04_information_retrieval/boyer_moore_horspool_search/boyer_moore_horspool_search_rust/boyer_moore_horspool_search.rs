//! Boyer-Moore-Horspool substring search with ASCII skip shifts.

fn checked(s: &str) -> Result<(), &'static str> {
    if s.len() > 512 || !s.bytes().all(|ch| (32..=126).contains(&ch)) {
        return Err("expected printable ASCII string <=512 characters");
    }
    Ok(())
}

/// All exact zero-based pattern occurrences, including overlaps.
pub fn boyer_moore_horspool_search(
    text: &str, pattern: &str,
) -> Result<Vec<usize>, &'static str> {
    checked(text)?;
    checked(pattern)?;
    let t = text.as_bytes();
    let p = pattern.as_bytes();
    let (n, m) = (t.len(), p.len());
    if m == 0 { return Ok((0..=n).collect()); }
    if m > n { return Ok(vec![]); }
    let mut shift = [m; 128];
    for j in 0..m-1 { shift[usize::from(p[j])] = m - 1 - j; }
    let mut result = Vec::new();
    let mut start = 0;
    while start <= n-m {
        let mut j = m;
        while j > 0 && t[start+j-1] == p[j-1] { j -= 1; }
        if j == 0 { result.push(start); }
        start += shift[usize::from(t[start+m-1])];
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::boyer_moore_horspool_search as search;
    #[test]
    fn examples() {
        assert_eq!(search("ABAAABCD","ABC"),Ok(vec![4]));
        assert_eq!(search("aaaa","aa"),Ok(vec![0,1,2]));
        assert_eq!(search("abababa","aba"),Ok(vec![0,2,4]));
        assert_eq!(search("ABCABCABC","ABC"),Ok(vec![0,3,6]));
        assert_eq!(search("mississippi","issi"),Ok(vec![1,4]));
        assert_eq!(search("abc","XYZ"),Ok(vec![]));
        assert_eq!(search("abc","ABC"),Ok(vec![]));
        assert_eq!(search("ab","abc"),Ok(vec![]));
        assert_eq!(search("~a~a","~a"),Ok(vec![0,2]));
        assert_eq!(search("a a a","a "),Ok(vec![0,2]));
    }
    #[test]
    fn boundaries() {
        assert_eq!(search("abc",""),Ok(vec![0,1,2,3]));
        assert_eq!(search("",""),Ok(vec![0]));
        assert_eq!(search("","a"),Ok(vec![]));
        assert_eq!(search(&"a".repeat(512),"aa"),Ok((0..511).collect()));
        assert_eq!(search(&"a".repeat(512),&"a".repeat(512)),Ok(vec![0]));
        assert_eq!(search(&"a".repeat(512),""),Ok((0..=512).collect()));
        assert_eq!(search("a","a"),Ok(vec![0]));
    }
    #[test]
    fn invalid() {
        assert!(search(&"a".repeat(513),"a").is_err());
        assert!(search("a",&"a".repeat(513)).is_err());
        assert!(search("é","e").is_err());
        assert!(search("a\tb","a").is_err());
        assert!(search("a\0b","a").is_err());
        assert!(search("a","\n").is_err());
    }
}
