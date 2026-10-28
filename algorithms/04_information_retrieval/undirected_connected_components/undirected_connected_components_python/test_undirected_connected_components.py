import unittest
from undirected_connected_components import undirected_connected_components as cc


class ComponentsTests(unittest.TestCase):
    def test_graphs(self):
        graph=[
            [0,0,1,0,0,0],[0,0,0,0,1,0],
            [1,0,0,1,0,0],[0,0,1,0,0,0],
            [0,1,0,0,0,0],[0,0,0,0,0,0],
        ]
        self.assertEqual(cc(graph),[0,1,0,0,1,5])
        self.assertEqual(cc([[1,1],[1,0]]),[0,0])
        self.assertEqual(cc([[0,0],[0,0]]),[0,1])
        self.assertEqual(cc([[1]]),[0])

    def test_invalid(self):
        cases=[
            [],[[0,1],[1]],[[0,1],[0,0]],
            [[0,2],[2,0]],[[0,-1],[-1,0]],
            [[0,float("nan")],[0,0]],[[0,float("inf")],[0,0]],
            [[0,True],[True,0]],[[0,"1"],["1",0]],
            [[0,0.5],[0.5,0]],[[0]*129 for _ in range(129)]
        ]
        for graph in cases:
            with self.subTest(n=len(graph)):
                with self.assertRaises(ValueError): cc(graph)


if __name__=="__main__":
    unittest.main()
