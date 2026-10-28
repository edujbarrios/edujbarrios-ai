import unittest
from bfs_shortest_path import bfs_shortest_path as bfs


class BFSShortestPathTests(unittest.TestCase):
    def test_reference(self):
        graph = [
            [0,1,1,0,0,0], [0,0,0,1,0,0], [0,0,0,1,0,0],
            [0,0,0,0,1,0], [0,0,0,0,0,0], [0,0,0,0,0,0]
        ]
        self.assertEqual(bfs(graph, 0), [0,1,1,2,3,-1])
        self.assertEqual(bfs(graph, 5), [-1,-1,-1,-1,-1,0])
        self.assertEqual(bfs([[1,1],[1,0]],0), [0,1])
        self.assertEqual(bfs([[1]],0), [0])
        self.assertEqual(bfs([[0]],0), [0])

    def test_invalid(self):
        cases = [
            ([],0), ([[0,1],[1]],0), ([[0]],1),
            ([[0]],True), ([[0,2],[0,0]],0), ([[0,-1],[0,0]],0),
            ([[0,float("nan")],[0,0]],0), ([[0,0.5],[0,0]],0),
            ([[0,True],[0,0]],0), ([[0,"1"],[0,0]],0),
            ([[0,1j],[0,0]],0), ([[0]*129 for _ in range(129)],0)
        ]
        for graph, source in cases:
            with self.subTest(n=len(graph),source=source):
                with self.assertRaises(ValueError):
                    bfs(graph,source)


if __name__ == "__main__":
    unittest.main()
