import unittest
from roc_auc import roc_auc

class RocAucTests(unittest.TestCase):
    def test_ranking(self):
        self.assertAlmostEqual(roc_auc([.1,.4,.35,.8],[0,0,1,1]),.75)
        self.assertEqual(roc_auc([.5,.5],[0,1]),.5)
        self.assertEqual(roc_auc([.9,.1],[0,1]),0.0)
        self.assertEqual(roc_auc([.1,.9],[0,1]),1.0)
        self.assertEqual(roc_auc([0,0,0,1],[0,1,0,1]),.75)
    def test_invalid(self):
        for s,y in (([],[]),([.5],[1]),([.1,.9],[1,1]),
                    ([.1,.9],[0,0]),([.1],[0,1]),
                    ([float("nan"),.9],[0,1]),([.1,.9],[0,2]),
                    ([True,.9],[0,1]),([.1,.9],[False,1])):
            with self.subTest(scores=s,labels=y),self.assertRaises(ValueError):
                roc_auc(s,y)

if __name__=="__main__":unittest.main()
