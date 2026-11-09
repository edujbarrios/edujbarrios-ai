//! Four-neighbor BFS flood-fill of exterior zero regions in binary masks.
use std::collections::VecDeque;

/// Fill zeros not four-connected to the image border; preserve exterior zeros.
pub fn binary_hole_filling(
    pixels: &[u8], height: usize, width: usize,
) -> Result<Vec<u8>, &'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width)
        || pixels.len()!=height*width || pixels.iter().any(|&v|v>1) {
        return Err("invalid binary mask or dimensions");
    }
    let n = pixels.len();
    let mut seen = vec![false; n];
    let mut queue = VecDeque::new();
    for (i,&pixel) in pixels.iter().enumerate() {
        let (row,col)=(i/width,i%width);
        if pixel==0 && (row==0 || row+1==height || col==0 || col+1==width) {
            seen[i]=true;
            queue.push_back(i);
        }
    }
    while let Some(index)=queue.pop_front() {
        let (row,col)=(index/width,index%width);
        let neighbors=[
            (row>0).then(|| index-width),
            (row+1<height).then(|| index+width),
            (col>0).then(|| index-1),
            (col+1<width).then(|| index+1),
        ];
        for next in neighbors.into_iter().flatten() {
            if pixels[next]==0 && !seen[next] {
                seen[next]=true;
                queue.push_back(next);
            }
        }
    }
    Ok(seen.iter().map(|&exterior|if exterior{0}else{1}).collect())
}

#[cfg(test)]
mod tests {
    use super::binary_hole_filling as fill;
    #[test]
    fn examples() {
        assert_eq!(fill(&[1,1,1,1,0,1,1,1,1],3,3),Ok(vec![1;9]));
        assert_eq!(fill(&[0,1,1,1,0,1,1,1,1],3,3),
                   Ok(vec![0,1,1,1,1,1,1,1,1]));
        assert_eq!(fill(&[0,0,0,0],2,2),Ok(vec![0;4]));
        assert_eq!(fill(&[1,1,1,1],2,2),Ok(vec![1;4]));
        assert_eq!(fill(&[0],1,1),Ok(vec![0]));
        assert_eq!(fill(&[1],1,1),Ok(vec![1]));
        assert_eq!(fill(&[1,0,1,0],1,4),Ok(vec![1,0,1,0]));
        assert_eq!(fill(&[1,0,1,0],4,1),Ok(vec![1,0,1,0]));
        let mut donut=vec![0,0,0,0,0, 0,1,1,1,0, 0,1,0,1,0,
                           0,1,1,1,0, 0,0,0,0,0];
        let mut expected=donut.clone();expected[12]=1;
        assert_eq!(fill(&donut,5,5),Ok(expected));
        donut[12]=1;
        assert_eq!(fill(&donut,5,5),Ok(donut.clone()));
    }
    #[test]
    fn maximum() {
        let mut large=vec![1u8;4096];large[32*64+32]=0;
        assert_eq!(fill(&large,64,64),Ok(vec![1;4096]));
        assert_eq!(fill(&[0u8;4096],64,64),Ok(vec![0;4096]));
    }
    #[test]
    fn invalid() {
        assert!(fill(&[1],0,1).is_err());
        assert!(fill(&[1],1,0).is_err());
        assert!(fill(&[1],65,1).is_err());
        assert!(fill(&[1],1,65).is_err());
        assert!(fill(&[0,1],1,1).is_err());
        assert!(fill(&[0,2],1,2).is_err());
    }
}
