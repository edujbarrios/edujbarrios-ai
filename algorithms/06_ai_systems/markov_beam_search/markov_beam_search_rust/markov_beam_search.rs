//! Fixed-length deterministic beam search over Markov token transitions.

fn valid_row(row:&[f64])->bool {
    !row.is_empty() && row.iter().all(|p|p.is_finite() && (0.0..=1.0).contains(p) && *p>0.0) &&
        (row.iter().sum::<f64>()-1.0).abs()<=1e-9
}
/// Return the highest-scoring path retained by a bounded probability beam.
pub fn markov_beam_search(initial:&[f64],transition:&[f64],length:usize,width:usize)
    ->Result<(Vec<u8>,f64),&'static str>{
    let n=initial.len();
    if !(1..=8).contains(&n) || !(1..=8).contains(&length) ||
       !(1..=8).contains(&width) ||
       n.checked_mul(n).ok_or("dimension overflow")?!=transition.len() ||
       !valid_row(initial) ||
       transition.chunks_exact(n).any(|row|!valid_row(row)) {
        return Err("invalid vocabulary, normalized probabilities or beam settings");
    }
    let mut beam=vec![(0.0_f64,Vec::<u8>::new())];
    for step in 0..length {
        let mut candidates=Vec::with_capacity(beam.len()*n);
        for (score,path) in &beam {
            for token in 0..n {
                let probability=if step==0 {initial[token]}
                    else {transition[*path.last().ok_or("missing token")? as usize*n+token]};
                let next_score=*score+probability.ln();
                if !next_score.is_finite(){return Err("beam score overflow");}
                let mut sequence=path.clone();
                sequence.push(token as u8);
                candidates.push((next_score,sequence));
            }
        }
        candidates.sort_by(|a,b|{
            b.0.partial_cmp(&a.0).expect("finite path scores")
                .then(a.1.cmp(&b.1))
        });
        candidates.truncate(width);
        beam=candidates;
    }
    let (score,path)=beam.into_iter().next().ok_or("empty beam")?;
    Ok((path,score))
}

#[cfg(test)]
mod tests {
    use super::markov_beam_search as beam;
    #[test]
    fn known_and_ties() {
        let (path,score)=beam(&[0.6,0.4],&[0.9,0.1,0.2,0.8],3,2).unwrap();
        assert_eq!(path,vec![0,0,0]);
        assert!((score-0.486_f64.ln()).abs()<1e-12);
        assert_eq!(beam(&[0.5,0.5],&[0.5;4],3,2).unwrap().0,vec![0,0,0]);
        assert_eq!(beam(&[0.6,0.4],&[0.9,0.1,0.2,0.8],3,1).unwrap().0,vec![0,0,0]);
        assert_eq!(beam(&[1.0],&[1.0],3,8),Ok((vec![0,0,0],0.0)));
    }
    #[test]
    fn invalid() {
        assert!(beam(&[],&[],1,1).is_err());
        assert!(beam(&[0.0,1.0],&[0.9,0.1,0.2,0.8],2,2).is_err());
        assert!(beam(&[0.6,0.4],&[0.8,0.8,0.2,0.8],2,2).is_err());
        assert!(beam(&[0.6,0.4],&[0.9,f64::NAN,0.2,0.8],2,2).is_err());
        assert!(beam(&[0.6,0.4],&[0.9,0.1,0.2,0.8],0,2).is_err());
        assert!(beam(&[0.6,0.4],&[0.9,0.1,0.2,0.8],2,0).is_err());
    }
}
