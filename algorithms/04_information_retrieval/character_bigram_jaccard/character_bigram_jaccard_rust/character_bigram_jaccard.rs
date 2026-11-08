//! Exact character bigram set Jaccard similarity over printable ASCII.

const ALPHABET: usize = 95;
const PAIRS: usize = ALPHABET * ALPHABET;

fn checked(s: &str) -> Result<(), &'static str> {
    if s.len() > 512 || !s.bytes().all(|ch| (32..=126).contains(&ch)) {
        return Err("expected printable ASCII of length at most 512");
    }
    Ok(())
}

/// Bigram Jaccard score in [0,1]; empty union gives 1.0.
pub fn character_bigram_jaccard(a: &str, b: &str) -> Result<f64, &'static str> {
    checked(a)?;
    checked(b)?;
    let mut left = [false; PAIRS];
    let mut right = [false; PAIRS];
    for pair in a.as_bytes().windows(2) {
        let key = usize::from(pair[0] - 32) * ALPHABET + usize::from(pair[1] - 32);
        left[key] = true;
    }
    for pair in b.as_bytes().windows(2) {
        let key = usize::from(pair[0] - 32) * ALPHABET + usize::from(pair[1] - 32);
        right[key] = true;
    }
    let mut intersection = 0usize;
    let mut union_count = 0usize;
    for i in 0..PAIRS {
        if left[i] && right[i] { intersection += 1; }
        if left[i] || right[i] { union_count += 1; }
    }
    Ok(if union_count == 0 { 1.0 }
       else { intersection as f64 / union_count as f64 })
}

#[cfg(test)]
mod tests {
    use super::character_bigram_jaccard as score;
    fn near(a: &str,b: &str,expected: f64) {
        assert!((score(a,b).unwrap()-expected).abs()<1e-12);
    }
    #[test]
    fn examples() {
        near("banana","bandana",0.6);
        near("bandana","banana",0.6);
        near("aaaa","aa",1.0);
        near("abcd","cdef",0.2);
        near("abc","xyz",0.0);
        near("abc","abcd",2.0/3.0);
        near("AB","ab",0.0);
    }
    #[test]
    fn bounds() {
        near("","",1.0);
        near("a","",1.0);
        near("a","b",1.0);
        near("a","ab",0.0);
        near(&"a".repeat(512),"aa",1.0);
        near(&"a".repeat(512),&"b".repeat(512),0.0);
    }
    #[test]
    fn invalid() {
        assert!(score(&"a".repeat(513),"a").is_err());
        assert!(score("a",&"a".repeat(513)).is_err());
        assert!(score("é","e").is_err());
        assert!(score("a\tb","a").is_err());
        assert!(score("a\0b","a").is_err());
        assert!(score("a","\n").is_err());
    }
}
