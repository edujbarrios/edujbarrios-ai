//! Discrete base-2 KL divergence, with missing-support +infinity results.

fn valid_distribution(p:&[f64])->bool{
    (1..=4096).contains(&p.len()) &&
    p.iter().all(|v|v.is_finite() && (0.0..=1.0).contains(v)) &&
    (p.iter().sum::<f64>()-1.0).abs()<=1e-9
}

/// Return KL_2(P||Q) in bits; infinite when Q misses positive P support.
pub fn kullback_leibler_divergence(p:&[f64],q:&[f64])
    ->Result<f64,&'static str>{
    if p.len()!=q.len() || !valid_distribution(p) || !valid_distribution(q) {
        return Err("invalid aligned normalized categorical distributions");
    }
    if p.iter().zip(q.iter()).any(|(&a,&b)|a>0.0 && b==0.0){
        return Ok(f64::INFINITY);
    }
    let mut value=0.0_f64;
    for (&a,&b) in p.iter().zip(q.iter()) {
        if a>0.0 {
            let term=a*(a.log2()-b.log2());
            if !term.is_finite(){return Err("KL term overflow");}
            value+=term;
            if !value.is_finite(){return Err("KL sum overflow");}
        }
    }
    if value< -1e-12 {return Err("negative relative entropy");}
    Ok(value.max(0.0))
}

#[cfg(test)]
mod tests {
    use super::kullback_leibler_divergence as kl;
    #[test]
    fn finite_infinite_and_directional() {
        assert_eq!(kl(&[0.5,0.5],&[0.5,0.5]),Ok(0.0));
        assert!((kl(&[1.0,0.0],&[0.5,0.5]).unwrap()-1.0).abs()<1e-12);
        assert!((kl(&[0.75,0.25],&[0.5,0.5]).unwrap()
            -0.1887218755408672).abs()<1e-12);
        assert_eq!(kl(&[1.0,0.0],&[0.0,1.0]),Ok(f64::INFINITY));
        assert_eq!(kl(&[1.0],&[1.0]),Ok(0.0));
        assert!((kl(&[0.5,0.5],&[0.75,0.25]).unwrap()
            -kl(&[0.75,0.25],&[0.5,0.5]).unwrap()).abs()>1e-5);
    }
    #[test]
    fn invalid() {
        assert!(kl(&[],&[]).is_err());
        assert!(kl(&[0.5,0.5],&[0.2,0.2]).is_err());
        assert!(kl(&[1.0,0.0],&[1.0]).is_err());
        assert!(kl(&[1.1,-0.1],&[0.5,0.5]).is_err());
        assert!(kl(&[0.5,0.5],&[f64::NAN,1.0]).is_err());
    }
}
