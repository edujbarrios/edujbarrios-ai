import unittest
from prim_mst import prim_mst


class PrimMstTests(unittest.TestCase):
    def test_reference_cases(self):
        graph = [
            [0, 1, 4, 0],
            [1, 0, 2, 5],
            [4, 2, 0, 3],
            [0, 5, 3, 0],
        ]
        self.assertEqual(prim_mst(graph), 6)
        self.assertEqual(prim_mst([[0, 1, 1], [1, 0, 1], [1, 1, 0]]), 2)
        self.assertEqual(prim_mst([[0]]), 0)

    def test_invalid(self):
        cases = [
            [], [[0, 1], [1]], [[0, 0], [0, 0]],
            [[0, 1], [2, 0]], [[1, 1], [1, 0]],
            [[0, -1], [-1, 0]],
            [[0, float("nan")], [float("nan"), 0]],
            [[0, float("inf")], [float("inf"), 0]],
            [[0, 1000001], [1000001, 0]],
            [[0, True], [True, 0]],
            [[0, "1"], ["1", 0]],
            [[0, 2j], [2j, 0]],
            [[0] * 129 for _ in range(129)],
        ]
        for graph in cases:
            with self.subTest(n=len(graph)):
                with self.assertRaises(ValueError):
                    prim_mst(graph)


if __name__ == "__main__":
    unittest.main()
