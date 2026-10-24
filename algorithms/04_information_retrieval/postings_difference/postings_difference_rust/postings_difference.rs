//! Sorted unique document-ID difference (AND NOT).

fn validate(values: &[u32]) -> Result<(),&'static str> {
    for (i,&v) in values.iter().enumerate() {
        if v>i32::MAX as u32 || (i>0 && v<=values[i-1]) {
            return Err("invalid strictly increasing document ID list");
        }
    }
    Ok(())
}

/// Return only included IDs absent from excluded IDs.
pub fn postings_difference(included:&[u32],excluded:&[u32])
    -> Result<Vec<u32>,&'static str> {
    validate(included)?;
    validate(excluded)?;
    let mut result=Vec::with_capacity(included.len());
    let (mut i,mut j)=(0,0);
    while i<included.len() && j<excluded.len() {
        if included[i]<excluded[j] {
            result.push(included[i]);i+=1;
        } else if included[i]>excluded[j] {
            j+=1;
        } else {
            i+=1;j+=1;
        }
    }
    result.extend_from_slice(&included[i..]);
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::postings_difference as difference;
    #[test]
    fn behavior() {
        assert_eq!(difference(&[0,2,4,7],&[1,2,5,7]),Ok(vec![0,4]));
        assert_eq!(difference(&[0,2,4],&[0,2,4]),Ok(vec![]));
        assert_eq!(difference(&[0,2,4],&[]),Ok(vec![0,2,4]));
        assert_eq!(difference(&[],&[0,2,4]),Ok(vec![]));
        assert_eq!(difference(&[],&[]),Ok(vec![]));
        assert_eq!(difference(&[i32::MAX as u32],&[]),Ok(vec![i32::MAX as u32]));
    }
    #[test]
    fn invalid() {
        assert!(difference(&[1,1],&[]).is_err());
        assert!(difference(&[],&[5,4]).is_err());
        assert!(difference(&[u32::MAX],&[]).is_err());
        assert!(difference(&[],&[0,0]).is_err());
    }
}
