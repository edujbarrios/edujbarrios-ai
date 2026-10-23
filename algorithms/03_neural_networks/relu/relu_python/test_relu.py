import unittest
from relu import relu


class ReluTests(unittest.TestCase):
    def test_regular(self):
        self.assertEqual(relu([-2, -0.5, 0, 3]), [0.0, 0.0, 0.0, 3.0])

    def test_empty(self):
        self.assertEqual(relu([]), [])

    def test_invalid(self):
        for bad in (float("nan"), float("inf"), True, "3"):
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                relu([1, bad])


if __name__ == "__main__":
    unittest.main()
