import unittest
from topological_sort import topological_sort as sort


class TopologicalSortTests(unittest.TestCase):
    def test_dags(self):
        dag = [[0,1,1,0],[0,0,0,1],[0,0,0,1],[0,0,0,0]]
        self.assertEqual(sort(dag), [0,1,2,3])
        self.assertEqual(sort([[0,0,0],[0,0,0],[0,0,0]]), [0,1,2])
        self.assertEqual(sort([[0,0,0],[0,0,0],[1,0,0]]), [1,2,0])
        self.assertEqual(sort([[0]]), [0])

    def test_invalid(self):
        cases = [
            [], [[0,1],[1]], [[0,1],[1,0]], [[1]],
            [[0,2],[0,0]], [[0,-1],[0,0]],
            [[0,float("nan")],[0,0]], [[0,float("inf")],[0,0]],
            [[0,0.5],[0,0]], [[0,True],[0,0]],
            [[0,"1"],[0,0]], [[0,1j],[0,0]],
            [[0]*129 for _ in range(129)]
        ]
        for graph in cases:
            with self.subTest(n=len(graph)):
                with self.assertRaises(ValueError):
                    sort(graph)


if __name__ == "__main__":
    unittest.main()
