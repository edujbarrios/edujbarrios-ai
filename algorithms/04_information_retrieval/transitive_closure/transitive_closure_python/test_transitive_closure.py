import unittest
from transitive_closure import transitive_closure


class TransitiveClosureTests(unittest.TestCase):
    def test_reference(self):
        chain=[[0,1,0],[0,0,1],[0,0,0]]
        expected=[[1,1,1],[0,1,1],[0,0,1]]
        self.assertEqual(transitive_closure(chain),expected)
        self.assertEqual(transitive_closure([[0,1],[1,0]]),[[1,1],[1,1]])
        self.assertEqual(transitive_closure([[0,0],[0,0]]),[[1,0],[0,1]])
        self.assertEqual(transitive_closure([[1]]),[[1]])
        self.assertEqual(transitive_closure([[0]]),[[1]])
        self.assertEqual(chain,[[0,1,0],[0,0,1],[0,0,0]])

    def test_invalid(self):
        cases=[
            [],[[0,1],[1]],[[0,2],[0,0]],[[0,-1],[0,0]],
            [[0,float("nan")],[0,0]],[[0,float("inf")],[0,0]],
            [[0,True],[0,0]],[[0,"1"],[0,0]],[[0,1j],[0,0]],
            [[0,0.5],[0,0]],[[0]*65 for _ in range(65)]
        ]
        for graph in cases:
            with self.subTest(n=len(graph)):
                with self.assertRaises(ValueError):
                    transitive_closure(graph)


if __name__=="__main__":
    unittest.main()
