//! CHW depthwise cross-correlation with same zero padding.
pub fn depthwise_conv2d(input: &[f64], channels: usize, height: usize,
                         width: usize, kernels: &[f64], kh: usize, kw: usize)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=8).contains(&channels) || !(1..=16).contains(&height) ||
       !(1..=16).contains(&width) || !(1..=5).contains(&kh) ||
       !(1..=5).contains(&kw) || kh%2==0 || kw%2==0 ||
       input.len()!=channels*height*width || kernels.len()!=channels*kh*kw ||
       input.iter().chain(kernels.iter()).any(|x|!x.is_finite() || x.abs()>1000.0) {
        return Err("invalid CHW tensor or depthwise kernels");
    }
    let mut result=vec![0.0;channels*height*width];
    for c in 0..channels {
        for r in 0..height {
            for s in 0..width {
                let mut total=0.0;
                for i in 0..kh {
                    for j in 0..kw {
                        let y=r as isize+i as isize-(kh/2) as isize;
                        let x=s as isize+j as isize-(kw/2) as isize;
                        if y>=0 && y<height as isize && x>=0 && x<width as isize {
                            total+=input[c*height*width+y as usize*width+x as usize]*
                                   kernels[c*kh*kw+i*kw+j];
                        }
                    }
                }
                result[c*height*width+r*width+s]=total;
            }
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::depthwise_conv2d as conv;
    #[test]
    fn reference() {
        let x=[1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0];
        let mut kernels=[0.0;18];
        kernels[4]=1.0;
        kernels[13]=-1.0;
        assert_eq!(conv(&x,2,2,2,&kernels,3,3),
                   Ok(vec![1.0,2.0,3.0,4.0,-5.0,-6.0,-7.0,-8.0]));
        assert_eq!(conv(&x[..4],1,2,2,&[1.0;9],3,3),Ok(vec![10.0;4]));
        assert_eq!(conv(&[1.0],1,1,1,&[1.0;9],3,3),Ok(vec![1.0]));
    }
    #[test]
    fn invalid() {
        let x=[1.0;4];
        assert!(conv(&x,0,2,2,&[1.0;9],3,3).is_err());
        assert!(conv(&x,1,0,2,&[1.0;9],3,3).is_err());
        assert!(conv(&x,1,2,2,&[1.0;9],2,3).is_err());
        assert!(conv(&x,1,2,2,&[1.0;9],3,0).is_err());
        assert!(conv(&x[..3],1,2,2,&[1.0;9],3,3).is_err());
        assert!(conv(&x,1,2,2,&[f64::NAN],1,1).is_err());
        assert!(conv(&x,1,2,2,&[1001.0],1,1).is_err());
    }
}
