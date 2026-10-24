import unittest
from shannon_entropy import shannon_entropy

class ShannonEntropyTests(unittest.TestCase):
    def test_known_distributions(self):
        self.assertEqual(shannon_entropy([1,0]),0)
        self.assertAlmostEqual(shannon_entropy([.5,.5]),1)
        self.assertAlmostEqual(shannon_entropy([.25]*4),2)
        self.assertAlmostEqual(shannon_entropy([.75,.25]),.8112781244591328)
        self.assertEqual(shannon_entropy([1]),0)
    def test_invalid(self):
        for p in ([],[.2,.2],[1.1,-.1],[float("nan"),1],
                  [float("inf"),0],[True,0],["1"],[0]):
            with self.subTest(p=p),self.assertRaises(ValueError):
                shannon_entropy(p)

if __name__=="__main__":unittest.main()
