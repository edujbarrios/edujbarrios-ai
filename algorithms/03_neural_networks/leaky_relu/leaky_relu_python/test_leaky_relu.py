import unittest
from leaky_relu import leaky_relu

class LeakyReluTests(unittest.TestCase):
    def test_values(self):
        x = leaky_relu([-2.0, 0.0, 3.0], 0.1)
        self.assertAlmostEqual(x[0], -0.2)
        self.assertEqual(x[1:], [0.0, 3.0])
        self.assertEqual(leaky_relu([], 0.1), [])

    def test_invalid(self):
        for a in (0.0, 1.0, -1.0, float("nan"), True):
            with self.subTest(alpha=a), self.assertRaises(ValueError):
                leaky_relu([1.0], a)
        for x in (float("nan"), float("inf"), True, "5"):
            with self.subTest(value=x), self.assertRaises(ValueError):
                leaky_relu([x], 0.1)

if __name__ == "__main__":
    unittest.main()
