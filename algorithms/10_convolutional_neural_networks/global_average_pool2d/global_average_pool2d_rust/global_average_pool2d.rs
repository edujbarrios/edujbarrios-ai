//! Channel-first global average pooling for CNN feature maps.
pub fn global_average_pool2d(input: &[f64], channels: usize,
                              height: usize, width: usize)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=16).contains(&channels) || !(1..=32).contains(&height) ||
       !(1..=32).contains(&width) || input.len()!=channels*height*width ||
       input.iter().any(|x|!x.is_finite() || x.abs()>1000.0) {
        return Err("invalid CHW tensor");
    }
    let spatial=height*width;
    Ok((0..channels).map(|c|
        input[c*spatial..(c+1)*spatial].iter().sum::<f64>()/spatial as f64)
        .collect())
}

#[cfg(test)]
mod tests {
    use super::global_average_pool2d as gap;
    #[test]
    fn reference() {
        assert_eq!(gap(&[1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0],2,2,2),
                   Ok(vec![2.5,6.5]));
        assert_eq!(gap(&[-3.0,-1.0],1,1,2),Ok(vec![-2.0]));
        assert_eq!(gap(&[-3.0],1,1,1),Ok(vec![-3.0]));
    }
    #[test]
    fn invalid() {
        assert!(gap(&[1.0;4],0,2,2).is_err());
        assert!(gap(&[1.0;4],17,2,2).is_err());
        assert!(gap(&[1.0;4],1,0,2).is_err());
        assert!(gap(&[1.0;4],1,2,33).is_err());
        assert!(gap(&[1.0;3],1,2,2).is_err());
        assert!(gap(&[1.0,f64::NAN,3.0,4.0],1,2,2).is_err());
        assert!(gap(&[1.0,1001.0,3.0,4.0],1,2,2).is_err());
    }
}
