//! Sørensen–Dice coefficient for aligned binary masks.
/// Return 1 for two foreground-empty masks; reject invalid masks.
pub fn binary_dice_coefficient(prediction: &[u8], target: &[u8]) -> Result<f64, &'static str> {
    if prediction.is_empty() || prediction.len() != target.len() {
        return Err("masks must be nonempty and equally sized");
    }
    let mut overlap = 0usize;
    let mut count_a = 0usize;
    let mut count_b = 0usize;
    for (&a,&b) in prediction.iter().zip(target) {
        if a > 1 || b > 1 { return Err("mask pixels must be zero or one"); }
        count_a += a as usize;
        count_b += b as usize;
        overlap += (a & b) as usize;
    }
    if count_a + count_b == 0 { Ok(1.0) }
    else { Ok(2.0 * overlap as f64 / (count_a as f64 + count_b as f64)) }
}
#[cfg(test)]
mod tests {
    use super::binary_dice_coefficient as dice;
    #[test] fn overlap() {
        assert_eq!(dice(&[1,1,0,0], &[1,0,1,0]), Ok(0.5));
        assert_eq!(dice(&[1,0,1], &[1,0,1]), Ok(1.0));
        assert_eq!(dice(&[1,0], &[0,1]), Ok(0.0));
        assert_eq!(dice(&[0,0], &[0,0]), Ok(1.0));
    }
    #[test] fn invalid() {
        assert!(dice(&[], &[]).is_err());
        assert!(dice(&[1], &[]).is_err());
        assert!(dice(&[2], &[1]).is_err());
    }
}
