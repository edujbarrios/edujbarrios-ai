import unittest
from binary_connected_components import binary_connected_components as components


class ComponentsTests(unittest.TestCase):
    def test_regions(self):
        self.assertEqual(components([1, 1, 0, 0, 1, 0, 0, 0, 1], 3, 3),
                         ([1, 1, 0, 0, 1, 0, 0, 0, 2], 2))
        self.assertEqual(components([1, 0, 0, 1], 2, 2), ([1, 0, 0, 2], 2))
        self.assertEqual(components([1, 1, 1, 1], 2, 2), ([1, 1, 1, 1], 1))
        self.assertEqual(components([0, 0, 0, 0], 2, 2), ([0] * 4, 0))
        self.assertEqual(components([1, 0, 1], 1, 3), ([1, 0, 2], 2))
        self.assertEqual(components([1], 1, 1), ([1], 1))
        self.assertEqual(components([0], 1, 1), ([0], 0))

    def test_maximum_checkerboard(self):
        pixels = [(r + c) % 2 for r in range(64) for c in range(64)]
        labels, count = components(pixels, 64, 64)
        self.assertEqual(count, 2048)
        self.assertEqual(sorted(v for v in labels if v), list(range(1, 2049)))

    def test_invalid(self):
        for pixels, height, width in [
            ([1], 0, 1), ([1], 1, 0), ([1], 65, 1), ([1], 1, 65),
            ([1, 0], 1, 1), ([0, 2], 1, 2),
            ([True, 0], 1, 2), ([1.0, 0], 1, 2),
            ([1, None], 1, 2), (None, 1, 1),
            ([1], 1.0, 1), ([1], True, 1), ([1], 1, False),
        ]:
            with self.subTest(pixels=pixels, height=height, width=width):
                with self.assertRaises(ValueError):
                    components(pixels, height, width)


if __name__ == "__main__":
    unittest.main()
