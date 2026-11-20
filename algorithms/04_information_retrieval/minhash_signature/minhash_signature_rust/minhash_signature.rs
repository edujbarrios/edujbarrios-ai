//! Deterministic MinHash-style signature using explicit modular hash pairs.

const PRIME: u32 = 65521;

/// Minima across tokens for every (multiplier, offset) pair; empty set returns
/// the prime sentinel in each signature slot.
pub fn minhash_signature(
    tokens: &[u32], a: &[u32], b: &[u32],
) -> Result<Vec<u32>, &'static str> {
    if a.is_empty() || a.len() != b.len() ||
        tokens.iter().any(|&id| id >= PRIME) ||
        a.iter().any(|&coef| coef == 0 || coef >= PRIME) ||
        b.iter().any(|&offset| offset >= PRIME) {
        return Err("invalid tokens or hash coefficient vectors");
    }
    let mut result = vec![PRIME; a.len()];
    for &token in tokens {
        for (j, (&multiplier, &offset)) in a.iter().zip(b.iter()).enumerate() {
            let numerator = multiplier as u64 * token as u64 + offset as u64;
            let hashed = (numerator % PRIME as u64) as u32;
            result[j] = result[j].min(hashed);
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::minhash_signature as signature;

    #[test]
    fn known_hashes_and_order_invariance() {
        assert_eq!(signature(&[1,2,3], &[1,3], &[0,7]), Ok(vec![1,10]));
        assert_eq!(signature(&[3,1,2,1], &[1,3], &[0,7]), Ok(vec![1,10]));
        assert_eq!(signature(&[], &[1,3], &[0,7]), Ok(vec![65521,65521]));
        assert_eq!(signature(&[65520], &[65520], &[65520]), Ok(vec![0]));
        assert_eq!(signature(&[0], &[1], &[13]), Ok(vec![13]));
    }

    #[test]
    fn invalid_inputs() {
        assert!(signature(&[1], &[], &[]).is_err());
        assert!(signature(&[1], &[1], &[]).is_err());
        assert!(signature(&[65521], &[1], &[0]).is_err());
        assert!(signature(&[1], &[0], &[0]).is_err());
        assert!(signature(&[1], &[65521], &[0]).is_err());
        assert!(signature(&[1], &[1], &[65521]).is_err());
    }
}
