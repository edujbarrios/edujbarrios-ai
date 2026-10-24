//! Reusable 30-bit exact-signature LSH bucket index.
use std::collections::BTreeMap;
const MAX_SIGNATURE:u32=(1_u32<<30)-1;

/// Static signature-to-original-indices mapping.
pub struct LshBucketIndex {
    buckets:BTreeMap<u32,Vec<usize>>,
}
impl LshBucketIndex {
    /// Validate and build buckets once; source indices are zero-based.
    pub fn build(signatures:&[u32]) -> Result<Self,&'static str> {
        if signatures.is_empty() || signatures.iter().any(|&s|s>MAX_SIGNATURE) {
            return Err("nonempty unsigned 30-bit signatures required");
        }
        let mut buckets=BTreeMap::new();
        for (index,&signature) in signatures.iter().enumerate() {
            buckets.entry(signature).or_insert_with(Vec::new).push(index);
        }
        Ok(Self{buckets})
    }
    /// Return a fresh list for the exact signature bucket.
    pub fn query(&self,signature:u32)->Result<Vec<usize>,&'static str>{
        if signature>MAX_SIGNATURE {return Err("invalid signature");}
        Ok(self.buckets.get(&signature).cloned().unwrap_or_default())
    }
}

#[cfg(test)]
mod tests {
    use super::LshBucketIndex;
    #[test]
    fn repeated_queries() {
        let idx=LshBucketIndex::build(&[3,7,3,0,7]).unwrap();
        assert_eq!(idx.query(3),Ok(vec![0,2]));
        assert_eq!(idx.query(7),Ok(vec![1,4]));
        assert_eq!(idx.query(4),Ok(vec![]));
        assert_eq!(idx.query(3),Ok(vec![0,2]));
        let idx=LshBucketIndex::build(&[(1<<30)-1]).unwrap();
        assert_eq!(idx.query((1<<30)-1),Ok(vec![0]));
    }
    #[test]
    fn invalid() {
        assert!(LshBucketIndex::build(&[]).is_err());
        assert!(LshBucketIndex::build(&[3,1<<30]).is_err());
        let idx=LshBucketIndex::build(&[3]).unwrap();
        assert!(idx.query(1<<30).is_err());
    }
}
