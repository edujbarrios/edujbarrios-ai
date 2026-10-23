import unittest
from scalar_quantization import scalar_quantization as q

class QuantizationTests(unittest.TestCase):
    def test_endpoints_and_saturation(self):
        self.assertEqual(q([0.0, 0.5, 1.0, -3.0, 8.0], 0, 1),
                         [0, 128, 255, 0, 255])
        self.assertEqual(q([], 0, 1), [])
    def test_invalid(self):
        for values, low, high in (([float("nan")], 0, 1),
                                  ([float("inf")], 0, 1),
                                  ([0], 1, 1),
                                  ([0], -1e308, 1e308),
                                  ([0], 0, float("inf")),
                                  ([True], 0, 1)):
            with self.subTest(values=values, low=low, high=high):
                with self.assertRaises(ValueError):
                    q(values, low, high)
if __name__ == "__main__":
    unittest.main()
