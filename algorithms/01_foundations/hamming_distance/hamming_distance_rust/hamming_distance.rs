//! Symbol-wise Hamming distance between byte slices.

/// Count differing byte positions or return an error for unequal lengths.
pub fn hamming_distance(a: &[u8], b: &[u8]) -> Result<usize, &'static str> {
    if a.len() != b.len() {
        return Err("inputs must have equal length");
    }
    Ok(a.iter().zip(b.iter()).filter(|(x, y)| x != y).count())
}

#[cfg(test)]
mod tests {
    use super::hamming_distance;

    #[test]
    fn mismatches() {
        assert_eq!(hamming_distance(&[1, 0, 1, 1], &[1, 1, 0, 1]), Ok(2));
        assert_eq!(hamming_distance(b"abc", b"abc"), Ok(0));
        assert_eq!(hamming_distance(&[0], &[255]), Ok(1));
    }

    #[test]
    fn empty_and_invalid() {
        assert_eq!(hamming_distance(&[], &[]), Ok(0));
        assert!(hamming_distance(b"a", b"").is_err());
    }
}
