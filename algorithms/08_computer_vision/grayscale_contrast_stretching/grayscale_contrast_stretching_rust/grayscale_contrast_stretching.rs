//! Whole-image min-max grayscale contrast stretching with half-up rounding.

/// Map occupied 8-bit intensity range linearly to [0,255].
pub fn grayscale_contrast_stretching(
    pixels: &[u8], height: usize, width: usize
) -> Result<Vec<u8>, &'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width)
        || pixels.len()!=height*width {
        return Err("invalid image dimensions or pixel count");
    }
    let lo = *pixels.iter().min().unwrap();
    let hi = *pixels.iter().max().unwrap();
    let span = u32::from(hi-lo);
    if span == 0 { return Ok(pixels.to_vec()); }
    Ok(pixels.iter().map(|&v| {
        ((510u32*u32::from(v-lo)+span)/(2*span)) as u8
    }).collect())
}

#[cfg(test)]
mod tests {
    use super::grayscale_contrast_stretching as stretch;
    #[test]
    fn examples() {
        assert_eq!(stretch(&[10,10,15,20],2,2),Ok(vec![0,0,128,255]));
        assert_eq!(stretch(&[5,6,7,8],1,4),Ok(vec![0,85,170,255]));
        assert_eq!(stretch(&[99,99],1,2),Ok(vec![99,99]));
        assert_eq!(stretch(&[0,255],1,2),Ok(vec![0,255]));
        assert_eq!(stretch(&[5],1,1),Ok(vec![5]));
        assert_eq!(stretch(&[0,0,0,0],2,2),Ok(vec![0,0,0,0]));
        assert_eq!(stretch(&[0,1,2],1,3),Ok(vec![0,128,255]));
    }
    #[test]
    fn maximum() {
        let mut image=vec![50u8;4096];image[4095]=100;
        let out=stretch(&image,64,64).unwrap();
        assert!(out[..4095].iter().all(|&x|x==0));
        assert_eq!(out[4095],255);
        assert_eq!(stretch(&[120u8;4096],64,64),Ok(vec![120;4096]));
    }
    #[test]
    fn invalid() {
        assert!(stretch(&[0],0,1).is_err());
        assert!(stretch(&[0],1,0).is_err());
        assert!(stretch(&[0],65,1).is_err());
        assert!(stretch(&[0],1,65).is_err());
        assert!(stretch(&[],1,1).is_err());
        assert!(stretch(&[0,1],1,1).is_err());
    }
}
