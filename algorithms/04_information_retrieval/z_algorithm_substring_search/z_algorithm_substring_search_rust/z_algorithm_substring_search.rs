//! Linear Z-algorithm exact substring search over printable ASCII.

fn checked(s: &str) -> Result<(), &'static str> {
    if s.len() > 512 || !s.bytes().all(|c| (32..=126).contains(&c)) {
        return Err("expected printable ASCII string <=512 characters");
    }
    Ok(())
}

/// All exact zero-based substring occurrences, including overlapping matches.
pub fn z_algorithm_substring_search(
    text: &str, pattern: &str,
) -> Result<Vec<usize>, &'static str> {
    checked(text)?;
    checked(pattern)?;
    let (n,m)=(text.len(),pattern.len());
    if m == 0 { return Ok((0..=n).collect()); }
    if m > n { return Ok(vec![]); }
    let mut seq = Vec::with_capacity(m+1+n);
    seq.extend_from_slice(pattern.as_bytes());
    seq.push(0);
    seq.extend_from_slice(text.as_bytes());
    let mut z = vec![0usize;seq.len()];
    let (mut left,mut right)=(0usize,0usize);
    let mut matches = Vec::new();
    for i in 1..seq.len() {
        if i < right { z[i] = (right-i).min(z[i-left]); }
        while i+z[i]<seq.len() && seq[z[i]]==seq[i+z[i]] { z[i]+=1; }
        if i+z[i]>right {left=i;right=i+z[i];}
        if i>=m+1 && z[i]>=m {matches.push(i-m-1);}
    }
    Ok(matches)
}

#[cfg(test)]
mod tests {
    use super::z_algorithm_substring_search as search;
    #[test]
    fn examples() {
        assert_eq!(search("banana","ana"),Ok(vec![1,3]));
        assert_eq!(search("aaaa","aa"),Ok(vec![0,1,2]));
        assert_eq!(search("abcabc","abc"),Ok(vec![0,3]));
        assert_eq!(search("abc","xyz"),Ok(vec![]));
        assert_eq!(search("abc","ABC"),Ok(vec![]));
        assert_eq!(search("ab","abc"),Ok(vec![]));
        assert_eq!(search("mississippi","issi"),Ok(vec![1,4]));
        assert_eq!(search("a a a","a "),Ok(vec![0,2]));
        assert_eq!(search("~!~!","~!"),Ok(vec![0,2]));
        assert_eq!(search("aba","aba"),Ok(vec![0]));
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
