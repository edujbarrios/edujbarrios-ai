//! Single-channel CNN same-padded 2D cross-correlation.

/// Return H×W result with stride one and zeros outside the image.
pub fn same_conv2d(
    image:&[f64],height:usize,width:usize,
    kernel:&[f64],kh:usize,kw:usize,
)->Result<Vec<f64>,&'static str>{
    if !(1..=32).contains(&height) || !(1..=32).contains(&width) ||
        !matches!(kh,1|3|5) || !matches!(kw,1|3|5) ||
        image.len()!=height*width || kernel.len()!=kh*kw ||
        image.iter().chain(kernel.iter()).any(|x|!x.is_finite() || x.abs()>1000.0) {
        return Err("invalid image, kernel or dimensions");
    }
    let mut result=vec![0.0;image.len()];
    for row in 0..height {
        for col in 0..width {
            let mut sum=0.0;
            for ky in 0..kh {
                let rr=row as isize+ky as isize-(kh/2) as isize;
                if !(0..height as isize).contains(&rr){continue;}
                for kx in 0..kw {
                    let cc=col as isize+kx as isize-(kw/2) as isize;
                    if (0..width as isize).contains(&cc) {
                        sum+=image[rr as usize*width+cc as usize]*kernel[ky*kw+kx];
                    }
                }
            }
            result[row*width+col]=sum;
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::same_conv2d as conv;
    #[test]
    fn examples(){
        assert_eq!(conv(&[1.,2.,3.,4.,5.,6.,7.,8.,9.],3,3,&[1.;9],3,3),
                   Ok(vec![12.,21.,16.,27.,45.,33.,24.,39.,28.]));
        assert_eq!(conv(&[1.,2.,3.,4.],2,2,&[1.;9],3,3),Ok(vec![10.;4]));
        assert_eq!(conv(&[5.],1,1,&[1.;9],3,3),Ok(vec![5.]));
        assert_eq!(conv(&[1.,2.,3.],1,3,&[1.,2.,3.],1,3),Ok(vec![8.,14.,8.]));
        assert_eq!(conv(&[1.,2.,3.],3,1,&[1.,2.,3.],3,1),Ok(vec![8.,14.,8.]));
        assert_eq!(conv(&[-1.,2.,3.,-4.],2,2,&[-2.],1,1),Ok(vec![2.,-4.,-6.,8.]));
        assert_eq!(conv(&[1.,2.,3.,4.],2,2,&[0.],1,1),Ok(vec![0.;4]));
        assert_eq!(conv(&[10.],1,1,&[0.;25],5,5),Ok(vec![0.]));
        assert_eq!(conv(&[1000.],1,1,&[1000.],1,1),Ok(vec![1_000_000.]));
    }
    #[test]
    fn maximum(){
        assert_eq!(conv(&[1.;1024],32,32,&[1.],1,1),Ok(vec![1.;1024]));
        assert_eq!(conv(&[0.;1024],32,32,&[1.;25],5,5),Ok(vec![0.;1024]));
    }
    #[test]
    fn invalid(){
        assert!(conv(&[1.],0,1,&[1.],1,1).is_err());
        assert!(conv(&[1.],1,33,&[1.],1,1).is_err());
        assert!(conv(&[1.],1,1,&[1.],2,1).is_err());
        assert!(conv(&[1.],1,1,&[1.],1,0).is_err());
        assert!(conv(&[],1,1,&[1.],1,1).is_err());
        assert!(conv(&[1.],1,1,&[],1,1).is_err());
        assert!(conv(&[f64::NAN],1,1,&[1.],1,1).is_err());
        assert!(conv(&[1.],1,1,&[f64::INFINITY],1,1).is_err());
        assert!(conv(&[1001.],1,1,&[1.],1,1).is_err());
    }
}
