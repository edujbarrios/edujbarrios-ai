//! 3x3 dilation followed by zero-padded erosion of an aligned binary image.
/// Return a binary row-major image, or Err on invalid dimensions or pixels.
pub fn binary_closing(image: &[u8], width: usize, height: usize) -> Result<Vec<u8>, &'static str> {
    if width == 0 || height == 0 || width > 64 || height > 64 ||
       image.len() != width * height || image.iter().any(|&v| v > 1) {
        return Err("invalid image dimensions or pixels");
    }
    let mut dilated = vec![0u8; image.len()];
    for y in 0..height {
        for x in 0..width {
            let mut on = 0u8;
            for dy in -1isize..=1 {
                for dx in -1isize..=1 {
                    let yy = y as isize + dy;
                    let xx = x as isize + dx;
                    if yy >= 0 && xx >= 0 && yy < height as isize && xx < width as isize &&
                       image[yy as usize * width + xx as usize] == 1 {
                        on = 1;
                    }
                }
            }
            dilated[y * width + x] = on;
        }
    }
    let mut output = vec![0u8; image.len()];
    for y in 0..height {
        for x in 0..width {
            let mut on = 1u8;
            for dy in -1isize..=1 {
                for dx in -1isize..=1 {
                    let yy = y as isize + dy;
                    let xx = x as isize + dx;
                    if yy < 0 || xx < 0 || yy >= height as isize || xx >= width as isize ||
                       dilated[yy as usize * width + xx as usize] == 0 {
                        on = 0;
                    }
                }
            }
            output[y * width + x] = on;
        }
    }
    Ok(output)
}
#[cfg(test)]
mod tests {
    use super::binary_closing as close;
    #[test] fn fill_hole() {
        let mut image = vec![0u8; 49];
        let mut expected = vec![0u8; 49];
        for y in 2..=4 { for x in 2..=4 {
            image[y*7+x] = 1;
            expected[y*7+x] = 1;
        }}
        image[3*7+3] = 0;
        assert_eq!(close(&image, 7, 7), Ok(expected));
    }
    #[test] fn invalid_and_border() {
        assert_eq!(close(&[1], 1, 1), Ok(vec![0]));
        assert_eq!(close(&[0], 1, 1), Ok(vec![0]));
        assert!(close(&[], 1, 1).is_err());
        assert!(close(&[2], 1, 1).is_err());
        assert!(close(&[1], 0, 1).is_err());
        assert!(close(&[1], 65, 1).is_err());
    }
}
