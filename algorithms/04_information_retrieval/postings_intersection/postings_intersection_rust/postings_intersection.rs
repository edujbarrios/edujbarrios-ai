//! AND intersection of strictly increasing document-ID postings lists.

fn validate(values: &[u32]) -> Result<(), &'static str> {
    for (i, &value) in values.iter().enumerate() {
        if value > i32::MAX as u32 {
            return Err("document ID exceeds 2^31-1");
        }
        if i > 0 && value <= values[i-1] {
            return Err("document IDs must be strictly increasing");
        }
    }
    Ok(())
}

/// Return sorted unique zero-based document IDs present in both lists.
pub fn postings_intersection(
    left: &[u32], right: &[u32]
) -> Result<Vec<u32>, &'static str> {
    validate(left)?;
    validate(right)?;
    let mut result = Vec::new();
    let (mut i, mut j) = (0, 0);
    while i < left.len() && j < right.len() {
        if left[i] < right[j] { i += 1; }
        else if left[i] > right[j] { j += 1; }
        else {
            result.push(left[i]);
            i += 1;
            j += 1;
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::postings_intersection as intersect;

    #[test]
    fn matches_and_empty() {
        assert_eq!(intersect(&[0,2,4,9],&[1,2,4,8]), Ok(vec![2,4]));
        assert_eq!(intersect(&[0,2,4,9],&[0,2,4,9]), Ok(vec![0,2,4,9]));
        assert_eq!(intersect(&[],&[2,4]), Ok(vec![]));
    }

    #[test]
    fn invalid() {
        assert!(intersect(&[1,1,2],&[1]).is_err());
        assert!(intersect(&[5,2],&[2]).is_err());
        assert!(intersect(&[u32::MAX],&[1]).is_err());
        assert!(intersect(&[],&[3,3]).is_err());
    }
}
