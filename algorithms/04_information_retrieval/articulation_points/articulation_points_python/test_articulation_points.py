import unittest
from articulation_points import articulation_points as cut


def graph(n, edges):
    a = [0] * (n * n)
    for u, v in edges:
        a[u*n + v] = a[v*n + u] = 1
    return a


class ArticulationPointTests(unittest.TestCase):
    def test_graphs(self):
        self.assertEqual(cut(graph(5, [(i,i+1) for i in range(4)]),5),[1,2,3])
        self.assertEqual(cut(graph(4, [(0,1),(1,2),(2,3),(3,0)]),4),[])
        self.assertEqual(cut(graph(5, [(0,i) for i in range(1,5)]),5),[0])
        self.assertEqual(cut(graph(6, [(0,1),(1,2),(3,4)]),6),[1])
        self.assertEqual(cut(graph(4, [(i,j) for i in range(4)
                                        for j in range(i+1,4)]),4),[])
        self.assertEqual(cut(graph(1, []),1),[])
        self.assertEqual(cut(graph(2, [(0,1)]),2),[])
        self.assertEqual(cut(graph(3, []),3),[])
        self.assertEqual(cut(tuple(graph(3, [(0,1),(1,2)])),3),[1])

    def test_maximum(self):
        a = graph(64, [(i,i+1) for i in range(63)])
        self.assertEqual(cut(a,64),list(range(1,63)))

    def test_invalid(self):
        for adj, n in [
            ([],0),([0],65),([0],True),([0],1.0),
            ([],1),([0,1],1),([1],1),([0,1,0,0],2),
            ([0,2,2,0],2),([0,True,True,0],2),
            ([0.0],1),(None,1),("0",1),
        ]:
            with self.subTest(adj=adj,n=n), self.assertRaises(ValueError):
                cut(adj,n)


if __name__ == "__main__":
    unittest.main()
