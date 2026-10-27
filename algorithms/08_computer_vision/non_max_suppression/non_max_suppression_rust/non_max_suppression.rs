//! Greedy class-agnostic intersection-over-union suppression.

#[derive(Clone, Copy, Debug)]
pub struct Box2D {
    pub x1: f64, pub y1: f64, pub x2: f64, pub y2: f64, pub score: f64,
}

fn iou(a: Box2D, b: Box2D) -> f64 {
    let width = (a.x2.min(b.x2) - a.x1.max(b.x1)).max(0.0);
    let height = (a.y2.min(b.y2) - a.y1.max(b.y1)).max(0.0);
    let intersection = width * height;
    let aa = (a.x2 - a.x1) * (a.y2 - a.y1);
    let ab = (b.x2 - b.x1) * (b.y2 - b.y1);
    intersection / (aa + ab - intersection)
}

/// Return selected indices ordered by score descending, then source index.
pub fn non_max_suppression(boxes: &[Box2D], threshold: f64)
    -> Result<Vec<usize>, &'static str> {
    if boxes.len() > 256 || !threshold.is_finite() ||
       !(0.0..=1.0).contains(&threshold) ||
       boxes.iter().any(|b| {
           [b.x1,b.y1,b.x2,b.y2,b.score].iter().any(|x| !x.is_finite()) ||
           [b.x1,b.y1,b.x2,b.y2].iter().any(|x| x.abs() > 1_000_000.0) ||
           b.x2 <= b.x1 || b.y2 <= b.y1 || !(0.0..=1.0).contains(&b.score)
       }) {
        return Err("invalid boxes or suppression threshold");
    }
    let mut discarded = vec![false; boxes.len()];
    let mut result = Vec::new();
    for _ in 0..boxes.len() {
        let mut best: Option<usize> = None;
        for i in 0..boxes.len() {
            if !discarded[i] && best.is_none_or(|j| boxes[i].score > boxes[j].score) {
                best = Some(i);
            }
        }
        let Some(selected) = best else { break; };
        result.push(selected);
        discarded[selected] = true;
        for i in 0..boxes.len() {
            if !discarded[i] && iou(boxes[selected],boxes[i]) > threshold {
                discarded[i] = true;
            }
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::{non_max_suppression as nms, Box2D};
    fn b(x1:f64,y1:f64,x2:f64,y2:f64,score:f64)->Box2D {
        Box2D{x1,y1,x2,y2,score}
    }
    #[test]
    fn reference_cases() {
        let boxes=[
            b(0.,0.,2.,2.,0.9),b(0.5,0.5,2.5,2.5,0.8),
            b(3.,3.,5.,5.,0.7),b(0.,0.,2.,2.,0.9),
            b(10.,10.,12.,12.,0.7)];
        assert_eq!(nms(&boxes,0.3),Ok(vec![0,2,4]));
        assert_eq!(nms(&boxes,0.4),Ok(vec![0,1,2,4]));
        assert_eq!(nms(&[b(0.,0.,1.,1.,1.),b(1.,0.,2.,1.,0.8)],0.),Ok(vec![0,1]));
        assert_eq!(nms(&[],1.),Ok(vec![]));
    }
    #[test]
    fn invalid() {
        assert!(nms(&[b(0.,0.,0.,2.,0.5)],0.5).is_err());
        assert!(nms(&[b(0.,0.,1.,1.,f64::NAN)],0.5).is_err());
        assert!(nms(&[b(0.,0.,1_000_001.,1.,0.5)],0.5).is_err());
        assert!(nms(&[b(0.,0.,1.,1.,0.5)],-0.1).is_err());
        assert!(nms(&[b(0.,0.,1.,1.,0.5)],f64::INFINITY).is_err());
        assert!(nms(&vec![b(0.,0.,1.,1.,0.5);257],0.5).is_err());
    }
}
