import unittest
from entropy_impurity import entropy_impurity as entropy


class EntropyTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(entropy([5,0]),0)
        self.assertAlmostEqual(entropy([1,1]),1)
        self.assertAlmostEqual(entropy([1,1,1,1]),2)
        self.assertAlmostEqual(entropy([3,1]),0.8112781244591328)
        self.assertEqual(entropy([1000000]),0)
        self.assertAlmostEqual(entropy([1000000,1000000]),1)

    def test_invalid(self):
        cases=[[],[0],[0,0],[-1,1],[1000001,0],[1.5,1],
               [float('nan'),1],[float('inf'),1],[True,1],
               ['1',2],[1+0j,2],[1]*129]
        for counts in cases:
            with self.subTest(counts=str(counts)[:30]):
                with self.assertRaises(ValueError):
                    entropy(counts)


if __name__=="__main__":
    unittest.main()
