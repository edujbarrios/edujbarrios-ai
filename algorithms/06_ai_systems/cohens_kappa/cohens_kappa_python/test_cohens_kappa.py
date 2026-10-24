import unittest
from cohens_kappa import cohens_kappa as kappa

class KappaTests(unittest.TestCase):
    def test_agreement(self):
        self.assertAlmostEqual(kappa([0,0,1,1],[0,0,0,1]),.5)
        self.assertEqual(kappa([0,1],[1,0]),-1)
        self.assertEqual(kappa([0,1],[0,1]),1)
        self.assertEqual(kappa([0,0,1,1],[0,1,0,1]),0)
        self.assertEqual(kappa([0,0],[1,1]),0)
    def test_invalid(self):
        for a,b in (([],[]),([0],[]),([0,0],[0,0]),
                    ([1,1],[1,1]),([0,2],[0,1]),([True,1],[0,1]),
                    ([0,1],[0,.5])):
            with self.subTest(a=a,b=b),self.assertRaises(ValueError):
                kappa(a,b)

if __name__=="__main__":unittest.main()
