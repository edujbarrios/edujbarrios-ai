//! Next-token n-gram repetition mask for autoregressive language modeling.

/// Return a boolean mask indexed by the zero-based vocabulary ID.
pub fn no_repeat_ngram(history: &[usize], vocab: usize, n: usize)
    -> Result<Vec<bool>, &'static str> {
    if !(1..=4096).contains(&vocab) || !(1..=32).contains(&n) ||
       history.len() > 16_384 || history.iter().any(|&t| t >= vocab) {
        return Err("invalid vocabulary, n-gram width, or token ID");
    }
    let mut blocked = vec![false; vocab];
    if history.len() < n {
        return Ok(blocked);
    }
    let prefix = &history[history.len() - (n - 1)..];
    for i in 0..=history.len() - n {
        if &history[i..i + n - 1] == prefix {
            blocked[history[i + n - 1]] = true;
        }
    }
    Ok(blocked)
}

#[cfg(test)]
mod tests {
    use super::no_repeat_ngram as mask;

    #[test]
    fn reference_cases() {
        assert_eq!(mask(&[1,2,1],4,1),Ok(vec![false,true,true,false]));
        assert_eq!(mask(&[1,2,1,2],5,2),Ok(vec![false,true,false,false,false]));
        assert_eq!(mask(&[1,2,3,1,2],5,3),Ok(vec![false,false,false,true,false]));
        assert_eq!(mask(&[1,1,1],3,2),Ok(vec![false,true,false]));
        assert_eq!(mask(&[],3,1),Ok(vec![false;3]));
        assert_eq!(mask(&[0,1],3,3),Ok(vec![false;3]));
    }

    #[test]
    fn invalid() {
        assert!(mask(&[0],0,1).is_err());
        assert!(mask(&[0],4097,1).is_err());
        assert!(mask(&[0],2,0).is_err());
        assert!(mask(&[0],2,33).is_err());
        assert!(mask(&[2],2,1).is_err());
        assert!(mask(&[0;16_385],2,1).is_err());
    }
}
