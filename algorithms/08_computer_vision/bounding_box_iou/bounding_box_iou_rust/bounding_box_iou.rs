//! Intersection over union of two valid axis-aligned boxes.

fn valid(box_coords: &[f64; 4]) -> bool {
    box_coords.iter().all(|v| v.is_finite() && v.abs() <= 1_000_000.0)
        && box_coords[0] < box_coords[2] && box_coords[1] < box_coords[3]
}

/// Calculate IoU of [xmin,ymin,xmax,ymax] bounding boxes.
pub fn bounding_box_iou(a: &[f64; 4], b: &[f64; 4]) -> Result<f64, &'static str> {
    if !valid(a) || !valid(b) {
        return Err("invalid bounding box coordinates");
    }
    let width = (a[2].min(b[2]) - a[0].max(b[0])).max(0.0);
    let height = (a[3].min(b[3]) - a[1].max(b[1])).max(0.0);
    let intersection = width * height;
    let area_a = (a[2] - a[0]) * (a[3] - a[1]);
    let area_b = (b[2] - b[0]) * (b[3] - b[1]);
    Ok(intersection / (area_a + area_b - intersection))
}

#[cfg(test)]
mod tests {
    use super::bounding_box_iou as iou;

    #[test]
    fn overlap() {
        assert!((iou(&[0.0, 0.0, 2.0, 2.0], &[1.0, 1.0, 3.0, 3.0]).unwrap() - 1.0 / 7.0).abs() < 1e-12);
    }

    #[test]
    fn boundaries() {
        let a = [0.0, 0.0, 2.0, 2.0];
        assert_eq!(iou(&a, &a), Ok(1.0));
        assert_eq!(iou(&a, &[2.0, 0.0, 3.0, 1.0]), Ok(0.0));
        assert_eq!(iou(&a, &[4.0, 4.0, 5.0, 5.0]), Ok(0.0));
    }

    #[test]
    fn invalid() {
        let a = [0.0, 0.0, 2.0, 2.0];
        assert!(iou(&a, &[1.0, 0.0, 1.0, 2.0]).is_err());
        assert!(iou(&a, &[0.0, f64::NAN, 2.0, 2.0]).is_err());
        assert!(iou(&a, &[0.0, 0.0, 1000001.0, 2.0]).is_err());
    }
}
