//! Laplace-smoothed binary multinomial Naive Bayes on count vectors.

const MAX_COUNT:u32=1_000_000;
const MAX_EXACT_SUM:f64=9_007_199_254_740_991.0;

/// Returns (log_prior[2], class-major log_likelihood[2*d]).
pub fn multinomial_naive_bayes(
    counts:&[u32],labels:&[u8],d:usize
) -> Result<([f64;2],Vec<f64>),&'static str> {
    if d==0 || labels.is_empty() ||
       labels.len().checked_mul(d).ok_or("dimension overflow")?!=counts.len() ||
       labels.iter().any(|&y|y>1) ||
       counts.iter().any(|&x|x>MAX_COUNT) {
        return Err("invalid count matrix or labels");
    }
    let mut tokens=vec![0.0_f64;d.checked_mul(2).ok_or("shape overflow")?];
    let (mut totals,mut members)=([0.0_f64;2],[0.0_f64;2]);
    for (i,&label) in labels.iter().enumerate() {
        let c=label as usize;
        members[c]+=1.0;
        for j in 0..d {
            let value=counts[i*d+j] as f64;
            tokens[c*d+j]+=value;
            totals[c]+=value;
            if tokens[c*d+j]>MAX_EXACT_SUM || totals[c]>MAX_EXACT_SUM {
                return Err("integer accumulation exceeds exact limit");
            }
        }
    }
    if members.iter().any(|&v|v==0.0) {
        return Err("both classes required");
    }
    let priors=[(members[0]/labels.len() as f64).ln(),
                (members[1]/labels.len() as f64).ln()];
    if !priors.iter().all(|x|x.is_finite()) {return Err("invalid class priors");}
    let mut likelihoods=vec![0.0_f64;2*d];
    for c in 0..2 {
        let denominator=totals[c]+d as f64;
        if !denominator.is_finite() || denominator<=0.0 {
            return Err("invalid class token sum");
        }
        for j in 0..d {
            let logp=((tokens[c*d+j]+1.0)/denominator).ln();
            if !logp.is_finite() {return Err("invalid token probability");}
            likelihoods[c*d+j]=logp;
        }
    }
    Ok((priors,likelihoods))
}

/// Predict using binary log priors and class-major likelihood weights.
pub fn multinomial_naive_bayes_predict(
    priors:&[f64;2],likelihoods:&[f64],query:&[u32]
) -> Result<u8,&'static str> {
    let d=query.len();
    if d==0 || d.checked_mul(2).ok_or("dimension overflow")?!=likelihoods.len() ||
       query.iter().any(|&v|v>MAX_COUNT) ||
       !priors.iter().chain(likelihoods.iter()).all(|v|v.is_finite()) {
        return Err("invalid query or model parameters");
    }
    let mut scores=*priors;
    for j in 0..d {
        for c in 0..2 {
            let term=query[j] as f64*likelihoods[c*d+j];
            if !term.is_finite(){return Err("likelihood product overflow");}
            scores[c]+=term;
            if !scores[c].is_finite(){return Err("classification score overflow");}
        }
    }
    Ok(if scores[1]>scores[0] {1} else {0})
}

#[cfg(test)]
mod tests {
    use super::{multinomial_naive_bayes as fit,multinomial_naive_bayes_predict as predict};
    #[test]
    fn model_and_inference() {
        let (p,w)=fit(&[3,0,2,0,0,3,0,2],&[0,0,1,1],2).unwrap();
        assert!((p[0]-0.5_f64.ln()).abs()<1e-12);
        assert!((w[0]-(6.0_f64/7.0).ln()).abs()<1e-12);
        assert!((w[1]-(1.0_f64/7.0).ln()).abs()<1e-12);
        assert!((w[2]-(1.0_f64/7.0).ln()).abs()<1e-12);
        assert!((w[3]-(6.0_f64/7.0).ln()).abs()<1e-12);
        assert_eq!(predict(&p,&w,&[2,0]),Ok(0));
        assert_eq!(predict(&p,&w,&[0,2]),Ok(1));
        assert_eq!(predict(&p,&w,&[0,0]),Ok(0));
    }
    #[test]
    fn invalid() {
        assert!(fit(&[],&[],1).is_err());
        assert!(fit(&[1,0],&[0],2).is_err());
        assert!(fit(&[1,0,0,1],&[0,0],2).is_err());
        assert!(fit(&[1,0,0,1],&[0,2],2).is_err());
        assert!(fit(&[1_000_001,0,0,1],&[0,1],2).is_err());
        assert!(predict(&[0.0,0.0],&[0.0,0.0],&[0,1]).is_err());
        assert!(predict(&[f64::NAN,0.0],&[0.0;4],&[0,1]).is_err());
    }
}
