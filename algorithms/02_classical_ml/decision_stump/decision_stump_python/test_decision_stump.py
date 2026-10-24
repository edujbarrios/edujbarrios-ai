import unittest
from decision_stump import decision_stump, decision_stump_predict

class DecisionStumpTests(unittest.TestCase):
    def test_orientations(self):
        self.assertEqual(decision_stump([0,1,2,3],[0,0,1,1]),(1.0,True,0))
        self.assertEqual(decision_stump([0,1,2,3],[1,1,0,0]),(1.0,False,0))
        self.assertEqual(decision_stump([3,0,2,1],[1,0,1,0]),(1.0,True,0))
        self.assertEqual(decision_stump_predict(2,1,True),1)
        self.assertEqual(decision_stump_predict(2,1,False),0)
    def test_constant(self):
        self.assertEqual(decision_stump([4,4],[1,1]),(4.0,False,0))
        self.assertEqual(decision_stump([4,4],[0,0]),(4.0,True,0))
        self.assertEqual(decision_stump([4,4],[0,1]),(4.0,True,1))
    def test_invalid(self):
        for x,y in (([],[]),([0],[]),([0],[2]),([0],[True]),
                    ([float("nan")],[0]),([True],[0])):
            with self.subTest(x=x,y=y),self.assertRaises(ValueError):
                decision_stump(x,y)
        for x,t,p in ((float("inf"),1,True),(0,float("nan"),True),(0,1,1)):
            with self.assertRaises(ValueError):
                decision_stump_predict(x,t,p)

if __name__=="__main__": unittest.main()
