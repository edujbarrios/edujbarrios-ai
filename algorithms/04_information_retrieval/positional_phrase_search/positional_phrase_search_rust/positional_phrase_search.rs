//! Exact two-term adjacency matching using sorted positional postings.
const LIMIT:u32=1_000_000_000;

fn valid(postings:&[(u32,u32)])->bool {
    postings.len()<=4096 &&
    postings.iter().all(|&(doc,pos)|doc<=LIMIT && pos<=LIMIT) &&
    postings.windows(2).all(|pair|pair[0]<pair[1])
}

/// Return unique document IDs where second term follows the first immediately.
pub fn positional_phrase_search(first:&[(u32,u32)],second:&[(u32,u32)])
    ->Result<Vec<u32>,&'static str>{
    if !valid(first) || !valid(second) {
        return Err("invalid or unsorted positional postings");
    }
    let mut results=Vec::new();
    let (mut a,mut b)=(0,0);
    while a<first.len() && b<second.len() {
        let da=first[a].0;
        let db=second[b].0;
        if da<db {
            while a<first.len() && first[a].0==da {a+=1;}
        } else if db<da {
            while b<second.len() && second[b].0==db {b+=1;}
        } else {
            let mut end_a=a;
            let mut end_b=b;
            while end_a<first.len() && first[end_a].0==da {end_a+=1;}
            while end_b<second.len() && second[end_b].0==db {end_b+=1;}
            let (mut pa,mut pb)=(a,b);
            while pa<end_a && pb<end_b {
                let wanted=first[pa].1+1;
                let found=second[pb].1;
                if wanted==found {
                    results.push(da);
                    break;
                }
                if wanted<found {pa+=1;} else {pb+=1;}
            }
            a=end_a;b=end_b;
        }
    }
    Ok(results)
}

#[cfg(test)]
mod tests {
    use super::positional_phrase_search as phrase;
    #[test]
    fn documents_and_offsets() {
        let a=[(1,0),(1,5),(2,1),(4,2)];
        let b=[(1,1),(1,8),(2,3),(4,3)];
        assert_eq!(phrase(&a,&b),Ok(vec![1,4]));
        assert_eq!(phrase(&[],&b),Ok(vec![]));
        assert_eq!(phrase(&a,&[]),Ok(vec![]));
        assert_eq!(phrase(&[(5,2)],&[(5,1)]),Ok(vec![]));
        assert_eq!(phrase(&[(1,0),(1,4)],&[(1,1),(1,5)]),Ok(vec![1]));
    }
    #[test]
    fn invalid() {
        assert!(phrase(&[(1,5),(1,0)],&[(1,1)]).is_err());
        assert!(phrase(&[(1,0),(1,0)],&[(1,1)]).is_err());
        assert!(phrase(&[(1,LIMIT_PLUS_ONE)],&[(1,1)]).is_err());
        assert!(phrase(&[(1,0)],&[(1,1),(1,0)]).is_err());
    }
    const LIMIT_PLUS_ONE:u32=1_000_000_001;
}
