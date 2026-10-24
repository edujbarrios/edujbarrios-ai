import math
import unittest
from floyd_warshall import floyd_warshall

class FloydWarshallTests(unittest.TestCase):
    def test_shortest_paths(self):
        inf=math.inf
        graph=[[0,3,inf],[inf,0,-2],[4,inf,0]]
        self.assertEqual(floyd_warshall(graph),
                         [[0,3,1],[2,0,-2],[4,7,0]])
        self.assertEqual(floyd_warshall([[0]]),[[0]])
        disconnected=floyd_warshall([[0,inf],[inf,0]])
        self.assertEqual(disconnected,[[0,inf],[inf,0]])
        self.assertEqual(graph,[[0,3,inf],[inf,0,-2],[4,inf,0]])
    def test_invalid(self):
        inf=math.inf
        for graph in ([],[[0,1]],[[0,-2],[-2,0]],
                      [[1,inf],[inf,0]],[[0,math.nan],[inf,0]],
                      [[0,-inf],[inf,0]],[[0,1e6+1],[inf,0]],
                      [[0,True],[inf,0]]):
            with self.subTest(graph=graph),self.assertRaises(ValueError):
                floyd_warshall(graph)

if __name__=="__main__": unittest.main()
