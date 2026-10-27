//! Otsu global threshold maximizing between-class gray-level variance.

/// Return the lowest threshold with maximum valid between-class variance.
pub fn otsu_threshold(pixels: &[u8]) -> Result<u8, &'static str> {
    if !(1..=4096).contains(&pixels.len()) {
        return Err("pixel count must be in 1..4096");
    }
    let mut histogram = [0_usize;256];
    let mut total_sum: usize = 0;
    for &pixel in pixels {
        histogram[pixel as usize] += 1;
        total_sum += pixel as usize;
    }
    let mut count_low = 0_usize;
    let mut sum_low = 0_usize;
    let mut best = -1.0_f64;
    let mut choice = 0_u8;
    for t in 0..255 {
        count_low += histogram[t];
        sum_low += t*histogram[t];
        if count_low == 0 || count_low == pixels.len() { continue; }
        let diff = (total_sum * count_low) as f64 -
                   (pixels.len() * sum_low) as f64;
        let score = diff*diff/(count_low*(pixels.len()-count_low)) as f64;
        if score > best {
            best = score;
            choice = t as u8;
        }
    }
    Ok(choice)
}

#[cfg(test)]
mod tests {
    use super::otsu_threshold as otsu;
    #[test]
    fn reference() {
        assert_eq!(otsu(&[10,20,200,210]),Ok(20));
        assert_eq!(otsu(&[0,0,255,255]),Ok(0));
        assert_eq!(otsu(&[1,2]),Ok(1));
        assert_eq!(otsu(&[7,7,7]),Ok(0));
        assert_eq!(otsu(&[255]),Ok(0));
        assert_eq!(otsu(&[0;10]),Ok(0));
    }
    #[test]
    fn invalid() {
        assert!(otsu(&[]).is_err());
        assert!(otsu(&vec![0_u8;4097]).is_err());
    }
}
