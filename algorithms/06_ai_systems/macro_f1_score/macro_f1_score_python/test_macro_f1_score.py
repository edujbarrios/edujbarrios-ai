import unittest
from macro_f1_score import macro_f1_score as macro


class MacroF1Tests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(macro([0,1,2,2],[0,2,1,2],3),0.5)
        self.assertAlmostEqual(macro([0,0],[0,0],3),1/3)
        self.assertEqual(macro([0,1,2],[0,1,2],3),1.0)
        self.assertEqual(macro([0,0],[1,1],2),0.0)
        self.assertEqual(macro([0,1],[0,1],2),1.0)
        self.assertAlmostEqual(macro((0,1,2),(0,0,2),3),(2/3+0+1)/3)
        self.assertEqual(macro([0]*512,[0]*512,2),0.5)
        self.assertEqual(macro([0]*512,[0]*512,16),1/16)
        self.assertEqual(macro([15],[15],16),1/16)
        self.assertEqual(macro([1],[0],2),0.0)
        self.assertEqual(macro([0],[0],2),0.5)
        self.assertAlmostEqual(macro([2,0,1,1],[2,1,0,1],3),0.5)  # label permutation

    def test_invalid(self):
        for actual,pred,classes in [
            ([],[],2),([0],[],2),([], [0],2),
            ([0],[0],1),([0],[0],17),([0],[0],True),([0],[0],2.0),
            ([0],[0],None),([2],[0],2),([0],[2],2),([-1],[0],2),
            ([0],[-1],2),([True],[0],2),([0],[False],2),
            ([0.0],[0],2),([0],[0.0],2),([None],[0],2),
            (None,[0],2),([0],None,2),("0",[0],2),
            ([0]*513,[0]*513,2),
        ]:
            with self.subTest(case=str((actual,pred,classes))[:60]):
                with self.assertRaises(ValueError):
                    macro(actual,pred,classes)


if __name__ == "__main__":
    unittest.main()
