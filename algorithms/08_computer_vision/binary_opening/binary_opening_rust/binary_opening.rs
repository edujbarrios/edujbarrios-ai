//! Binary opening: 3x3 zero-padded erosion followed by dilation.
/// Return a new binary row-major image or Err for invalid data.
pub fn binary_opening(image: &[u8], width: usize, height: usize) -> Result<Vec<u8>, &'static str> {
    if width==0 || height==0 || width>64 || height>64 ||
       image.len()!=width*height || image.iter().any(|&x| x>1) {
        return Err("invalid image dimensions or pixels");
    }
    let mut eroded = vec![0u8; image.len()];
    for y in 0..height {
        for x in 0..width {
            let mut keep=1u8;
            for dy in -1isize..=1 {
                for dx in -1isize..=1 {
                    let ny=y as isize+dy;
                    let nx=x as isize+dx;
                    if ny<0 || nx<0 || ny>=height as isize || nx>=width as isize ||
                        image[ny as usize*width+nx as usize]==0 { keep=0; }
                }
            }
            eroded[y*width+x]=keep;
        }
    }
    let mut output=vec![0u8; image.len()];
    for y in 0..height {
        for x in 0..width {
            let mut keep=0u8;
            for dy in -1isize..=1 {
                for dx in -1isize..=1 {
                    let ny=y as isize+dy;
                    let nx=x as isize+dx;
                    if ny>=0 && nx>=0 && ny<height as isize && nx<width as isize &&
                       eroded[ny as usize*width+nx as usize]==1 { keep=1; }
                }
            }
            output[y*width+x]=keep;
        }
    }
    Ok(output)
}
#[cfg(test)]
mod tests {
    use super::binary_opening as opening;
    #[test] fn noise_and_block() {
        let mut image=vec![0u8;49];
        let mut expected=vec![0u8;49];
        for y in 2..=4 { for x in 2..=4 {
            image[y*7+x]=1;
            expected[y*7+x]=1;
        }}
        image[0]=1;
        assert_eq!(opening(&image,7,7),Ok(expected));
    }
    #[test] fn boundaries_and_invalid() {
        assert_eq!(opening(&[1],1,1),Ok(vec![0]));
        assert_eq!(opening(&[0],1,1),Ok(vec![0]));
        assert!(opening(&[],1,1).is_err());
        assert!(opening(&[2],1,1).is_err());
        assert!(opening(&[1],0,1).is_err());
        assert!(opening(&[1],65,1).is_err());
        assert!(opening(&[1],2,1).is_err());
    }
}
