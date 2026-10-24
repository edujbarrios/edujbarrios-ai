//! Viterbi dynamic programming for two-state, binary-observation HMMs.

fn normalized_pair(p:f64,q:f64)->bool{
    p.is_finite() && q.is_finite() && p>0.0 && q>0.0 &&
    p<=1.0 && q<=1.0 && ((p+q)-1.0).abs()<=1e-9
}

/// Decode the max-joint-probability path, breaking predecessor/final ties to 0.
pub fn viterbi_decoding(
    observations:&[u8], initial:&[f64;2], transition:&[f64;4],
    emission:&[f64;4]
) -> Result<(Vec<u8>,f64),&'static str> {
    let n=observations.len();
    if !(1..=4096).contains(&n) || observations.iter().any(|&o|o>1) ||
       !normalized_pair(initial[0],initial[1]) ||
       (0..2).any(|i| !normalized_pair(transition[2*i],transition[2*i+1]) ||
                      !normalized_pair(emission[2*i],emission[2*i+1])) {
        return Err("invalid observation sequence or probability distributions");
    }
    let mut prev=[
        initial[0].ln()+emission[observations[0] as usize].ln(),
        initial[1].ln()+emission[2+observations[0] as usize].ln()
    ];
    if !prev.iter().all(|v|v.is_finite()){return Err("initial path log overflow");}
    let mut back=vec![[0_u8;2];n];
    for t in 1..n {
        let mut next=[0.0_f64;2];
        for s in 0..2 {
            let a=prev[0]+transition[s].ln();
            let b=prev[1]+transition[2+s].ln();
            if !a.is_finite() || !b.is_finite(){return Err("transition score overflow");}
            let predecessor=if b>a {1_usize} else {0_usize};
            back[t][s]=predecessor as u8;
            next[s]=[a,b][predecessor]+emission[2*s+observations[t] as usize].ln();
            if !next[s].is_finite(){return Err("emission score overflow");}
        }
        prev=next;
    }
    let mut state=if prev[1]>prev[0] {1_usize} else {0_usize};
    let best=prev[state];
    let mut path=vec![0_u8;n];
    for t in (0..n).rev() {
        path[t]=state as u8;
        if t>0 {state=back[t][state] as usize;}
    }
    Ok((path,best))
}

#[cfg(test)]
mod tests {
    use super::viterbi_decoding as decode;
    #[test]
    fn known_paths_and_ties() {
        let p=[0.6,0.4];
        let a=[0.7,0.3,0.4,0.6];
        let b=[0.5,0.5,0.1,0.9];
        let (path,score)=decode(&[0,1,0],&p,&a,&b).unwrap();
        assert_eq!(path,vec![0,0,0]);
        assert!((score-0.03675_f64.ln()).abs()<1e-12);
        assert_eq!(decode(&[1,1,1],&p,&a,&b).unwrap().0,vec![1,1,1]);
        let m=[0.5;4];
        assert_eq!(decode(&[0,1,0],&[0.5,0.5],&m,&m).unwrap().0,vec![0,0,0]);
        assert_eq!(decode(&[0],&p,&a,&b).unwrap().0,vec![0]);
    }
    #[test]
    fn invalid() {
        let p=[0.6,0.4];
        let a=[0.7,0.3,0.4,0.6];
        let b=[0.5,0.5,0.1,0.9];
        assert!(decode(&[],&p,&a,&b).is_err());
        assert!(decode(&vec![0;4097],&p,&a,&b).is_err());
        assert!(decode(&[0,2],&p,&a,&b).is_err());
        assert!(decode(&[0],&[0.0,1.0],&a,&b).is_err());
        assert!(decode(&[0],&p,&[0.7,0.4,0.4,0.6],&b).is_err());
        assert!(decode(&[0],&p,&a,&[0.5,0.5,0.0,1.0]).is_err());
    }
}
