//! Unit-cost Levenshtein distance on printable ASCII strings.

fn check(input: &str) -> Result<(),&'static str> {
    if input.len()>512 || !input.bytes().all(|b|(32..=126).contains(&b)) {
        return Err("printable ASCII input of <=512 bytes required");
    }
    Ok(())
}

/// Dynamic programming using just two rows on the shorter input.
pub fn levenshtein_distance(a: &str,b: &str) -> Result<usize,&'static str> {
    check(a)?;
    check(b)?;
    let (long,short)=if a.len()>=b.len() {(a.as_bytes(),b.as_bytes())}
                      else {(b.as_bytes(),a.as_bytes())};
    let mut prev:Vec<usize>=(0..=short.len()).collect();
    let mut curr=vec![0_usize;short.len()+1];
    for (i,&left) in long.iter().enumerate() {
        curr[0]=i+1;
        for (j,&right) in short.iter().enumerate() {
            curr[j+1]=(prev[j+1]+1).min(curr[j]+1)
                .min(prev[j]+usize::from(left!=right));
        }
        std::mem::swap(&mut prev,&mut curr);
    }
    Ok(prev[short.len()])
}

#[cfg(test)]
mod tests {
    use super::levenshtein_distance as d;
    #[test]
    fn examples() {
        assert_eq!(d("kitten","sitting"),Ok(3));
        assert_eq!(d("sitting","kitten"),Ok(3));
        assert_eq!(d("","abc"),Ok(3));
        assert_eq!(d("abc",""),Ok(3));
        assert_eq!(d("cat","cat"),Ok(0));
        assert_eq!(d("ab","ba"),Ok(2));
        assert_eq!(d("A","a"),Ok(1));
        assert_eq!(d(&"a".repeat(512),""),Ok(512));
    }
    #[test]
    fn invalid() {
        assert!(d(&"a".repeat(513),"").is_err());
        assert!(d("a\tb","ab").is_err());
        assert!(d("a\nb","ab").is_err());
        assert!(d("é","e").is_err());
        assert!(d("\0","").is_err());
        assert!(d("\u{0001}","").is_err());
    }
}
