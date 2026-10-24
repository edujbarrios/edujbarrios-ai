import unittest
from stratified_k_fold import stratified_k_fold as split

class StratifiedKFoldTests(unittest.TestCase):
    def test_assignment_and_balancing(self):
        self.assertEqual(split([0,1,0,1,0,1],3),[0,0,1,1,2,2])
        self.assertEqual(split([0,0,0,0,1,1,1,1],3),[0,1,2,0,1,2,0,1])
        labels=[0]*7+[1]*5
        folds=split(labels,4)
        for k in range(4):
            self.assertGreaterEqual(sum(y==0 and f==k for y,f in zip(labels,folds)),1)
            self.assertGreaterEqual(sum(y==1 and f==k for y,f in zip(labels,folds)),1)
        sizes=[folds.count(i) for i in range(4)]
        self.assertLessEqual(max(sizes)-min(sizes),1)
    def test_invalid(self):
        cases=(([],2),([0,1],1),([0,1],2),
               ([0,0,1,1],3),([0,0,1,2],2),
               ([False,0,1,1],2),([0,0,1,1],True))
        for labels,k in cases:
            with self.subTest(labels=labels,k=k),self.assertRaises(ValueError):
                split(labels,k)

if __name__=="__main__":unittest.main()
