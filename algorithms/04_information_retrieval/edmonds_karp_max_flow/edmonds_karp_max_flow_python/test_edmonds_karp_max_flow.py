import unittest
from edmonds_karp_max_flow import edmonds_karp_max_flow as max_flow


def network(n, edges):
    a = [0]*(n*n)
    for u,v,c in edges:
        a[u*n+v] = c
    return a


CLASSIC = [
    (0,1,16),(0,2,13),(1,2,10),(2,1,4),
    (1,3,12),(2,4,14),(3,2,9),(4,3,7),
    (3,5,20),(4,5,4),
]


class FlowTests(unittest.TestCase):
    def test_examples(self):
        a = network(6,CLASSIC)
        original = a.copy()
        self.assertEqual(max_flow(a,6,0,5),23)
        self.assertEqual(a,original)
        self.assertEqual(max_flow(network(2,[(0,1,7)]),2,0,1),7)
        self.assertEqual(max_flow(network(2,[(0,1,7)]),2,1,0),0)
        self.assertEqual(max_flow(network(3,[(0,1,4),(1,2,2),(0,2,1)]),3,0,2),3)
        self.assertEqual(max_flow(network(3,[(0,1,4),(1,0,3),(1,2,2),(0,2,1)]),3,0,2),3)
        self.assertEqual(max_flow(network(4,[(0,1,6),(2,3,7)]),4,0,3),0)
        self.assertEqual(max_flow(network(3,[]),3,0,2),0)
        self.assertEqual(max_flow(tuple(network(2,[(0,1,1000)])),2,0,1),1000)

    def test_limits(self):
        edges = [(0,i,1000) for i in range(1,15)]
        edges += [(i,15,1000) for i in range(1,15)]
        self.assertEqual(max_flow(network(16,edges),16,0,15),14000)

    def test_invalid(self):
        for matrix,n,source,sink in [
            ([],1,0,0),([0],17,0,1),([],2,0,1),
            ([0,2,0,0],2,0,0),([0,2,0,0],2,0,2),
            ([0,2,0,0],2,-1,1),([0,2,0,0],2,True,1),
            ([0,2,0,0],2,0,1.0),([1,0,0,0],2,0,1),
            ([0,1001,0,0],2,0,1),([0,-1,0,0],2,0,1),
            ([0,True,0,0],2,0,1),([0,1.0,0,0],2,0,1),
            (None,2,0,1),("0000",2,0,1),
        ]:
            with self.subTest(n=n,source=source,sink=sink,matrix=matrix):
                with self.assertRaises(ValueError):
                    max_flow(matrix,n,source,sink)


if __name__ == "__main__":
    unittest.main()
