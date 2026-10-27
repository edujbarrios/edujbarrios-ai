//! 3x3 binary image dilation with zero padding.

/// Dilate a flat H*W binary mask, preserving its dimensions.
pub fn binary_dilation(pixels: &[u8], height: usize, width: usize)
    -> Result<Vec<u8>, &'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width) ||
       pixels.len()!=height*width || pixels.iter().any(|&v|v>1) {
        return Err("invalid binary image or dimensions");
    }
    let mut result=vec![0_u8;height*width];
    for r in 0..height {
        for c in 0..width {
            for dr in -1_isize..=1 {
                for dc in -1_isize..=1 {
                    let y=r as isize+dr;
                    let x=c as isize+dc;
                    if y>=0 && y<height as isize && x>=0 &&
                       x<width as isize && pixels[y as usize*width+x as usize]==1 {
                        result[r*width+c]=1;
                    }
                }
            }
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::binary_dilation as dilate;
    #[test]
    fn reference() {
        assert_eq!(dilate(&[0,0,0,0,1,0,0,0,0],3,3),Ok(vec![1;9]));
        assert_eq!(dilate(&[1,0,0,0,0,0,0,0,0],3,3),
                   Ok(vec![1,1,0,1,1,0,0,0,0]));
        assert_eq!(dilate(&[0,1,0],1,3),Ok(vec![1,1,1]));
        assert_eq!(dilate(&[0],1,1),Ok(vec![0]));
        assert_eq!(dilate(&[1],1,1),Ok(vec![1]));
    }
    #[test]
    fn invalid() {
        let mask=[0,0,0,0,1,0,0,0,0];
        assert!(dilate(&mask,0,3).is_err());
        assert!(dilate(&mask,3,0).is_err());
        assert!(dilate(&mask,65,3).is_err());
        assert!(dilate(&mask,3,65).is_err());
        assert!(dilate(&mask[..8],3,3).is_err());
        assert!(dilate(&[0,2,0],1,3).is_err());
    }
}
