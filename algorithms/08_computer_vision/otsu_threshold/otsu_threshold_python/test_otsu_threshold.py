import unittest
from otsu_threshold import otsu_threshold as otsu


class OtsuTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(otsu([10,20,200,210]),20)
        self.assertEqual(otsu([0,0,255,255]),0)
        self.assertEqual(otsu([1,2]),1)
        self.assertEqual(otsu([7,7,7]),0)
        self.assertEqual(otsu([255]),0)
        self.assertEqual(otsu([0]*10+[255]*10),0)

    def test_invalid(self):
        cases=[[],[0]*4097,[-1],[256],[1.5],[True],
               [float('inf')],[float('nan')],['1'],'123']
        for pixels in cases:
            with self.subTest(case=str(pixels)[:45]):
                with self.assertRaises(ValueError):
                    otsu(pixels)


if __name__ == '__main__':
    unittest.main()
