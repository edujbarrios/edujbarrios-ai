import unittest
from undirected_graph_bridges import undirected_graph_bridges as bridges


def graph(n, edges):
    a = [0]*(n*n)
    for u,v in edges:
        a[u*n+v] = a[v*n+u] = 1
    return a


class BridgeTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(bridges(graph(4,[(0,1),(1,2),(2,3)]),4),
                         [0,1,1,2,2,3])
        self.assertEqual(bridges(graph(4,[(0,1),(1,2),(2,3),(3,0)]),4),[])
        self.assertEqual(bridges(graph(4,[(0,1),(0,2),(0,3)]),4),
                         [0,1,0,2,0,3])
        self.assertEqual(bridges(graph(5,[(0,1),(1,2),(2,0),(2,3),(3,4)]),5),
                         [2,3,3,4])
        self.assertEqual(bridges(graph(7,[(0,1),(1,2),(3,4),(4,5),(5,3)]),7),
                         [0,1,1,2])
        self.assertEqual(bridges(graph(4,[(i,j) for i in range(4)
                                         for j in range(i+1,4)]),4),[])
        self.assertEqual(bridges(graph(1,[]),1),[])
        self.assertEqual(bridges(graph(2,[(0,1)]),2),[0,1])
        self.assertEqual(bridges(graph(3,[]),3),[])
        self.assertEqual(bridges(tuple(graph(3,[(0,1),(1,2)])),3),[0,1,1,2])

    def test_maximum(self):
        got = bridges(graph(64,[(i,i+1) for i in range(63)]),64)
        expected = [vertex for i in range(63) for vertex in (i,i+1)]
        self.assertEqual(got,expected)

    def test_invalid(self):
        for a,n in [
            ([],0),([0],65),([0],True),([0],1.5),([],1),
            ([1],1),([0,1],1),([0,1,0,0],2),([0,2,2,0],2),
            ([0.0],1),([True],1),([0,True,True,0],2),
            (None,1),("0",1),
        ]:
            with self.subTest(a=a,n=n), self.assertRaises(ValueError):
                bridges(a,n)


if __name__ == "__main__":
    unittest.main()
