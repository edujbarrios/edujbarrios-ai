import unittest
from sobel_edges import sobel_edges as sobel


class SobelTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(sobel([0,1,2,0,1,2,0,1,2],3,3),
                         [4,8,4,4,8,4,4,8,4])
        self.assertEqual(sobel([0,0,0,1,1,1,2,2,2],3,3),
                         [4,4,4,8,8,8,4,4,4])
        self.assertEqual(sobel([5,5,5,5],2,2),[0]*4)
        self.assertEqual(sobel([7],1,1),[0])

    def test_invalid(self):
        ramp=[0,1,2,0,1,2,0,1,2]
        cases=[(ramp,0,3),(ramp,3,0),(ramp,65,3),
               (ramp,3,65),(ramp,True,3),(ramp,3,3.0),
               (ramp[:-1],3,3),(ramp[:1]+[float('nan')]+ramp[2:],3,3),
               (ramp[:1]+[float('inf')]+ramp[2:],3,3),
               (ramp[:1]+[1001]+ramp[2:],3,3),
               (ramp[:1]+[True]+ramp[2:],3,3)]
        for args in cases:
            with self.subTest(args=str(args)[:55]):
                with self.assertRaises(ValueError):
                    sobel(*args)


if __name__ == "__main__":
    unittest.main()
