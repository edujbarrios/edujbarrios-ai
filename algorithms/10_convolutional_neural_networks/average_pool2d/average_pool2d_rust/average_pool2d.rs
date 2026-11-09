//! Disjoint 2x2 average pooling with stride two.

/// Return floor(H/2)*floor(W/2) row-major local means.
pub fn average_pool2d(image:&[f64],height:usize,width:usize)
    ->Result<Vec<f64>,&'static str>{
    if !(2..=32).contains(&height) || !(2..=32).contains(&width) ||
        image.len()!=height*width ||
        image.iter().any(|x|!x.is_finite() || x.abs()>1000.0) {
        return Err("invalid image dimensions or values");
    }
    let (oh,ow)=(height/2,width/2);
    let mut result=vec![0.0;oh*ow];
    for r in 0..oh {
        for c in 0..ow {
            let i=2*r*width+2*c;
            result[r*ow+c]=(image[i]+image[i+1]+image[i+width]+image[i+width+1])/4.0;
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::average_pool2d as pool;
    #[test]
    fn examples() {
        let x:[f64;16]=std::array::from_fn(|i|i as f64+1.0);
        assert_eq!(pool(&x,4,4),Ok(vec![3.5,5.5,11.5,13.5]));
        let odd:[f64;15]=std::array::from_fn(|i|i as f64+1.0);
        assert_eq!(pool(&odd,3,5),Ok(vec![4.,6.]));
        assert_eq!(pool(&[1.,2.,3.,4.],2,2),Ok(vec![2.5]));
        assert_eq!(pool(&[-4.,-2.,-6.,-8.],2,2),Ok(vec![-5.]));
        assert_eq!(pool(&[0.;4],2,2),Ok(vec![0.]));
        assert_eq!(pool(&[1000.;4],2,2),Ok(vec![1000.]));
        assert_eq!(pool(&[-1000.;4],2,2),Ok(vec![-1000.]));
        assert_eq!(pool(&[1.,2.,3.,4.,5.,6.],2,3),Ok(vec![3.]));
        assert_eq!(pool(&[1.,2.,3.,4.,5.,6.],3,2),Ok(vec![2.5]));
    }
    #[test]
    fn maximum() {
        assert_eq!(pool(&[1.;1024],32,32),Ok(vec![1.;256]));
        let checker:Vec<f64>=(0..32).flat_map(|r|
            (0..32).map(move |c|if (r+c)%2==0 {1.} else {-1.})).collect();
        assert_eq!(pool(&checker,32,32),Ok(vec![0.;256]));
    }
    #[test]
    fn invalid() {
        assert!(pool(&[1.],1,2).is_err());
        assert!(pool(&[1.],2,1).is_err());
        assert!(pool(&[1.],33,2).is_err());
        assert!(pool(&[1.],2,33).is_err());
        assert!(pool(&[],2,2).is_err());
        assert!(pool(&[1.;3],2,2).is_err());
        assert!(pool(&[1.,2.,3.,f64::NAN],2,2).is_err());
        assert!(pool(&[1.,2.,3.,f64::INFINITY],2,2).is_err());
        assert!(pool(&[1.,2.,3.,1001.],2,2).is_err());
    }
}
