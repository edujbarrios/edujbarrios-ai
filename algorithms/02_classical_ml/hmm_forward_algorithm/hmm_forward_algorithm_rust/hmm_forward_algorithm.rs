//! Log-space Forward algorithm for marginal HMM observation likelihood.

fn pair_is_distribution(a:f64,b:f64)->bool {
    a.is_finite() && b.is_finite() && a>0.0 && b>0.0 &&
    a<=1.0 && b<=1.0 && ((a+b)-1.0).abs()<=1e-9
}
fn logsumexp2(a:f64,b:f64)->f64 {
    let high=a.max(b);
    let low=a.min(b);
    high+(low-high).exp().ln_1p()
}

/// Return log P(observations) after marginalizing both hidden states.
pub fn hmm_forward_algorithm(obs:&[u8], initial:&[f64;2],
    transition:&[f64;4], emission:&[f64;4])->Result<f64,&'static str>{
    if !(1..=4096).contains(&obs.len()) || obs.iter().any(|&x|x>1) ||
       !pair_is_distribution(initial[0],initial[1]) ||
       (0..2).any(|s|!pair_is_distribution(transition[2*s],transition[2*s+1]) ||
                       !pair_is_distribution(emission[2*s],emission[2*s+1])) {
        return Err("invalid sequence or normalized HMM probabilities");
    }
    let mut prev=[
        initial[0].ln()+emission[obs[0] as usize].ln(),
        initial[1].ln()+emission[2+obs[0] as usize].ln()
    ];
    if !prev.iter().all(|x|x.is_finite()){
        return Err("nonfinite initial log probability");
    }
    for &symbol in &obs[1..] {
        let mut next=[0.0_f64;2];
        for s in 0..2 {
            let from0=prev[0]+transition[s].ln();
            let from1=prev[1]+transition[2+s].ln();
            if !from0.is_finite() || !from1.is_finite(){
                return Err("transition log overflow");
            }
            next[s]=logsumexp2(from0,from1)+emission[2*s+symbol as usize].ln();
            if !next[s].is_finite(){return Err("emission log overflow");}
        }
        prev=next;
    }
    let result=logsumexp2(prev[0],prev[1]);
    if !result.is_finite() || result>1e-12 {
        return Err("invalid observation likelihood");
    }
    Ok(result.min(0.0))
}

#[cfg(test)]
mod tests {
    use super::hmm_forward_algorithm as forward;
    #[test]
    fn known_and_uniform() {
        let p=[0.6,0.4];
        let a=[0.7,0.3,0.4,0.6];
        let b=[0.5,0.5,0.1,0.9];
        assert!((forward(&[0,1,0],&p,&a,&b).unwrap()-0.069616_f64.ln()).abs()<1e-12);
        assert!((forward(&[0],&p,&a,&b).unwrap()-0.34_f64.ln()).abs()<1e-12);
        let seq:Vec<u8>=(0..100).map(|i|(i%2) as u8).collect();
        assert!((forward(&seq,&[0.5,0.5],&[0.5;4],&[0.5;4]).unwrap()
            -100.0*0.5_f64.ln()).abs()<1e-11);
    }
    #[test]
    fn invalid() {
        let p=[0.6,0.4];
        let a=[0.7,0.3,0.4,0.6];
        let b=[0.5,0.5,0.1,0.9];
        assert!(forward(&[],&p,&a,&b).is_err());
        assert!(forward(&vec![0;4097],&p,&a,&b).is_err());
        assert!(forward(&[0,2],&p,&a,&b).is_err());
        assert!(forward(&[0],&[0.0,1.0],&a,&b).is_err());
        assert!(forward(&[0],&p,&[0.7,0.4,0.4,0.6],&b).is_err());
        assert!(forward(&[0],&p,&a,&[0.5,0.5,0.0,1.0]).is_err());
    }
}
