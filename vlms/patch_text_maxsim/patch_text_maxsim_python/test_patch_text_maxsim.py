import unittest
from patch_text_maxsim import patch_text_maxsim as maxsim


class MaxSimTests(unittest.TestCase):
    def test_reference(self):
        patches=[1,0,0,1]; tokens=[1,0,.6,.8]
        ids,score=maxsim(patches,2,tokens,2,2)
        self.assertEqual(ids,[0,1])
        self.assertAlmostEqual(score,.9)
        ids,score=maxsim([1,0,1,0],2,[1,0],1,2)
        self.assertEqual(ids,[0])
        self.assertAlmostEqual(score,1)
        self.assertEqual(maxsim([0,1],1,[0,-1],1,2),([0],-1))
        ids,score=maxsim([2,0,0,3],2,tokens,2,2)
        self.assertEqual(ids,[0,1])
        self.assertAlmostEqual(score,.9)

    def test_invalid(self):
        p=[1,0,0,1];t=[1,0,.6,.8]
        cases=[(p,0,t,2,2),(p,33,t,2,2),(p,2,t,0,2),
               (p,2,t,2,65),(p,True,t,2,2),
               ([0,0,0,1],2,t,2,2),
               ([1,float('nan'),0,1],2,t,2,2),
               ([1001,0,0,1],2,t,2,2),
               (p,2,t[:-1],2,2),(p,2,[1,True,.6,.8],2,2)]
        for args in cases:
            with self.subTest(args=str(args)[:50]):
                with self.assertRaises(ValueError):
                    maxsim(*args)


if __name__ == "__main__":
    unittest.main()
