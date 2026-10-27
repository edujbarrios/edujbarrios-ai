import math
import unittest

from bellman_ford_shortest_path import bellman_ford_shortest_path as shortest


class BellmanFordTests(unittest.TestCase):
    def test_negative_edges_and_unreachable(self):
        graph = [(0, 1, 4), (0, 2, 5), (1, 2, -2), (2, 3, 3)]
        self.assertEqual(shortest(graph, 5, 0)[:4], [0, 4, 2, 5])
        self.assertTrue(math.isinf(shortest(graph, 5, 0)[4]))
        self.assertEqual(shortest(graph, 5, 4)[4], 0)
        self.assertEqual(shortest([(0, 1, 0), (0, 1, 5), (1, 2, -1)], 3, 0),
                         [0, 0, -1])
        self.assertEqual(shortest([], 1, 0), [0])
        self.assertEqual(shortest([(2, 3, -3), (3, 2, 1)], 4, 0)[0], 0)
        self.assertTrue(math.isinf(shortest([(2, 3, -3), (3, 2, 1)], 4, 0)[2]))

    def test_negative_cycles(self):
        with self.assertRaises(ValueError):
            shortest([(0, 1, 1), (1, 2, -3), (2, 1, 1)], 3, 0)
        with self.assertRaises(ValueError):
            shortest([(0, 0, -1)], 1, 0)

    def test_invalid_inputs(self):
        cases = [
            ([], 0, 0), ([], 129, 0), ([], 2, 2), ([], True, 0),
            ([(0, 3, 1)], 3, 0), ([(-1, 0, 1)], 2, 0),
            ([(True, 0, 1)], 2, 0), ([(0, 1, float("nan"))], 2, 0),
            ([(0, 1, math.inf)], 2, 0), ([(0, 1, 1000001)], 2, 0),
            ([(0, 1, True)], 2, 0), ([(0, 1)], 2, 0),
            ([(0, 1, 0)] * 16385, 2, 0),
        ]
        for edges, n, source in cases:
            with self.subTest(n=n, source=source, edges=str(edges)[:40]):
                with self.assertRaises(ValueError):
                    shortest(edges, n, source)


if __name__ == "__main__":
    unittest.main()
