//! Alpha-balanced binary focal loss with fixed gamma=2 and epsilon floor.

/// Return finite mean binary focal loss on valid probabilities and labels.
pub fn binary_focal_loss(probabilities:&[f64],labels:&[u8])
    ->Result<f64,&'static str> {
    let n=probabilities.len();
    if n==0 || labels.len()!=n {
        return Err("nonempty aligned data required");
    }
    let mut total=0.0_f64;
    for (&p,&label) in probabilities.iter().zip(labels.iter()) {
        if !p.is_finite() || !(0.0..=1.0).contains(&p) || label>1 {
            return Err("invalid probability or binary label");
        }
        let pt=if label==1 {p} else {1.0-p};
        let q=pt.max(1e-12);
        let alpha=if label==1 {0.25} else {0.75};
        let difficulty=1.0-q;
        let sample=-alpha*difficulty*difficulty*q.ln();
        if !sample.is_finite() || sample<0.0 {return Err("invalid focal contribution");}
        total+=sample;
        if !total.is_finite(){return Err("focal sum overflow");}
    }
    let result=total/n as f64;
    if !result.is_finite() || result<0.0 {
        return Err("invalid focal loss");
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::binary_focal_loss as focal;
    #[test]
    fn values() {
        assert!((focal(&[0.5,0.5],&[1,0]).unwrap()-2.0_f64.ln()/8.0).abs()<1e-12);
        assert_eq!(focal(&[0.0,1.0],&[0,1]),Ok(0.0));
        assert!((focal(&[0.0,1.0],&[1,0]).unwrap()-(1e12_f64).ln()/2.0).abs()<1e-10);
        assert!((focal(&[0.5],&[1]).unwrap()-2.0_f64.ln()/16.0).abs()<1e-12);
        assert!(focal(&[0.1],&[1]).unwrap()>focal(&[0.9],&[1]).unwrap());
    }
    #[test]
    fn invalid() {
        assert!(focal(&[],&[]).is_err());
        assert!(focal(&[0.5],&[]).is_err());
        assert!(focal(&[1.2,0.5],&[1,0]).is_err());
        assert!(focal(&[f64::NAN],&[1]).is_err());
        assert!(focal(&[0.5],&[2]).is_err());
    }
}
