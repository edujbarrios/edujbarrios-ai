//! Linear-time OR union over sorted unique document-ID postings.

fn validate(values: &[u32]) -> Result<(),&'static str> {
    for (i,&v) in values.iter().enumerate() {
        if v>i32::MAX as u32 || (i>0 && v<=values[i-1]) {
            return Err("postings must contain increasing IDs <=2^31-1");
        }
    }
    Ok(())
}

/// Return sorted unique zero-based IDs present in either list.
pub fn postings_union(left: &[u32],right: &[u32])
    -> Result<Vec<u32>,&'static str> {
    validate(left)?;
    validate(right)?;
    let capacity=left.len().checked_add(right.len()).ok_or("length overflow")?;
    let mut result=Vec::with_capacity(capacity);
    let (mut i,mut j)=(0,0);
    while i<left.len() && j<right.len() {
        if left[i]<right[j] {
            result.push(left[i]);i+=1;
        } else if left[i]>right[j] {
            result.push(right[j]);j+=1;
        } else {
            result.push(left[i]);i+=1;j+=1;
        }
    }
    result.extend_from_slice(&left[i..]);
    result.extend_from_slice(&right[j..]);
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::postings_union as union;
    #[test]
    fn overlap_and_empty() {
        assert_eq!(union(&[0,2,4],&[1,2,5]),Ok(vec![0,1,2,4,5]));
        assert_eq!(union(&[],&[0,2,4]),Ok(vec![0,2,4]));
        assert_eq!(union(&[0,2,4],&[]),Ok(vec![0,2,4]));
        assert_eq!(union(&[],&[]),Ok(vec![]));
        assert_eq!(union(&[i32::MAX as u32],&[]),Ok(vec![i32::MAX as u32]));
    }
    #[test]
    fn invalid() {
        assert!(union(&[1,1],&[]).is_err());
        assert!(union(&[],&[5,4]).is_err());
        assert!(union(&[u32::MAX],&[]).is_err());
        assert!(union(&[1],&[0,0]).is_err());
    }
}
