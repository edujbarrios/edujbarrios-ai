//! Multi-source BFS for 4-neighbor Manhattan distances to background pixels.
use std::collections::VecDeque;

/// Distance to nearest in-image zero; sentinel height+width if no zero.
pub fn binary_distance_transform(
    pixels: &[u8], height: usize, width: usize,
) -> Result<Vec<u16>, &'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width)
        || pixels.len() != height * width || pixels.iter().any(|&v| v > 1) {
        return Err("invalid binary image or dimensions");
    }
    let sentinel = (height + width) as u16;
    let mut distance = vec![sentinel; height * width];
    let mut queue = VecDeque::new();
    for (index, &pixel) in pixels.iter().enumerate() {
        if pixel == 0 {
            distance[index] = 0;
            queue.push_back(index);
        }
    }
    while let Some(index) = queue.pop_front() {
        let (row, col) = (index / width, index % width);
        let next_distance = distance[index] + 1;
        let neighbors = [
            (row > 0).then(|| index - width),
            (row + 1 < height).then(|| index + width),
            (col > 0).then(|| index - 1),
            (col + 1 < width).then(|| index + 1),
        ];
        for next in neighbors.into_iter().flatten() {
            if distance[next] == sentinel {
                distance[next] = next_distance;
                queue.push_back(next);
            }
        }
    }
    Ok(distance)
}

#[cfg(test)]
mod tests {
    use super::binary_distance_transform as distance;
    #[test]
    fn examples() {
        assert_eq!(distance(&[1,1,1,1,0,1,1,1,1],3,3),
                   Ok(vec![2,1,2,1,0,1,2,1,2]));
        assert_eq!(distance(&[0,1,1,1],2,2),Ok(vec![0,1,1,2]));
        assert_eq!(distance(&[1,1,1,1],2,2),Ok(vec![4,4,4,4]));
        assert_eq!(distance(&[0,0,0,0],2,2),Ok(vec![0,0,0,0]));
        assert_eq!(distance(&[0],1,1),Ok(vec![0]));
        assert_eq!(distance(&[1],1,1),Ok(vec![2]));
        assert_eq!(distance(&[1,0,1,1],1,4),Ok(vec![1,0,1,2]));
        assert_eq!(distance(&[1,0,1,1],4,1),Ok(vec![1,0,1,2]));
    }
    #[test]
    fn maximum() {
        let mut mask=vec![1u8;4096];mask[0]=0;
        let result=distance(&mask,64,64).unwrap();
        assert_eq!(result[0],0);
        assert_eq!(result[1],1);
        assert_eq!(result[64],1);
        assert_eq!(result[4095],126);
        assert_eq!(distance(&[1u8;4096],64,64),Ok(vec![128;4096]));
    }
    #[test]
    fn invalid() {
        assert!(distance(&[1],0,1).is_err());
        assert!(distance(&[1],1,0).is_err());
        assert!(distance(&[1],65,1).is_err());
        assert!(distance(&[1],1,65).is_err());
        assert!(distance(&[1,0],1,1).is_err());
        assert!(distance(&[0,2],1,2).is_err());
    }
}
