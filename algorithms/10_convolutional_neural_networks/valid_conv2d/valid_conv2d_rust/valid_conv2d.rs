//! Valid 2D single-channel cross-correlation for CNN reference tests.
pub fn valid_conv2d(image: &[f64], height: usize, width: usize,
                    kernel: &[f64], kh: usize, kw: usize)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=32).contains(&height) || !(1..=32).contains(&width) ||
       !(1..=8).contains(&kh) || !(1..=8).contains(&kw) ||
       kh>height || kw>width || image.len()!=height*width ||
       kernel.len()!=kh*kw ||
       image.iter().chain(kernel.iter()).any(|x| !x.is_finite() || x.abs()>1000.0) {
        return Err("invalid image or kernel");
    }
    let oh=height-kh+1;
    let ow=width-kw+1;
    let mut result=Vec::with_capacity(oh*ow);
    for r in 0..oh {
        for c in 0..ow {
            let mut value=0.0;
            for y in 0..kh {
                for x in 0..kw {
                    value+=image[(r+y)*width+c+x]*kernel[y*kw+x];
                }
            }
            result.push(value);
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::valid_conv2d as conv;
    #[test]
    fn reference() {
        let im=[1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0];
        assert_eq!(conv(&im,3,3,&[1.0,0.0,0.0,-1.0],2,2),Ok(vec![-4.0;4]));
        assert_eq!(conv(&im,3,3,&[-2.0],1,1),
                   Ok(im.iter().map(|x|-2.0*x).collect()));
        assert_eq!(conv(&[5.0],1,1,&[3.0],1,1),Ok(vec![15.0]));
    }
    #[test]
    fn invalid() {
        let im=[1.0;9];
        assert!(conv(&im,0,3,&[1.0],1,1).is_err());
        assert!(conv(&im,3,0,&[1.0],1,1).is_err());
        assert!(conv(&im,33,3,&[1.0],1,1).is_err());
        assert!(conv(&im,3,3,&[1.0],4,1).is_err());
        assert!(conv(&im,3,3,&[1.0],0,1).is_err());
        assert!(conv(&im[..8],3,3,&[1.0],1,1).is_err());
        assert!(conv(&im,3,3,&[f64::NAN],1,1).is_err());
        assert!(conv(&im,3,3,&[1001.0],1,1).is_err());
    }
}
