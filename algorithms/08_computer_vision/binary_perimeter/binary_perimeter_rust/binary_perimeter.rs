//! Unit-edge perimeter of binary mask using four-neighbor grid adjacency.

/// Count sides of foreground pixels adjacent to zero or image exterior.
pub fn binary_perimeter(
    pixels: &[u8], height: usize, width: usize,
) -> Result<u32, &'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width)
        || pixels.len() != height * width || pixels.iter().any(|&x| x > 1) {
        return Err("invalid binary mask or dimensions");
    }
    let mut count=0u32;
    for (index,&pixel) in pixels.iter().enumerate() {
        if pixel==0 { continue; }
        let row=index/width;
        let col=index%width;
        if row==0 || pixels[index-width]==0 { count+=1; }
        if row+1==height || pixels[index+width]==0 { count+=1; }
        if col==0 || pixels[index-1]==0 { count+=1; }
        if col+1==width || pixels[index+1]==0 { count+=1; }
    }
    Ok(count)
}

#[cfg(test)]
mod tests {
    use super::binary_perimeter as perimeter;
    #[test]
    fn examples() {
        assert_eq!(perimeter(&[1],1,1),Ok(4));
        assert_eq!(perimeter(&[0],1,1),Ok(0));
        assert_eq!(perimeter(&[1,1,1,1],2,2),Ok(8));
        assert_eq!(perimeter(&[0,0,0,0],2,2),Ok(0));
        assert_eq!(perimeter(&[1,0,1],1,3),Ok(8));
        assert_eq!(perimeter(&[1,1,1],1,3),Ok(8));
        assert_eq!(perimeter(&[1,1,1],3,1),Ok(8));
        assert_eq!(perimeter(&[1,1,1,1,0,1,1,1,1],3,3),Ok(16));
        assert_eq!(perimeter(&[0,0,0,0,1,0,0,0,0],3,3),Ok(4));
        assert_eq!(perimeter(&[1,0,0,1],2,2),Ok(8));
    }
    #[test]
    fn maximum() {
        assert_eq!(perimeter(&[1u8;4096],64,64),Ok(256));
        let checker: Vec<u8>=(0..64).flat_map(|r|
            (0..64).map(move |c|((r+c)%2) as u8)).collect();
        assert_eq!(perimeter(&checker,64,64),Ok(8192));
        assert_eq!(perimeter(&[0u8;4096],64,64),Ok(0));
    }
    #[test]
    fn invalid() {
        assert!(perimeter(&[1],0,1).is_err());
        assert!(perimeter(&[1],1,0).is_err());
        assert!(perimeter(&[1],65,1).is_err());
        assert!(perimeter(&[1],1,65).is_err());
        assert!(perimeter(&[],1,1).is_err());
        assert!(perimeter(&[0,1],1,1).is_err());
        assert!(perimeter(&[2],1,1).is_err());
    }
}
