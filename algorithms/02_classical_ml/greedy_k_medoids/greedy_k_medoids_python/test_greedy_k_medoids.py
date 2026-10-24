import unittest
from greedy_k_medoids import greedy_k_medoids as select

class KMedoidsTests(unittest.TestCase):
    def test_medoids_and_ties(self):
        x=[[0],[1],[9],[10]]
        self.assertEqual(select(x,1),[1])
        self.assertEqual(select(x,2),[1,2])
        self.assertEqual(select(x,3),[1,2,0])
        self.assertEqual(select(x,4),[1,2,0,3])
        self.assertEqual(select([[0],[0],[0]],3),[0,1,2])
        self.assertEqual(select([[5]],1),[0])
    def test_invalid(self):
        cases=(([],1),([[]],1),([[0]],0),([[0]],2),
               ([[1],[1,2]],1),([[float("nan")],[1]],1),
               ([[True],[1]],1),([[-1.7e308],[1.7e308]],1))
        for x,k in cases:
            with self.subTest(x=x,k=k),self.assertRaises(ValueError):
                select(x,k)

if __name__=="__main__":unittest.main()
