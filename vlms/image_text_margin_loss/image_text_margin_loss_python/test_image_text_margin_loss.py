import unittest
from image_text_margin_loss import image_text_margin_loss as loss


class ImageTextMarginTests(unittest.TestCase):
    def test_reference(self):
        aligned=[1,0,0,1]
        swapped=[0,1,1,0]
        duplicate=[1,0,1,0]
        self.assertAlmostEqual(loss(aligned,aligned,2,2,.25),0)
        self.assertAlmostEqual(loss(aligned,swapped,2,2,.25),1.25)
        self.assertAlmostEqual(loss(duplicate,duplicate,2,2,.25),.25)
        self.assertAlmostEqual(loss([2,0,0,3],aligned,2,2,.25),0)
        self.assertAlmostEqual(loss(aligned,aligned,2,2,2),1)

    def test_invalid(self):
        a=[1,0,0,1]
        cases=[(a,a,1,2,.25),(a,a,2,0,.25),(a,a,2,129,.25),
               (a,a,2,2,-1),(a,a,2,2,2.1),
               (a,a,2,2,float('nan')),(a,a,2,2,True),
               (a,a,True,2,.25),(a,[0,0,0,1],2,2,.25),
               (a,[1,0,float('nan'),1],2,2,.25),
               (a,[1,0,1001,1],2,2,.25),
               (a,[1,0,1],2,2,.25),
               (a,[1,0,True,1],2,2,.25)]
        for args in cases:
            with self.subTest(args=str(args)[:55]):
                with self.assertRaises(ValueError):
                    loss(*args)


if __name__ == "__main__":
    unittest.main()
