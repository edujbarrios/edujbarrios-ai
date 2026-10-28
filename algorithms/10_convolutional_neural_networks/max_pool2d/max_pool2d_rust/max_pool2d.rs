//! Fixed 2x2 max pooling, stride 2, no padding.
pub fn max_pool2d(input: &[f64], height: usize, width: usize)
    -> Result<Vec<f64>, &'static str> {
    if !(2..=32).contains(&height) || !(2..=32).contains(&width) ||
       input.len()!=height*width ||
       input.iter().any(|x|!x.is_finite() || x.abs()>1000.0) {
        return Err("invalid tensor dimensions or values");
    }
    let mut result=Vec::with_capacity((height/2)*(width/2));
    for r in 0..height/2 {
        for c in 0..width/2 {
            let at=(2*r)*width+2*c;
            let best=input[at].max(input[at+1])
                .max(input[at+width]).max(input[at+width+1]);
            result.push(best);
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::max_pool2d as pool;
    #[test]
    fn reference() {
        let a:Vec<f64>=(1..=16).map(|i|i as f64).collect();
        assert_eq!(pool(&a,4,4),Ok(vec![6.0,8.0,14.0,16.0]));
        let b:Vec<f64>=(1..=9).map(|i|i as f64).collect();
        assert_eq!(pool(&b,3,3),Ok(vec![5.0]));
        assert_eq!(pool(&[-4.0,-2.0,-1.0,-3.0],2,2),Ok(vec![-1.0]));
    }
    #[test]
    fn invalid() {
        assert!(pool(&[1.0;4],1,2).is_err());
        assert!(pool(&[1.0;4],2,1).is_err());
        assert!(pool(&[1.0;4],33,2).is_err());
        assert!(pool(&[1.0;3],2,2).is_err());
        assert!(pool(&[f64::NAN;4],2,2).is_err());
        assert!(pool(&[1001.0;4],2,2).is_err());
    }
}
