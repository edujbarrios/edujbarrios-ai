//! Deterministic full-sort selection of Top-K numerical scores.

pub fn top_k(scores: &[f64], k: usize) -> Result<Vec<usize>, &'static str> {
    if scores.is_empty() || k == 0 || k > scores.len() {
        return Err("invalid score length or k");
    }
    if !scores.iter().all(|x| x.is_finite()) {
        return Err("scores must be finite");
    }
    let mut indices: Vec<usize> = (0..scores.len()).collect();
    indices.sort_by(|&a, &b| scores[b].total_cmp(&scores[a]).then(a.cmp(&b)));
    indices.truncate(k);
    Ok(indices)
}

#[cfg(test)]
mod tests {
    use super::top_k;

    #[test]
    fn ordering_and_ties() {
        assert_eq!(top_k(&[0.8,0.9,0.9,-1.0],3), Ok(vec![1,2,0]));
        assert_eq!(top_k(&[1.0,1.0,1.0],2), Ok(vec![0,1]));
        assert_eq!(top_k(&[2.0,-4.0],2), Ok(vec![0,1]));
    }
    #[test]
    fn invalid() {
        assert!(top_k(&[],1).is_err());
        assert!(top_k(&[1.0],0).is_err());
        assert!(top_k(&[1.0],2).is_err());
        assert!(top_k(&[0.1,f64::NAN],1).is_err());
        assert!(top_k(&[f64::INFINITY],1).is_err());
    }
}
