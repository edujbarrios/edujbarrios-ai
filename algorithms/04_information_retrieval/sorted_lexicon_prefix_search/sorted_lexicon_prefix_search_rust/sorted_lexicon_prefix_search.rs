//! Prefix search with lower_bound over a strictly sorted ASCII lexicon.

fn valid_word(word: &str, allow_empty: bool) -> bool {
    (allow_empty || !word.is_empty()) &&
        word.len() <= 512 && word.bytes().all(|b| b.is_ascii_lowercase())
}

/// Return zero-based indices for all exact prefix matches.
pub fn sorted_lexicon_prefix_search(
    terms: &[&str], prefix: &str,
) -> Result<Vec<usize>, &'static str> {
    if !valid_word(prefix, true) {
        return Err("prefix must contain only lowercase ASCII characters");
    }
    for (i, word) in terms.iter().enumerate() {
        if !valid_word(word, false) || (i > 0 && terms[i-1] >= *word) {
            return Err("dictionary must be unique sorted lowercase ASCII words");
        }
    }
    let mut low = 0usize;
    let mut high = terms.len();
    while low < high {
        let mid = low + (high - low) / 2;
        if terms[mid] < prefix { low = mid + 1; }
        else { high = mid; }
    }
    let mut indices = Vec::new();
    while low < terms.len() && terms[low].starts_with(prefix) {
        indices.push(low);
        low += 1;
    }
    Ok(indices)
}

#[cfg(test)]
mod tests {
    use super::sorted_lexicon_prefix_search as search;

    #[test]
    fn matches() {
        let words = ["ant", "antenna", "ape", "bat", "batch", "cat"];
        assert_eq!(search(&words, "an"), Ok(vec![0, 1]));
        assert_eq!(search(&words, "ba"), Ok(vec![3, 4]));
        assert_eq!(search(&words, "cat"), Ok(vec![5]));
        assert_eq!(search(&words, "z"), Ok(vec![]));
        assert_eq!(search(&words, ""), Ok(vec![0, 1, 2, 3, 4, 5]));
        assert_eq!(search(&[], ""), Ok(vec![]));
        assert_eq!(search(&["a"], "a"), Ok(vec![0]));
        assert_eq!(search(&[&"a".repeat(512)], &"a".repeat(512)), Ok(vec![0]));
    }

    #[test]
    fn rejects_invalid_dictionary() {
        assert!(search(&["bat", "ant"], "a").is_err());
        assert!(search(&["ant", "ant"], "an").is_err());
        assert!(search(&[""], "").is_err());
        assert!(search(&["Ant"], "a").is_err());
        assert!(search(&["mañana"], "m").is_err());
        assert!(search(&["a-b"], "a").is_err());
        assert!(search(&["ant"], "A").is_err());
        assert!(search(&["ant"], &"a".repeat(513)).is_err());
    }
}
