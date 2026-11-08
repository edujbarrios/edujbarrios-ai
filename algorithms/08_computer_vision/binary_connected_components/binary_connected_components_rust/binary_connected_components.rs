//! Four-connected binary component labeling using deterministic BFS.

/// Row-major labels (background zero) and count; 1-based component IDs.
pub fn binary_connected_components(
    pixels: &[u8], height: usize, width: usize,
) -> Result<(Vec<usize>, usize), &'static str> {
    if !(1..=64).contains(&height) || !(1..=64).contains(&width)
        || pixels.len() != height * width || pixels.iter().any(|&x| x > 1) {
        return Err("invalid binary mask or dimensions");
    }
    let n = height * width;
    let mut labels = vec![0usize; n];
    let mut count = 0usize;
    let mut queue = Vec::with_capacity(n);
    for start in 0..n {
        if pixels[start] == 0 || labels[start] != 0 {
            continue;
        }
        count += 1;
        labels[start] = count;
        queue.clear();
        queue.push(start);
        let mut head = 0usize;
        while head < queue.len() {
            let index = queue[head];
            head += 1;
            let (row, col) = (index / width, index % width);
            let neighbors = [
                (row > 0).then(|| index - width),
                (row + 1 < height).then(|| index + width),
                (col > 0).then(|| index - 1),
                (col + 1 < width).then(|| index + 1),
            ];
            for next in neighbors.into_iter().flatten() {
                if pixels[next] == 1 && labels[next] == 0 {
                    labels[next] = count;
                    queue.push(next);
                }
            }
        }
    }
    Ok((labels, count))
}

#[cfg(test)]
mod tests {
    use super::binary_connected_components as components;
    #[test]
    fn examples() {
        assert_eq!(components(&[1,1,0,0,1,0,0,0,1],3,3),
                   Ok((vec![1,1,0,0,1,0,0,0,2],2)));
        assert_eq!(components(&[1,0,0,1],2,2),Ok((vec![1,0,0,2],2)));
        assert_eq!(components(&[1,1,1,1],2,2),Ok((vec![1,1,1,1],1)));
        assert_eq!(components(&[0,0,0,0],2,2),Ok((vec![0;4],0)));
        assert_eq!(components(&[1,0,1],1,3),Ok((vec![1,0,2],2)));
        assert_eq!(components(&[1],1,1),Ok((vec![1],1)));
    }
    #[test]
    fn maximum_checkerboard() {
        let mask: Vec<u8> = (0..64).flat_map(|r| (0..64).map(move |c| ((r+c)%2) as u8)).collect();
        let (labels,count) = components(&mask,64,64).unwrap();
        assert_eq!(count,2048);
        let foreground: Vec<usize> = labels.into_iter().filter(|&x|x>0).collect();
        assert_eq!(foreground,(1..=2048).collect::<Vec<_>>());
    }
    #[test]
    fn invalid() {
        assert!(components(&[1],0,1).is_err());
        assert!(components(&[1],1,0).is_err());
        assert!(components(&[1],65,1).is_err());
        assert!(components(&[1],1,65).is_err());
        assert!(components(&[1,0],1,1).is_err());
        assert!(components(&[0,2],1,2).is_err());
    }
}
