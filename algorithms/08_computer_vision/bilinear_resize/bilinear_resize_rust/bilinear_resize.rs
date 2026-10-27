//! Grayscale image resizing by half-pixel-center bilinear interpolation.

/// Resize flat row-major grayscale pixels, with edge-clamped coordinates.
pub fn bilinear_resize(pixels: &[f64], h: usize, w: usize,
                       out_h: usize, out_w: usize)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=64).contains(&h) || !(1..=64).contains(&w) ||
       !(1..=64).contains(&out_h) || !(1..=64).contains(&out_w) ||
       pixels.len() != h*w ||
       pixels.iter().any(|x| !x.is_finite() || x.abs() > 1_000_000.0) {
        return Err("invalid dimensions or pixel data");
    }
    fn coordinate(index: usize, size: usize, destination: usize) -> f64 {
        ((index as f64 + 0.5) * size as f64 / destination as f64 - 0.5)
            .clamp(0.0, (size - 1) as f64)
    }
    let mut output = Vec::with_capacity(out_h * out_w);
    for r in 0..out_h {
        let y = coordinate(r,h,out_h);
        let y0 = y.floor() as usize;
        let y1 = (y0+1).min(h-1);
        let wy = y - y0 as f64;
        for c in 0..out_w {
            let x = coordinate(c,w,out_w);
            let x0 = x.floor() as usize;
            let x1 = (x0+1).min(w-1);
            let wx = x - x0 as f64;
            let a = (1.0-wx)*pixels[y0*w+x0] + wx*pixels[y0*w+x1];
            let b = (1.0-wx)*pixels[y1*w+x0] + wx*pixels[y1*w+x1];
            output.push((1.0-wy)*a + wy*b);
        }
    }
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::bilinear_resize as resize;
    #[test]
    fn reference() {
        let image=[0.0,10.0,20.0,30.0];
        assert_eq!(resize(&image,2,2,3,3),
                   Ok(vec![0.0,5.0,10.0,10.0,15.0,20.0,20.0,25.0,30.0]));
        assert_eq!(resize(&image,2,2,2,2),Ok(image.to_vec()));
        assert_eq!(resize(&[7.0],1,1,3,2),Ok(vec![7.0;6]));
        assert_eq!(resize(&[0.0,10.0],1,2,1,3),Ok(vec![0.0,5.0,10.0]));
        assert_eq!(resize(&[0.0,10.0],2,1,3,1),Ok(vec![0.0,5.0,10.0]));
    }
    #[test]
    fn invalid() {
        let image=[0.0,10.0,20.0,30.0];
        assert!(resize(&image,0,2,3,3).is_err());
        assert!(resize(&image,2,2,0,3).is_err());
        assert!(resize(&image,2,2,65,3).is_err());
        assert!(resize(&image,65,2,3,3).is_err());
        assert!(resize(&[0.0,10.0,20.0],2,2,3,3).is_err());
        assert!(resize(&[0.0,f64::NAN,20.0,30.0],2,2,3,3).is_err());
        assert!(resize(&[0.0,1_000_001.0,20.0,30.0],2,2,3,3).is_err());
    }
}
