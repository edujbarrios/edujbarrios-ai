import math
import unittest
from quantile_linear_interpolation import quantile_linear_interpolation as q


class QuantileTests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(q([1,2,3,4],0.25),1.75)
        self.assertAlmostEqual(q([1,2,3,4],0.5),2.5)
        self.assertAlmostEqual(q([1,2,3,4],0.75),3.25)
        self.assertAlmostEqual(q([5,10],0.2),6.0)
        self.assertAlmostEqual(q([42],0.9),42.0)
        self.assertEqual(q([100,2,-5],0),-5)
        self.assertEqual(q([100,2,-5],1),100)
        self.assertAlmostEqual(q((4,1,3,2),0.25),1.75)
        self.assertEqual(q([7]*512,0.37),7)
        self.assertEqual(q([0,1,2,3,4],0.5),2)
        self.assertTrue(math.isclose(q([-1e308,1e308],0.5),0.0,abs_tol=1e292))
        self.assertEqual(q([-1e308,1e308],0.0),-1e308)
        self.assertEqual(q([-1e308,1e308],1.0),1e308)

    def test_invalid(self):
        for values, p in [
            ([],0.5),([1]*513,0.5),([math.inf],0.5),([math.nan],0.5),
            ([True],0.5),([1,"2"],0.5),(None,0.5),("123",0.5),
            ([10**400],0.5),([1],True),([1],None),([1],-0.01),
            ([1],1.01),([1],math.inf),([1],math.nan),([1],10**400),
        ]:
            with self.subTest(value=str(values)[:24],probability=p):
                with self.assertRaises(ValueError):
                    q(values,p)


if __name__ == "__main__":
    unittest.main()
