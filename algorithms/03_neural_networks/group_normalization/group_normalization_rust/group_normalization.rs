//! Group normalization on a channel-major single-example tensor.

/// Normalize C channels across G groups of S spatial values per channel.
pub fn group_normalization(
    x:&[f64],channels:usize,spatial_size:usize,groups:usize,
    gamma:&[f64],beta:&[f64],epsilon:f64,
)->Result<Vec<f64>,&'static str>{
    if !(1..=32).contains(&channels) || !(1..=64).contains(&spatial_size) ||
        groups==0 || groups>channels || channels%groups!=0 ||
        x.len()!=channels*spatial_size || gamma.len()!=channels ||
        beta.len()!=channels || !epsilon.is_finite() ||
        epsilon<=0.0 || epsilon>1.0 ||
        x.iter().chain(gamma.iter()).chain(beta.iter())
            .any(|v|!v.is_finite() || v.abs()>1000.0) {
        return Err("invalid normalization dimensions, epsilon or tensor values");
    }
    let group_size=(channels/groups)*spatial_size;
    let mut out=vec![0.0;x.len()];
    for g in 0..groups {
        let start=g*group_size;
        let end=start+group_size;
        let mut mean=0.0f64;
        for &value in &x[start..end]{mean+=value/group_size as f64;}
        let mut variance=0.0f64;
        for &value in &x[start..end] {
            let d=value-mean;
            variance+=(d*d)/group_size as f64;
        }
        let denom=(variance+epsilon).sqrt();
        if !denom.is_finite() || denom<=0.0 {
            return Err("nonfinite normalization denominator");
        }
        for i in start..end {
            let c=i/spatial_size;
            let value=gamma[c]*((x[i]-mean)/denom)+beta[c];
            if !value.is_finite(){return Err("nonfinite group normalization");}
            out[i]=value;
        }
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::group_normalization as norm;
    fn near(got:&[f64],want:&[f64]){
        assert_eq!(got.len(),want.len());
        for (&a,&b) in got.iter().zip(want.iter()){
            assert!((a-b).abs()<1e-10,"got {a}, expected {b}");
        }
    }
    #[test]
    fn examples(){
        let x=[1.,2.,3.,4.,5.,6.,7.,8.];
        let got=norm(&x,4,2,2,&[1.;4],&[0.;4],0.25).unwrap();
        let want:Vec<f64>=(0..8)
            .map(|i|((i%4) as f64-1.5)/1.5f64.sqrt()).collect();
        near(&got,&want);
        assert_eq!(norm(&[10.,20.],2,1,2,&[3.,4.],&[5.,6.],1e-5),
                   Ok(vec![5.,6.]));
        assert_eq!(norm(&[7.;4],2,2,2,&[3.,4.],&[5.,6.],1e-5),
                   Ok(vec![5.,5.,6.,6.]));
        assert_eq!(norm(&[9.],1,1,1,&[2.],&[3.],1e-5),Ok(vec![3.]));
        let result=norm(&[1.,3.,5.,7.],2,2,1,&[2.,0.5],&[1.,-1.],1.).unwrap();
        let want=[1.-6./6f64.sqrt(),1.-2./6f64.sqrt(),
                  -1.+0.5/6f64.sqrt(),-1.+1.5/6f64.sqrt()];
        near(&result,&want);
    }
    #[test]
    fn maximum(){
        assert_eq!(norm(&[0.;2048],32,64,8,&[1.;32],&[0.;32],1e-5),
                   Ok(vec![0.;2048]));
        assert_eq!(norm(&[0.;2048],32,64,32,&[1.;32],&[2.;32],1e-5),
                   Ok(vec![2.;2048]));
        assert_eq!(norm(&[0.;2048],32,64,1,&[1.;32],&[3.;32],1e-5),
                   Ok(vec![3.;2048]));
    }
    #[test]
    fn invalid(){
        assert!(norm(&[1.],0,1,1,&[1.],&[0.],1e-5).is_err());
        assert!(norm(&[1.],33,1,1,&[1.],&[0.],1e-5).is_err());
        assert!(norm(&[1.],1,0,1,&[1.],&[0.],1e-5).is_err());
        assert!(norm(&[1.],1,65,1,&[1.],&[0.],1e-5).is_err());
        assert!(norm(&[1.],1,1,0,&[1.],&[0.],1e-5).is_err());
        assert!(norm(&[1.,2.,3.],3,1,2,&[1.;3],&[0.;3],1e-5).is_err());
        assert!(norm(&[],1,1,1,&[1.],&[0.],1e-5).is_err());
        assert!(norm(&[1.],1,1,1,&[],&[0.],1e-5).is_err());
        assert!(norm(&[1.],1,1,1,&[1.],&[],1e-5).is_err());
        assert!(norm(&[f64::NAN],1,1,1,&[1.],&[0.],1e-5).is_err());
        assert!(norm(&[1.],1,1,1,&[f64::INFINITY],&[0.],1e-5).is_err());
        assert!(norm(&[1001.],1,1,1,&[1.],&[0.],1e-5).is_err());
        assert!(norm(&[1.],1,1,1,&[1.],&[0.],0.).is_err());
        assert!(norm(&[1.],1,1,1,&[1.],&[0.],2.).is_err());
        assert!(norm(&[1.],1,1,1,&[1.],&[0.],f64::NAN).is_err());
    }
}
