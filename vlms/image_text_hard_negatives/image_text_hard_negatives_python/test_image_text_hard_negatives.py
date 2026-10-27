import unittest
from image_text_hard_negatives import image_text_hard_negatives as negatives


class HardNegativeTests(unittest.TestCase):
    def test_reference(self):
        identity=[1,0,0,1]
        same=[1,0,1,0,1,0]
        triple=[1,0,0,1,0.6,0.8]
        self.assertEqual(negatives(identity,identity,2,2),[1,0,1,0])
        self.assertEqual(negatives(same,same,3,2),[1,0,0,1,0,0])
        self.assertEqual(negatives(triple,triple,3,2),[2,2,1,2,2,1])
        self.assertEqual(negatives([2,0,0,3],identity,2,2),[1,0,1,0])

    def test_invalid(self):
        a=[1,0,0,1]
        cases=[(a,a,1,2),(a,a,65,2),(a,a,2,0),(a,a,2,129),
               (a,a,True,2),(a,a,2,True),(a,[1,0,0,0],2,2),
               (a,[1,0,float('nan'),1],2,2),
               (a,[1,0,float('inf'),1],2,2),
               (a,[1,0,1001,1],2,2),(a,[1,0,1],2,2),
               (a,[1,0,True,1],2,2)]
        for args in cases:
            with self.subTest(args=str(args)[:65]):
                with self.assertRaises(ValueError):
                    negatives(*args)


if __name__ == '__main__':
    unittest.main()
