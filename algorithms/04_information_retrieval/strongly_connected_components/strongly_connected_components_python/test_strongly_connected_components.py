import unittest
from strongly_connected_components import strongly_connected_components as scc


class StronglyConnectedComponentsTests(unittest.TestCase):
    def test_graphs(self):
        graph = [
            [0,1,0,0,0,0], [0,0,1,0,0,0],
            [1,0,0,1,0,0], [0,0,0,0,1,0],
            [0,0,0,1,0,1], [0,0,0,0,0,0]
        ]
        self.assertEqual(scc(graph), [0,0,0,3,3,5])
        self.assertEqual(scc([[0,1],[1,0]]), [0,0])
        self.assertEqual(scc([[0,1],[0,0]]), [0,1])
        self.assertEqual(scc([[1]]), [0])
        self.assertEqual(scc([[0]]), [0])
        self.assertEqual(scc([[0,0,0],[0,0,0],[0,0,0]]), [0,1,2])

    def test_invalid(self):
        cases = [
            [], [[0,1],[1]], [[0,2],[0,0]],
            [[0,-1],[0,0]], [[0,float('nan')],[0,0]],
            [[0,float('inf')],[0,0]], [[0,True],[0,0]],
            [[0,0.5],[0,0]], [[0,'1'],[0,0]],
            [[0,1j],[0,0]], [[0]*129 for _ in range(129)]
        ]
        for graph in cases:
            with self.subTest(n=len(graph)):
                with self.assertRaises(ValueError):
                    scc(graph)


if __name__ == '__main__':
    unittest.main()
