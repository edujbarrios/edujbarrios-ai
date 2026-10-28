//! Shannon class entropy in bits from bounded integer counts.

/// Entropy of 1..128 integer class counts; requires positive total.
pub fn entropy_impurity(counts: &[u32]) -> Result<f64, &'static str> {
    if !(1..=128).contains(&counts.len()) ||
       counts.iter().any(|&c| c>1_000_000) {
        return Err("1..128 class counts bounded by 1000000 required");
    }
    let total: u64=counts.iter().map(|&c| u64::from(c)).sum();
    if total==0 { return Err("positive total required"); }
    let total=total as f64;
    let mut result=0.0_f64;
    for &count in counts {
        if count>0 {
            let p=f64::from(count)/total;
            result-=p*p.log2();
        }
    }
    Ok(result.max(0.0))
}
#[cfg(test)]
mod tests {
    use super::entropy_impurity as h;
    fn close(a:f64,b:f64) { assert!((a-b).abs()<1e-12,"{a} != {b}"); }
    #[test] fn examples() {
        assert_eq!(h(&[5,0]),Ok(0.));
        close(h(&[1,1]).unwrap(),1.);
        close(h(&[1,1,1,1]).unwrap(),2.);
        close(h(&[3,1]).unwrap(),0.8112781244591328);
        assert_eq!(h(&[1000000]),Ok(0.));
        close(h(&[1000000,1000000]).unwrap(),1.);
    }
    #[test] fn invalid() {
        assert!(h(&[]).is_err());
        assert!(h(&[0,0]).is_err());
        assert!(h(&[1000001,0]).is_err());
        assert!(h(&vec![1;129]).is_err());
    }
}
