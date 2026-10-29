//! Fixed 3x3 median filtering with replicated boundaries.

/// Filter finite, bounded row-major grayscale pixels with dimensions in 1..64.
pub fn median_filter(pixels: &[f64], height: usize, width: usize)
    -> Result<Vec<f64>, &'static str>
{
    if !(1..=64).contains(&height) || !(1..=64).contains(&width) ||
       pixels.len() != height * width ||
       pixels.iter().any(|x| !x.is_finite() || x.abs() > 1_000_000.0) {
        return Err("invalid image dimensions or pixel values");
    }
    let mut out = Vec::with_capacity(height * width);
    for row in 0..height {
        for col in 0..width {
            let mut window = [0.0_f64; 9];
            let mut index = 0;
            for dr in -1_i32..=1 {
                for dc in -1_i32..=1 {
                    let r = (row as i32 + dr).clamp(0, height as i32 - 1) as usize;
                    let c = (col as i32 + dc).clamp(0, width as i32 - 1) as usize;
                    window[index] = pixels[r * width + c];
                    index += 1;
                }
            }
            window.sort_by(|a, b| a.total_cmp(b));
            out.push(window[4]);
        }
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::median_filter;

    #[test]
    fn reference_cases() {
        let impulse = [0.0,0.0,0.0,0.0,16.0,0.0,0.0,0.0,0.0];
        assert_eq!(median_filter(&impulse,3,3),
                   Ok(vec![0.0;9]));
        assert_eq!(median_filter(&[0.0,0.0,8.0],1,3),Ok(vec![0.0,0.0,8.0]));
        assert_eq!(median_filter(&[7.0],1,1),Ok(vec![7.0]));
        assert_eq!(median_filter(&[5.0;6],2,3),Ok(vec![5.0;6]));
        assert_eq!(median_filter(&[0.0,0.0,0.0,0.0,9.0,9.0,0.0,9.0,9.0],3,3),Ok(vec![0.0,0.0,0.0,0.0,0.0,9.0,0.0,9.0,9.0]));
    }
    #[test]
    fn invalid_cases() {
        assert!(median_filter(&[],0,0).is_err());
        assert!(median_filter(&[0.0;4],0,2).is_err());
        assert!(median_filter(&[0.0;4],2,65).is_err());
        assert!(median_filter(&[0.0;3],2,2).is_err());
        assert!(median_filter(&[0.0,0.0,f64::NAN,0.0],2,2).is_err());
        assert!(median_filter(&[0.0,0.0,f64::INFINITY,0.0],2,2).is_err());
        assert!(median_filter(&[0.0,0.0,1_000_001.0,0.0],2,2).is_err());
    }
}
