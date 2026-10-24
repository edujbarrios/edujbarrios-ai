import math
import unittest
from kullback_leibler_divergence import kullback_leibler_divergence as kl

class KLDivergenceTests(unittest.TestCase):
    def test_finite_and_infinite(self):
        self.assertEqual(kl([.5,.5],[.5,.5]),0)
        self.assertAlmostEqual(kl([1,0],[.5,.5]),1)
        self.assertAlmostEqual(kl([.75,.25],[.5,.5]),.1887218755408672)
        self.assertEqual(kl([1,0],[0,1]),math.inf)
        self.assertEqual(kl([1],[1]),0)
        self.assertAlmostEqual(kl([0,1],[.5,.5]),1)
        self.assertNotAlmostEqual(kl([.75,.25],[.5,.5]),kl([.5,.5],[.75,.25]))
    def test_invalid(self):
        for a,b in (([],[]),([.5,.5],[.2,.2]),
                    ([1,0],[1]),([1.1,-.1],[.5,.5]),
                    ([.5,.5],[math.nan,1]),([True,0],[1,0])):
            with self.subTest(a=a,b=b),self.assertRaises(ValueError):
                kl(a,b)

if __name__=="__main__":unittest.main()
