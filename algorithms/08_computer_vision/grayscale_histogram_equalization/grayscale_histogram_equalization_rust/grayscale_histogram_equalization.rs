//! Global 8-bit histogram equalization with cdf_min and half-up rounding.

/// Equalize a grayscale image, row-major, dimensions 1..64.
pub fn grayscale_histogram_equalization(
    pixels: &[u8], height: usize, width: usize,
) -> Result<Vec<u8>, &'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width)
        || pixels.len() != height * width {
        return Err("invalid image dimensions or length");
    }
    let mut histogram = [0usize; 256];
    for &value in pixels { histogram[usize::from(value)] += 1; }
    let cdf_min = histogram.iter().copied().find(|&n| n != 0).unwrap();
    let denom = pixels.len() - cdf_min;
    if denom == 0 { return Ok(pixels.to_vec()); }
    let mut mapping = [0u8; 256];
    let mut cumulative = 0usize;
    for (v, &count) in histogram.iter().enumerate() {
        cumulative += count;
        if cumulative > cdf_min {
            let numerator = 255 * (cumulative - cdf_min);
            mapping[v] = ((numerator * 2 + denom) / (2 * denom)) as u8;
        }
    }
    Ok(pixels.iter().map(|&v| mapping[usize::from(v)]).collect())
}

#[cfg(test)]
mod tests {
    use super::grayscale_histogram_equalization as eq;
    #[test]
    fn examples() {
        assert_eq!(eq(&[10,10,20,30],2,2),Ok(vec![0,0,128,255]));
        assert_eq!(eq(&[20,20,20],1,3),Ok(vec![20,20,20]));
        assert_eq!(eq(&[0,255],1,2),Ok(vec![0,255]));
        assert_eq!(eq(&[1],1,1),Ok(vec![1]));
        assert_eq!(eq(&[0,1,2,3,4],1,5),Ok(vec![0,64,128,191,255]));
    }
    #[test]
    fn maximum() {
        let mut image = vec![0u8;4096];
        image[4095]=255;
        let mapped=eq(&image,64,64).unwrap();
        assert!(mapped[..4095].iter().all(|&x| x == 0));
        assert_eq!(mapped[4095],255);
    }
    #[test]
    fn invalid() {
        assert!(eq(&[0],0,1).is_err());
        assert!(eq(&[0],1,0).is_err());
        assert!(eq(&[0],65,1).is_err());
        assert!(eq(&[0],1,65).is_err());
        assert!(eq(&[],1,1).is_err());
        assert!(eq(&[0,1],1,1).is_err());
    }
}
