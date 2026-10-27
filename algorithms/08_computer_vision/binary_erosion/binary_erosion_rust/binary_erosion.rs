//! Binary 3x3 morphological erosion with zero-padded exterior.

/// Compute a row-major eroded 0/1 mask.
pub fn binary_erosion(pixels: &[u8], height: usize, width: usize)
    -> Result<Vec<u8>, &'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width) ||
        pixels.len()!=height*width || pixels.iter().any(|&x| x>1) {
        return Err("invalid binary mask or dimensions");
    }
    let mut result=vec![0_u8;height*width];
    for r in 0..height {
        for c in 0..width {
            let mut hit=1_u8;
            for dr in -1_isize..=1 {
                for dc in -1_isize..=1 {
                    let y=r as isize+dr;
                    let x=c as isize+dc;
                    if y<0 || y>=height as isize || x<0 || x>=width as isize ||
                        pixels[y as usize*width+x as usize]==0 {
                        hit=0;
                    }
                }
            }
            result[r*width+c]=hit;
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::binary_erosion as erode;
    #[test]
    fn reference() {
        assert_eq!(erode(&[1;9],3,3),Ok(vec![0,0,0,0,1,0,0,0,0]));
        assert_eq!(erode(&[0;9],3,3),Ok(vec![0;9]));
        assert_eq!(erode(&[1,1,1,1,0,1,1,1,1],3,3),Ok(vec![0;9]));
        assert_eq!(erode(&[1,1,1],1,3),Ok(vec![0;3]));
        assert_eq!(erode(&[1],1,1),Ok(vec![0]));
    }
    #[test]
    fn invalid() {
        let a=[1;9];
        assert!(erode(&a,0,3).is_err());
        assert!(erode(&a,3,0).is_err());
        assert!(erode(&a,65,3).is_err());
        assert!(erode(&a,3,65).is_err());
        assert!(erode(&a[..8],3,3).is_err());
        assert!(erode(&[2],1,1).is_err());
    }
}
