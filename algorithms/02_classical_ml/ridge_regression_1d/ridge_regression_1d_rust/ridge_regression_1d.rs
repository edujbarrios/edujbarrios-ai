//! Fit univariate ridge regression with an unpenalized intercept.

/// Return (intercept,slope) for SSE + lambda*slope^2.
pub fn ridge_regression_1d(x:&[f64],y:&[f64],lambda:f64)
    ->Result<(f64,f64),&'static str>{
    let n=x.len();
    if !(1..=4096).contains(&n) || y.len()!=n ||
       !lambda.is_finite() || !(0.0..=1e12).contains(&lambda) ||
       x.iter().chain(y.iter()).any(|v|!v.is_finite() || v.abs()>1e6) {
        return Err("invalid regression input or penalty");
    }
    let mx=x.iter().map(|v|v/n as f64).sum::<f64>();
    let my=y.iter().map(|v|v/n as f64).sum::<f64>();
    let (mut sxx,mut sxy)=(0.0_f64,0.0_f64);
    for (&a,&b) in x.iter().zip(y.iter()) {
        let dx=a-mx;
        let dy=b-my;
        sxx+=dx*dx;
        sxy+=dx*dy;
    }
    let denom=sxx+lambda;
    let slope=if denom==0.0 {0.0} else {sxy/denom};
    let intercept=my-slope*mx;
    if ![mx,my,sxx,sxy,denom,slope,intercept].iter().all(|v|v.is_finite()){
        return Err("regression arithmetic overflow");
    }
    Ok((intercept,slope))
}

#[cfg(test)]
mod tests {
    use super::ridge_regression_1d as fit;
    #[test]
    fn example_and_singular() {
        let (a,b)=fit(&[0.0,1.0,2.0],&[1.0,3.0,5.0],0.0).unwrap();
        assert!((a-1.0).abs()<1e-12 && (b-2.0).abs()<1e-12);
        let (a,b)=fit(&[0.0,1.0,2.0],&[1.0,3.0,5.0],2.0).unwrap();
        assert!((a-2.0).abs()<1e-12 && (b-1.0).abs()<1e-12);
        assert_eq!(fit(&[2.0,2.0,2.0],&[1.0,2.0,3.0],0.0),Ok((2.0,0.0)));
        assert_eq!(fit(&[0.0],&[1.0],0.0),Ok((1.0,0.0)));
    }
    #[test]
    fn invalid() {
        assert!(fit(&[],&[],0.0).is_err());
        assert!(fit(&[0.0,1.0],&[1.0],0.0).is_err());
        assert!(fit(&[1.0],&[1.0],-1.0).is_err());
        assert!(fit(&[1.0],&[1.0],f64::INFINITY).is_err());
        assert!(fit(&[f64::NAN],&[1.0],0.0).is_err());
        assert!(fit(&[1000001.0],&[1.0],0.0).is_err());
    }
}
