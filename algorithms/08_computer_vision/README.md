# Computer Vision

Image, geometry and detection algorithms underlying object detectors, vision
encoders and other learned or classical vision pipelines.

## Implemented algorithms (11)

| Algorithm | What it does |
| --- | --- |
| [`bilinear_resize`](bilinear_resize/README.md) | Resizes grayscale images with half-pixel bilinear interpolation. |
| [`binary_dice_coefficient`](binary_dice_coefficient/README.md) | Measures overlap between aligned foreground segmentation masks. |
| [`binary_iou`](binary_iou/README.md) | Measures pixelwise overlap between aligned binary segmentation masks. |
| [`bounding_box_iou`](bounding_box_iou/README.md) | Computes intersection over union for valid axis-aligned detection boxes. |
| [`binary_dilation`](binary_dilation/README.md) | Dilates a binary image using a 3×3 square structuring element. |
| [`binary_erosion`](binary_erosion/README.md) | Erodes binary masks with a 3×3 square kernel and zero padding. |
| [`gaussian_blur`](gaussian_blur/README.md) | Smooths grayscale images with a fixed normalized 3×3 Gaussian kernel. |
| [`median_filter`](median_filter/README.md) | Removes isolated image outliers using a 3×3 median. |
| [`non_max_suppression`](non_max_suppression/README.md) | Keeps high-confidence bounding boxes while suppressing overlapping detections. |
| [`otsu_threshold`](otsu_threshold/README.md) | Computes Otsu's optimal 8-bit global segmentation threshold. |
| [`sobel_edges`](sobel_edges/README.md) | Computes replicated-border Sobel gradient magnitudes on grayscale images. |

[← All sections](../../README.md)
