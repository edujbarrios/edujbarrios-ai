//! Root-mean-square normalization for one LLM token feature vector.
pub fn rms_normalization(x: &[f64], gain: &[f64], epsilon: f64)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=1024).contains(&x.len()) || gain.len()!=x.len() ||
       !epsilon.is_finite() || !(1e-8..=1.0).contains(&epsilon) ||
       x.iter().chain(gain.iter()).any(|v|!v.is_finite() || v.abs()>1000.0) {
        return Err("invalid features, gains or epsilon");
    }
    let sum_sq:f64=x.iter().map(|v|v*v).sum();
    let inv=1.0/(sum_sq/x.len() as f64+epsilon).sqrt();
    Ok(x.iter().zip(gain.iter()).map(|(v,g)|v*inv*g).collect())
}

#[cfg(test)]
mod tests {
    use super::rms_normalization as rms;
    #[test]
    fn reference() {
        let actual=rms(&[1.0,1.0],&[1.0,2.0],1.0).unwrap();
        assert!((actual[0]-1.0/2.0f64.sqrt()).abs()<1e-12);
        assert!((actual[1]-2.0/2.0f64.sqrt()).abs()<1e-12);
        assert_eq!(rms(&[0.0,0.0],&[1.0,2.0],1e-8),Ok(vec![0.0,0.0]));
        assert!((rms(&[3.0,4.0],&[1.0,1.0],1e-8).unwrap()[0] -
            3.0/(12.5f64+1e-8).sqrt()).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        assert!(rms(&[],&[],1.0).is_err());
        assert!(rms(&[1.0,1.0],&[1.0],1.0).is_err());
        assert!(rms(&[1.0],&[1.0],0.0).is_err());
        assert!(rms(&[1.0],&[1.0],1e-9).is_err());
        assert!(rms(&[1.0],&[1.0],2.0).is_err());
        assert!(rms(&[1.0],&[1.0],f64::NAN).is_err());
        assert!(rms(&[f64::NAN],&[1.0],1.0).is_err());
        assert!(rms(&[1.0],&[1001.0],1.0).is_err());
    }
}
