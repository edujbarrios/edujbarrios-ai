//! Foreground boundary pixels under four-neighbor connectivity.

/// Extract binary foreground pixels touching zero or image exterior.
pub fn binary_boundary_extraction(
    pixels:&[u8],height:usize,width:usize,
)->Result<Vec<u8>,&'static str>{
    if !(1..=64).contains(&height) || !(1..=64).contains(&width)
        || pixels.len()!=height*width || pixels.iter().any(|&v|v>1){
        return Err("invalid flat binary image or dimensions");
    }
    let mut result=vec![0u8;pixels.len()];
    for(i,&pixel) in pixels.iter().enumerate(){
        if pixel==0 {continue;}
        let (row,col)=(i/width,i%width);
        if row==0 || row+1==height || col==0 || col+1==width
            || pixels[i-width]==0 || pixels[i+width]==0
            || pixels[i-1]==0 || pixels[i+1]==0 {
            result[i]=1;
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests{
    use super::binary_boundary_extraction as boundary;
    #[test]
    fn examples(){
        assert_eq!(boundary(&[1u8;9],3,3),Ok(vec![1,1,1,1,0,1,1,1,1]));
        assert_eq!(boundary(&[0,0,0,0,1,0,0,0,0],3,3),
                   Ok(vec![0,0,0,0,1,0,0,0,0]));
        assert_eq!(boundary(&[1,1,1,1,0,1,1,1,1],3,3),
                   Ok(vec![1,1,1,1,0,1,1,1,1]));
        assert_eq!(boundary(&[0u8;9],3,3),Ok(vec![0;9]));
        assert_eq!(boundary(&[1],1,1),Ok(vec![1]));
        assert_eq!(boundary(&[0],1,1),Ok(vec![0]));
        assert_eq!(boundary(&[1,1,1,1],1,4),Ok(vec![1;4]));
        assert_eq!(boundary(&[1,1,1,1],4,1),Ok(vec![1;4]));
        assert_eq!(boundary(&[1,0,0,1],2,2),Ok(vec![1,0,0,1]));
    }
    #[test]
    fn maximum(){
        let result=boundary(&[1u8;4096],64,64).unwrap();
        assert_eq!(result.iter().filter(|&&x|x==1).count(),252);
        assert_eq!(result[0],1);
        assert_eq!(result[32*64+32],0);
        assert_eq!(boundary(&[0u8;4096],64,64),Ok(vec![0;4096]));
        let checker:Vec<u8>=(0..64).flat_map(|r|
            (0..64).map(move |c| ((r+c)%2) as u8)).collect();
        assert_eq!(boundary(&checker,64,64),Ok(checker.clone()));
    }
    #[test]
    fn invalid(){
        assert!(boundary(&[1],0,1).is_err());
        assert!(boundary(&[1],1,0).is_err());
        assert!(boundary(&[1],65,1).is_err());
        assert!(boundary(&[1],1,65).is_err());
        assert!(boundary(&[],1,1).is_err());
        assert!(boundary(&[0,1],1,1).is_err());
        assert!(boundary(&[2],1,1).is_err());
    }
}
