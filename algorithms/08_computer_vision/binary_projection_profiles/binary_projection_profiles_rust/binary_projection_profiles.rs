//! Row and column sums of a row-major binary mask.

/// H horizontal counts concatenated with W vertical counts.
pub fn binary_projection_profiles(
    pixels:&[u8],height:usize,width:usize,
)->Result<Vec<u16>,&'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width)
        || pixels.len()!=height*width || pixels.iter().any(|&v|v>1) {
        return Err("invalid binary pixels or dimensions");
    }
    let mut output=vec![0u16;height+width];
    for (i,&pixel) in pixels.iter().enumerate() {
        if pixel!=0 {
            output[i/width]+=1;
            output[height+i%width]+=1;
        }
    }
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::binary_projection_profiles as profiles;
    #[test]
    fn examples() {
        assert_eq!(profiles(&[1,0,1,0,1,1],2,3),Ok(vec![2,2,1,1,2]));
        assert_eq!(profiles(&[1,0,1,1],1,4),Ok(vec![3,1,0,1,1]));
        assert_eq!(profiles(&[1,0,1,1],4,1),Ok(vec![1,0,1,1,3]));
        assert_eq!(profiles(&[0,0,0,0],2,2),Ok(vec![0,0,0,0]));
        assert_eq!(profiles(&[1,1,1,1],2,2),Ok(vec![2,2,2,2]));
        assert_eq!(profiles(&[0],1,1),Ok(vec![0,0]));
        assert_eq!(profiles(&[1],1,1),Ok(vec![1,1]));
        assert_eq!(profiles(&[1,0,0,1],2,2),Ok(vec![1,1,1,1]));
    }
    #[test]
    fn maximum() {
        assert_eq!(profiles(&[1u8;4096],64,64),Ok(vec![64;128]));
        assert_eq!(profiles(&[0u8;4096],64,64),Ok(vec![0;128]));
        let checker:Vec<u8>=(0..64).flat_map(|r|
            (0..64).map(move |c|((r+c)%2) as u8)).collect();
        assert_eq!(profiles(&checker,64,64),Ok(vec![32;128]));
        let mut pixels=vec![0u8;4096];pixels[0]=1;pixels[4095]=1;
        let out=profiles(&pixels,64,64).unwrap();
        assert_eq!(out[0],1);
        assert_eq!(out[63],1);
        assert_eq!(out[64],1);
        assert_eq!(out[127],1);
        assert_eq!(out[..64].iter().sum::<u16>(),2);
        assert_eq!(out[64..].iter().sum::<u16>(),2);
    }
    #[test]
    fn invalid() {
        assert!(profiles(&[0],0,1).is_err());
        assert!(profiles(&[0],1,0).is_err());
        assert!(profiles(&[0],65,1).is_err());
        assert!(profiles(&[0],1,65).is_err());
        assert!(profiles(&[],1,1).is_err());
        assert!(profiles(&[0,1],1,1).is_err());
        assert!(profiles(&[2],1,1).is_err());
    }
}
