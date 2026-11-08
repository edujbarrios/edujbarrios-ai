//! Jaro-Winkler similarity for printable ASCII strings.

fn validate(s: &str) -> Result<(), &'static str> {
    if s.len() > 512 || !s.bytes().all(|b| (32..=126).contains(&b)) {
        return Err("expected printable ASCII of length at most 512");
    }
    Ok(())
}

/// Returns a case-sensitive similarity from zero to one.
pub fn jaro_winkler_similarity(a: &str, b: &str) -> Result<f64, &'static str> {
    validate(a)?;
    validate(b)?;
    let x = a.as_bytes();
    let y = b.as_bytes();
    let (n, m) = (x.len(), y.len());
    if n == 0 || m == 0 {
        return Ok(if n == m { 1.0 } else { 0.0 });
    }
    let window = (n.max(m) / 2).saturating_sub(1);
    let mut matched_x = vec![false; n];
    let mut matched_y = vec![false; m];
    let mut matches = 0usize;
    for (i, &ch) in x.iter().enumerate() {
        for j in i.saturating_sub(window)..m.min(i + window + 1) {
            if !matched_y[j] && ch == y[j] {
                matched_x[i] = true;
                matched_y[j] = true;
                matches += 1;
                break;
            }
        }
    }
    if matches == 0 {
        return Ok(0.0);
    }
    let mut mismatches = 0usize;
    let mut j = 0;
    for (i, &ch) in x.iter().enumerate() {
        if !matched_x[i] {
            continue;
        }
        while !matched_y[j] {
            j += 1;
        }
        mismatches += usize::from(ch != y[j]);
        j += 1;
    }
    let matched = matches as f64;
    let jaro = (matched / n as f64 + matched / m as f64
        + (matched - mismatches as f64 / 2.0) / matched) / 3.0;
    let prefix = x.iter().zip(y).take(4).take_while(|(u, v)| u == v).count();
    Ok(if jaro > 0.7 {
        jaro + 0.1 * prefix as f64 * (1.0 - jaro)
    } else {
        jaro
    })
}

#[cfg(test)]
mod tests {
    use super::jaro_winkler_similarity as score;

    fn near(a: &str, b: &str, expected: f64) {
        assert!((score(a, b).unwrap() - expected).abs() < 1e-12);
    }

    #[test]
    fn known_cases() {
        near("MARTHA", "MARHTA", 0.9611111111111111);
        near("DIXON", "DICKSONX", 0.8133333333333334);
        near("JELLYFISH", "SMELLYFISH", 0.8962962962962963);
        near("ABCD", "ACBD", 0.925);
        near("abc", "axz", 5.0 / 9.0);
        near("MARHTA", "MARTHA", 0.9611111111111111);
    }

    #[test]
    fn edge_cases() {
        near("", "", 1.0);
        near("", "abc", 0.0);
        near("abc", "", 0.0);
        near("x", "x", 1.0);
        near("x", "y", 0.0);
        near("A", "a", 0.0);
        near(&"a".repeat(512), &"a".repeat(512), 1.0);
    }

    #[test]
    fn invalid() {
        assert!(score(&"a".repeat(513), "a").is_err());
        assert!(score("a", &"a".repeat(513)).is_err());
        assert!(score("a\tb", "a").is_err());
        assert!(score("a\nb", "a").is_err());
        assert!(score("a\0b", "a").is_err());
        assert!(score("é", "e").is_err());
        assert!(score("\u{0001}", "a").is_err());
    }
}
