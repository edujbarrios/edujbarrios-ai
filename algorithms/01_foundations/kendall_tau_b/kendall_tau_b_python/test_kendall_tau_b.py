import unittest
from kendall_tau_b import kendall_tau_b as tau


class KendallTauTests(unittest.TestCase):
    def test_values(self):
        self.assertAlmostEqual(tau([1,2,3], [2,3,4]), 1.0)
        self.assertAlmostEqual(tau([1,2,3], [4,3,2]), -1.0)
        self.assertAlmostEqual(tau([1,2,3], [1,3,2]), 1/3)
        self.assertAlmostEqual(tau([1,1,2,3], [1,2,2,3]), 0.8)
        self.assertAlmostEqual(tau([1,2], [1,2]), 1.0)

    def test_errors(self):
        cases = [
            ([],[]), ([1],[2]), ([1,2],[1]),
            ([1,2,3],[7,7,7]), ([7,7,7],[1,2,3]),
            ([1,2,3],[1,float("nan"),3]), ([1,2,3],[1,float("inf"),3]),
            ([1,2,3],[1,1000001,3]), ([1,True],[1,2]),
            ([1,2j],[1,2]), (list(range(129)),list(range(129)))
        ]
        for x,y in cases:
            with self.subTest(n=len(x)):
                with self.assertRaises(ValueError):
                    tau(x,y)


if __name__ == "__main__":
    unittest.main()
