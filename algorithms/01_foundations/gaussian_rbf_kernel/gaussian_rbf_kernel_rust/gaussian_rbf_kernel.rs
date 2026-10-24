//! Dense symmetric Gaussian RBF kernel Gram matrix.

/// Return row-major K[n*n] from bounded row-major X[n*d].
pub fn gaussian_rbf_kernel(points:&[f64],n:usize,d:usize,sigma:f64)
    ->Result<Vec<f64>,&'static str>{
    if !(1..=128).contains(&n) || !(1..=32).contains(&d) ||
       n.checked_mul(d).ok_or("dimension overflow")?!=points.len() ||
       !sigma.is_finite() || !(1e-6..=1e6).contains(&sigma) ||
       points.iter().any(|x|!x.is_finite() || x.abs()>1e6){
        return Err("invalid finite points or kernel scale");
    }
    let mut kernel=vec![0.0_f64;n*n];
    for i in 0..n {
        for j in i..n {
            let mut distance=0.0_f64;
            for c in 0..d {
                distance=distance.hypot(points[i*d+c]-points[j*d+c]);
            }
            let scaled=distance/sigma;
            let value=(-0.5*scaled*scaled).exp();
            if !distance.is_finite() || !scaled.is_finite() ||
               !value.is_finite() || !(0.0..=1.0).contains(&value){
                return Err("kernel computation overflow");
            }
            kernel[i*n+j]=value;
            kernel[j*n+i]=value;
        }
    }
    Ok(kernel)
}

#[cfg(test)]
mod tests {
    use super::gaussian_rbf_kernel as rbf;
    #[test]
    fn analytic_and_duplicates(){
        let k=rbf(&[0.0,1.0,2.0],3,1,1.0).unwrap();
        assert_eq!([k[0],k[4],k[8]],[1.0,1.0,1.0]);
        assert!((k[1]-(-0.5_f64).exp()).abs()<1e-12);
        assert!((k[2]-(-2.0_f64).exp()).abs()<1e-12);
        assert_eq!(k[1],k[3]);
        assert_eq!(rbf(&[2.0,2.0],2,1,1.0),Ok(vec![1.0;4]));
        assert_eq!(rbf(&[0.0],1,1,1.0),Ok(vec![1.0]));
        assert!((rbf(&[0.0,1.0],2,1,2.0).unwrap()[1]-(-0.125_f64).exp()).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        assert!(rbf(&[],0,1,1.0).is_err());
        assert!(rbf(&[1.0],1,0,1.0).is_err());
        assert!(rbf(&[1.0],1,1,0.0).is_err());
        assert!(rbf(&[1.0],1,1,1e-7).is_err());
        assert!(rbf(&[1.0],1,1,1e7).is_err());
        assert!(rbf(&[f64::NAN],1,1,1.0).is_err());
        assert!(rbf(&[1000001.0],1,1,1.0).is_err());
    }
}
