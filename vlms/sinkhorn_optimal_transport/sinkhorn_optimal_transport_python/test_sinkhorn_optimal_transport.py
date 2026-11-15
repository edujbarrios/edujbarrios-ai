import math
import unittest
from sinkhorn_optimal_transport import sinkhorn_optimal_transport as sinkhorn


class SinkhornTests(unittest.TestCase):
    def test_uniform_and_diagonal_alignment(self):
        uniform = sinkhorn([[0, 0], [0, 0]])
        self.assertTrue(uniform["converged"])
        self.assertEqual(uniform["iterations"], 1)
        for row in uniform["plan"]:
            for mass in row:
                self.assertAlmostEqual(mass, 0.25)
        costs = [[0, 2], [2, 0]]
        preferred = sinkhorn(costs)
        self.assertTrue(preferred["converged"])
        self.assertGreater(preferred["plan"][0][0], preferred["plan"][0][1])
        self.assertGreater(preferred["plan"][1][1], preferred["plan"][1][0])
        self.assertEqual(costs, [[0, 2], [2, 0]])

    def test_rectangular_and_convergence_diagnostic(self):
        result = sinkhorn([[0, 1, 2], [3, 1, 0]])
        self.assertTrue(result["converged"])
        for row in result["plan"]:
            self.assertAlmostEqual(sum(row), 0.5, places=7)
        for j in range(3):
            self.assertAlmostEqual(sum(row[j] for row in result["plan"]), 1 / 3, places=7)
        partial = sinkhorn([[0, 10], [1, 0]], max_iterations=1, tolerance=1e-12)
        self.assertEqual(partial["iterations"], 1)
        self.assertFalse(partial["converged"])
        self.assertGreater(partial["marginal_error"], 1e-12)

    def test_invalid_inputs(self):
        for costs, params in (
            ([], {}), ([[]], {}), ([[0], [0, 1]], {}),
            ([[True]], {}), ([[math.nan]], {}), ([[-1]], {}),
            ([[21]], {}), ([[0]], {"regularization": 0}),
            ([[0]], {"regularization": math.inf}),
            ([[0]], {"max_iterations": 0}),
            ([[0]], {"max_iterations": True}),
            ([[0]], {"tolerance": 0}),
            (None, {}),
        ):
            with self.subTest(costs=costs, params=params), self.assertRaises(ValueError):
                sinkhorn(costs, **params)


if __name__ == "__main__":
    unittest.main()
