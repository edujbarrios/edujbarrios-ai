//! Inclusive 2D prefix sums of unsigned 8-bit grayscale pixels.

/// Compute row-major, same-size inclusive integral image (H,W in 1..64).
pub fn integral_image(
    pixels: &[u8], height: usize, width: usize,
) -> Result<Vec<u32>, &'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width)
        || pixels.len() != height * width {
        return Err("invalid image dimensions or pixel count");
    }
    let mut output = vec![0u32; height * width];
    for row in 0..height {
        let mut row_sum = 0u32;
        for col in 0..width {
            let i = row * width + col;
            row_sum += u32::from(pixels[i]);
            output[i] = row_sum + if row > 0 { output[i - width] } else { 0 };
        }
    }
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::integral_image as sum;
    #[test]
    fn examples() {
        assert_eq!(sum(&[1, 2, 3, 4], 2, 2), Ok(vec![1, 3, 4, 10]));
        assert_eq!(sum(&[0, 0, 0, 0], 2, 2), Ok(vec![0, 0, 0, 0]));
        assert_eq!(sum(&[5], 1, 1), Ok(vec![5]));
        assert_eq!(sum(&[1, 2, 3], 1, 3), Ok(vec![1, 3, 6]));
        assert_eq!(sum(&[1, 2, 3], 3, 1), Ok(vec![1, 3, 6]));
    }
    #[test]
    fn maximum_size() {
        let output = sum(&[255u8; 4096], 64, 64).unwrap();
        assert_eq!(output[0], 255);
        assert_eq!(output[63], 64 * 255);
        assert_eq!(output[4095], 64 * 64 * 255);
    }
    #[test]
    fn invalid() {
        assert!(sum(&[0], 0, 1).is_err());
        assert!(sum(&[0], 1, 0).is_err());
        assert!(sum(&[0], 65, 1).is_err());
        assert!(sum(&[0], 1, 65).is_err());
        assert!(sum(&[], 1, 1).is_err());
        assert!(sum(&[0, 1], 1, 1).is_err());
    }
}
