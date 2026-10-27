import math
import unittest
from temperature_softmax import temperature_softmax as softmax


class TemperatureSoftmaxTests(unittest.TestCase):
    def test_reference(self):
        expected = math.e / (1 + math.e)
        self.assertAlmostEqual(softmax([0,2],2)[1],expected)
        self.assertAlmostEqual(softmax([998,1000],2)[1],expected)
        for value in softmax([3,3,3],100):
            self.assertAlmostEqual(value,1/3)
        self.assertEqual(softmax([-1000,1000],0.01),[0,1])
        self.assertEqual(softmax([5],1),[1])
        self.assertAlmostEqual(sum(softmax([0,1,2],1)),1)

    def test_invalid(self):
        cases=[([],1),([1]*4097,1),([1],0),([1],0.001),
               ([1],101),([1],float('nan')),([1],True),
               ([float('inf')],1),([float('nan')],1),
               ([1001],1),([True],1),("123",1)]
        for args in cases:
            with self.subTest(args=str(args)[:50]):
                with self.assertRaises(ValueError):
                    softmax(*args)


if __name__ == "__main__":
    unittest.main()
