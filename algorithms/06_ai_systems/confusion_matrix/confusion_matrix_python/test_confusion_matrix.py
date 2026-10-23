import unittest
from confusion_matrix import confusion_matrix

class ConfusionMatrixTests(unittest.TestCase):
    def test_mixed_and_perfect(self):
        self.assertEqual(confusion_matrix([0,0,1,1,1],[0,1,1,0,1]),(1,1,1,2))
        self.assertEqual(confusion_matrix([0,1],[0,1]),(1,0,0,1))
        self.assertEqual(confusion_matrix([0,0],[0,0]),(2,0,0,0))
    def test_invalid(self):
        for a,p in (([],[]),([0],[0,1]),([0,2],[0,1]),
                    ([0,1],[0,2]),([0,True],[0,1]),
                    ([0,1],[0,False]),([0,1.0],[0,1])):
            with self.subTest(a=a,p=p),self.assertRaises(ValueError):
                confusion_matrix(a,p)

if __name__=="__main__": unittest.main()
