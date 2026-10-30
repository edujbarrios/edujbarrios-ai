import math
import unittest
from gradient_clip_by_value import gradient_clip_by_value as clip

class GradientClipValueTests(unittest.TestCase):
    def test_fixtures(self):
        self.assertEqual(clip([-3,-0.5,0,0.5,3],1),[-1,-0.5,0,0.5,1])
        self.assertEqual(clip([-1,0,1],1),[-1,0,1])
        self.assertEqual(clip([-1e308,1e308],1),[-1,1])
        self.assertEqual(clip([0],2),[0])

    def test_invalid(self):
        for gradients,bound in (([],1),([math.nan],1),([math.inf],1),
                                ([True],1),(["1"],1),([1],0),([1],-1),
                                ([1],math.inf),([1],True),([10**400],1)):
            with self.subTest(gradients=gradients,bound=bound), self.assertRaises(ValueError):
                clip(gradients,bound)

if __name__ == "__main__":
    unittest.main()
