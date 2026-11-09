//! Center-expansion longest palindrome over printable ASCII.

/// Return (length, zero-based start) with earliest-start tie breaking.
pub fn longest_palindromic_substring(text: &str)
    -> Result<(usize, usize), &'static str> {
    if text.len() > 512 || !text.bytes().all(|ch| (32..=126).contains(&ch)) {
        return Err("expected printable ASCII string <=512 characters");
    }
    let bytes = text.as_bytes();
    let n = bytes.len() as isize;
    let mut best_length = 0usize;
    let mut best_start = 0usize;
    for center in 0..n {
        for right_offset in 0..=1 {
            let mut left = center;
            let mut right = center + right_offset;
            while left >= 0 && right < n && bytes[left as usize] == bytes[right as usize] {
                let length = (right - left + 1) as usize;
                let start = left as usize;
                if length > best_length || (length == best_length && start < best_start) {
                    best_length = length;
                    best_start = start;
                }
                left -= 1;
                right += 1;
            }
        }
    }
    Ok((best_length, best_start))
}

#[cfg(test)]
mod tests {
    use super::longest_palindromic_substring as match_text;
    #[test]
    fn examples() {
        assert_eq!(match_text("babad"),Ok((3,0)));
        assert_eq!(match_text("cbbd"),Ok((2,1)));
        assert_eq!(match_text("aaaa"),Ok((4,0)));
        assert_eq!(match_text("abc"),Ok((1,0)));
        assert_eq!(match_text("racecar"),Ok((7,0)));
        assert_eq!(match_text("abacdfgdcaba"),Ok((3,0)));
        assert_eq!(match_text("a b a"),Ok((5,0)));
        assert_eq!(match_text("a!a"),Ok((3,0)));
        assert_eq!(match_text("abbaxabba"),Ok((9,0)));
    }
    #[test]
    fn boundaries() {
        assert_eq!(match_text(""),Ok((0,0)));
        assert_eq!(match_text("z"),Ok((1,0)));
        assert_eq!(match_text(&"a".repeat(512)),Ok((512,0)));
        assert_eq!(match_text(&("a".repeat(256)+"&"+"b")),Ok((256,0)));
    }
    #[test]
    fn invalid() {
        assert!(match_text(&"a".repeat(513)).is_err());
        assert!(match_text("é").is_err());
        assert!(match_text("a\tb").is_err());
        assert!(match_text("a\0b").is_err());
        assert!(match_text("a\nb").is_err());
    }
}
