import math
import unittest
from perplexity import perplexity

class PerplexityTests(unittest.TestCase):
    def test_probability_fixtures(self):
        self.assertAlmostEqual(perplexity([math.log(.5),math.log(.25)]),math.sqrt(8))
        self.assertAlmostEqual(perplexity([math.log(.25)]*2),4)
        self.assertEqual(perplexity([0,0,0]),1)
        self.assertAlmostEqual(perplexity([math.log(.5)]),2)
        self.assertAlmostEqual(perplexity([math.log(.5)]*100),2)
    def test_invalid(self):
        for data in ([],[math.nan],[math.inf],[-math.inf],[.1],
                     [-1000],[True]):
            with self.subTest(data=data),self.assertRaises(ValueError):
                perplexity(data)

if __name__=="__main__":unittest.main()
