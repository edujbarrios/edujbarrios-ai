import unittest
from bipartite_graph_check import bipartite_graph_check as check


class BipartiteTests(unittest.TestCase):
    def test_graphs(self):
        path=[[0,1,0],[1,0,1],[0,1,0]]
        triangle=[[0,1,1],[1,0,1],[1,1,0]]
        cycle4=[[0,1,0,1],[1,0,1,0],[0,1,0,1],[1,0,1,0]]
        remote=[[0,0,0,0],[0,0,1,1],[0,1,0,1],[0,1,1,0]]
        self.assertTrue(check(path))
        self.assertTrue(check(cycle4))
        self.assertTrue(check([[0]]))
        self.assertFalse(check(triangle))
        self.assertFalse(check(remote))
        self.assertFalse(check([[1]]))

    def test_invalid(self):
        bad=[
            [],[[0,1],[1]],[[0,1],[0,0]],[[0,2],[2,0]],
            [[0,-1],[-1,0]],[[0,float('nan')],[0,0]],
            [[0,float('inf')],[0,0]],[[0,True],[True,0]],
            [[0,0.5],[0.5,0]],[[0,'1'],['1',0]],
            [[0]*129 for _ in range(129)],
        ]
        for graph in bad:
            with self.subTest(n=len(graph)):
                with self.assertRaises(ValueError):
                    check(graph)


if __name__=="__main__":
    unittest.main()
