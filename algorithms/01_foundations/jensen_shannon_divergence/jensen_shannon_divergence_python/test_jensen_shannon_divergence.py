import unittest
from jensen_shannon_divergence import jensen_shannon_divergence as jsd

class JSDTests(unittest.TestCase):
    def test_values(self):
        self.assertAlmostEqual(jsd([1,0],[0,1]),1)
        self.assertAlmostEqual(jsd([.5,.5],[.5,.5]),0)
        self.assertAlmostEqual(jsd([1,0],[.5,.5]),.31127812445913283)
        self.assertEqual(jsd([1],[1]),0)
        self.assertAlmostEqual(jsd([0,1],[.5,.5]),jsd([.5,.5],[0,1]))
    def test_invalid(self):
        for a,b in (([],[]),([1],[.5]),([1,0],[1]),
                    ([1,0],[-.1,1.1]),([1,0],[float("nan"),1]),
                    ([1,0],[.2,.2]),([True,0],[1,0])):
            with self.subTest(a=a,b=b),self.assertRaises(ValueError):
                jsd(a,b)

if __name__=="__main__":unittest.main()
