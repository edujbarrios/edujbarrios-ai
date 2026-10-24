//! Token perplexity as exp(mean negative conditional log probability).

/// Require 1..4096 finite natural log probabilities <=0 and finite output.
pub fn perplexity(log_probabilities:&[f64])->Result<f64,&'static str>{
    let n=log_probabilities.len();
    if !(1..=4096).contains(&n) {
        return Err("log probability vector length must be 1..4096");
    }
    let mut mean_nll=0.0_f64;
    for &value in log_probabilities {
        if !value.is_finite() || value>0.0 {
            return Err("log probabilities must be finite and nonpositive");
        }
        let contribution=(-value)/n as f64;
        mean_nll+=contribution;
        if !contribution.is_finite() || !mean_nll.is_finite() {
            return Err("mean negative log likelihood overflow");
        }
    }
    let result=mean_nll.exp();
    if !result.is_finite() || result<1.0 {
        return Err("perplexity exponential overflow");
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::perplexity;
    #[test]
    fn examples() {
        assert!((perplexity(&[0.5_f64.ln(),0.25_f64.ln()]).unwrap()
             -8.0_f64.sqrt()).abs()<1e-12);
        assert!((perplexity(&[0.25_f64.ln();2]).unwrap()-4.0).abs()<1e-12);
        assert_eq!(perplexity(&[0.0;3]),Ok(1.0));
        assert!((perplexity(&[0.5_f64.ln()]).unwrap()-2.0).abs()<1e-12);
        assert!((perplexity(&vec![0.5_f64.ln();100]).unwrap()-2.0).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        assert!(perplexity(&[]).is_err());
        assert!(perplexity(&[f64::NAN]).is_err());
        assert!(perplexity(&[f64::INFINITY]).is_err());
        assert!(perplexity(&[f64::NEG_INFINITY]).is_err());
        assert!(perplexity(&[0.1]).is_err());
        assert!(perplexity(&[-1000.0]).is_err());
    }
}
