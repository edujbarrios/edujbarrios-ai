import unittest
from image_text_mrr import image_text_mrr as mrr


class MRRTests(unittest.TestCase):
    def test_reference(self):
        identity=[1,0,0,1]
        self.assertEqual(mrr(identity,identity,2,2),(1,1))
        self.assertEqual(mrr(identity,[0,1,1,0],2,2),(.5,.5))
        self.assertEqual(mrr([1,0,1,0],[1,0,1,0],2,2),(.75,.75))
        self.assertEqual(mrr([2,0,0,3],identity,2,2),(1,1))

    def test_invalid(self):
        a=[1,0,0,1]
        cases=[(a,a,1,2),(a,a,65,2),(a,a,2,0),(a,a,2,129),
               (a,a,True,2),(a,a,2,True),(a,[0,0,0,1],2,2),
               (a,[1,float('nan'),0,1],2,2),
               (a,[1,float('inf'),0,1],2,2),
               (a,[1,1001,0,1],2,2),(a,[1,0,1],2,2),
               (a,[True,0,0,1],2,2)]
        for args in cases:
            with self.subTest(args=str(args)[:50]):
                with self.assertRaises(ValueError):
                    mrr(*args)


if __name__ == "__main__":
    unittest.main()
