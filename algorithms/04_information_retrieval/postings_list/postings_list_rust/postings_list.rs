//! One-term inverted postings list for signed 32-bit token IDs.

/// Return (zero-based document index, term frequency) for matching documents.
pub fn postings_list(documents: &[Vec<i32>], target: i32) -> Vec<(usize, usize)> {
    let mut result = Vec::new();
    for (doc_id, document) in documents.iter().enumerate() {
        let frequency = document.iter().filter(|&&token| token == target).count();
        if frequency > 0 {
            result.push((doc_id, frequency));
        }
    }
    result
}

#[cfg(test)]
mod tests {
    use super::postings_list as postings;

    #[test]
    fn standard_and_absent_cases() {
        let docs = [vec![1,2,1], vec![], vec![2,1], vec![3]];
        assert_eq!(postings(&docs,1), vec![(0,2),(2,1)]);
        assert_eq!(postings(&docs,4), vec![]);
        assert_eq!(postings(&[], -4), vec![]);
    }

    #[test]
    fn negative_and_duplicates() {
        assert_eq!(postings(&[vec![-5,-5]],-5),vec![(0,2)]);
        assert_eq!(postings(&[vec![i32::MIN,i32::MAX]],i32::MAX),vec![(0,1)]);
    }
}
