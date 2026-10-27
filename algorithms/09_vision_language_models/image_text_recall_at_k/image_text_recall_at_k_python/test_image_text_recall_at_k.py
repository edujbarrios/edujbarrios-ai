import unittest
from image_text_recall_at_k import image_text_recall_at_k as recall


class ImageTextRecallTests(unittest.TestCase):
    def test_reference(self):
        aligned = [1,0,0,1]
        swapped = [0,1,1,0]
        duplicates = [1,0,1,0]
        self.assertEqual(recall(aligned,aligned,2,2,1),(1,1))
        self.assertEqual(recall(aligned,swapped,2,2,1),(0,0))
        self.assertEqual(recall(aligned,swapped,2,2,2),(1,1))
        self.assertEqual(recall(duplicates,duplicates,2,2,1),(.5,.5))
        self.assertEqual(recall([2,0,0,3],aligned,2,2,1),(1,1))

    def test_invalid(self):
        a=[1,0,0,1]
        cases=[(a,a,1,2,1),(a,a,65,2,1),(a,a,2,0,1),
               (a,a,2,129,1),(a,a,2,2,0),(a,a,2,2,3),
               (a,a,True,2,1),(a,a,2,2,True),
               (a,[0,0,0,1],2,2,1),(a,[1,0,float('nan'),1],2,2,1),
               (a,[1,0,float('inf'),1],2,2,1),
               (a,[1,0,1001,1],2,2,1),
               (a,[1,0,1],2,2,1),(a,[1,0,True,1],2,2,1)]
        for args in cases:
            with self.subTest(args=str(args)[:55]):
                with self.assertRaises(ValueError):
                    recall(*args)


if __name__ == "__main__":
    unittest.main()
