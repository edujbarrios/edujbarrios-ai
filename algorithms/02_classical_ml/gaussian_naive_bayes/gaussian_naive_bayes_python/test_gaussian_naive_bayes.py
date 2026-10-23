import unittest
from gaussian_naive_bayes import gaussian_naive_bayes as classify

class GaussianNBTests(unittest.TestCase):
    def test_separation_and_tie(self):
        x=[[0,0],[0,1],[5,5],[5,6]]
        y=[0,0,1,1]
        label,p=classify(x,y,[5,5])
        self.assertEqual(label,1)
        self.assertGreater(p,0.99)
        label,p=classify(x,y,[0,0])
        self.assertEqual(label,0)
        self.assertLess(p,0.01)
        self.assertEqual(classify([[0],[2]],[0,1],[1]),(0,0.5))
    def test_invalid(self):
        for x,y,q in (([],[],[0]), ([[0],[1]],[0,0],[0]),
                      ([[0],[1]],[0,2],[0]), ([[0],[1]],[0,1],[]),
                      ([[0],[1,2]],[0,1],[0]), ([[float("nan")],[1]],[0,1],[0]),
                      ([[0],[1]],[0,True],[0]), ([[0],[1]],[0,1],[float("inf")])):
            with self.subTest(x=x,y=y,q=q),self.assertRaises(ValueError):
                classify(x,y,q)

if __name__=="__main__": unittest.main()
