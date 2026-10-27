//! Sobel gradient magnitude with replicated-edge boundary conditions.

/// Evaluate 3x3 Sobel kernels on row-major grayscale pixel values.
pub fn sobel_edges(pixels: &[f64], height: usize, width: usize)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width) ||
       pixels.len() != height*width ||
       pixels.iter().any(|x| !x.is_finite() || x.abs() > 1000.0) {
        return Err("invalid dimensions or pixels");
    }
    const KX: [[i32;3];3] = [[-1,0,1],[-2,0,2],[-1,0,1]];
    const KY: [[i32;3];3] = [[-1,-2,-1],[0,0,0],[1,2,1]];
    let mut result = Vec::with_capacity(height*width);
    for row in 0..height {
        for col in 0..width {
            let mut gx = 0.0;
            let mut gy = 0.0;
            for y in 0..3 {
                for x in 0..3 {
                    let r = (row as isize + y as isize - 1)
                        .clamp(0,(height-1) as isize) as usize;
                    let c = (col as isize + x as isize - 1)
                        .clamp(0,(width-1) as isize) as usize;
                    let p = pixels[r*width+c];
                    gx += KX[y][x] as f64 * p;
                    gy += KY[y][x] as f64 * p;
                }
            }
            result.push(gx.hypot(gy));
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::sobel_edges as sobel;
    #[test]
    fn reference() {
        assert_eq!(sobel(&[0.,1.,2.,0.,1.,2.,0.,1.,2.],3,3),
                   Ok(vec![4.,8.,4.,4.,8.,4.,4.,8.,4.]));
        assert_eq!(sobel(&[0.,0.,0.,1.,1.,1.,2.,2.,2.],3,3),
                   Ok(vec![4.,4.,4.,8.,8.,8.,4.,4.,4.]));
        assert_eq!(sobel(&[5.;4],2,2),Ok(vec![0.;4]));
        assert_eq!(sobel(&[7.],1,1),Ok(vec![0.]));
    }
    #[test]
    fn invalid() {
        let x=[0.,1.,2.,0.,1.,2.,0.,1.,2.];
        assert!(sobel(&x,0,3).is_err());
        assert!(sobel(&x,3,0).is_err());
        assert!(sobel(&x,65,3).is_err());
        assert!(sobel(&x,3,65).is_err());
        assert!(sobel(&x[..8],3,3).is_err());
        assert!(sobel(&[1001.],1,1).is_err());
        assert!(sobel(&[f64::NAN],1,1).is_err());
    }
}
