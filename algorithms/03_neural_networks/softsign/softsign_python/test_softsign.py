import unittest
from softsign import softsign


class SoftsignTests(unittest.TestCase):
    def test_reference(self):
        x=[-2,-1,0,1,2]
        expected=[-2/3,-0.5,0,0.5,2/3]
        for a,b in zip(softsign(x),expected):
            self.assertAlmostEqual(a,b)
        y=softsign([1000000,-1000000])
        self.assertAlmostEqual(y[0],1000000/1000001)
        self.assertAlmostEqual(y[1],-1000000/1000001)
        self.assertEqual(softsign([0]),[0])
        self.assertTrue(all(-1<z<1 for z in y))

    def test_invalid(self):
        cases=[[],[True],["1"],[1j],[float("nan")],
               [float("inf")],[1000001],[-1000001],[0]*10001]
        for values in cases:
            with self.subTest(length=len(values)):
                with self.assertRaises(ValueError): softsign(values)


if __name__=="__main__":
    unittest.main()
