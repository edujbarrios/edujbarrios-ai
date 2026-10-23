import unittest
from f1_score import f1_score

class F1Tests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(f1_score([0,0,1,1,1],[0,1,1,0,1]),2/3)
        self.assertEqual(f1_score([0,0],[0,0]),0.0)
        self.assertEqual(f1_score([1,1],[1,1]),1.0)
        self.assertEqual(f1_score([1,0],[0,1]),0.0)
        self.assertAlmostEqual(f1_score([1,1],[0,1]),2/3)
    def test_invalid(self):
        for actual,pred in (([],[]),([1],[]),([1],[1,0]),
                            ([0,2],[0,1]),([0,1],[0,2]),
                            ([True,0],[0,0]),([0,1],[0,False]),
                            ([0,1.0],[0,1])):
            with self.subTest(actual=actual,pred=pred),self.assertRaises(ValueError):
                f1_score(actual,pred)

if __name__=="__main__":unittest.main()
