import unittest
from k_means import k_means


class KMeansTests(unittest.TestCase):
    def test_two_clusters(self):
        centers, labels = k_means([[0,0],[10,10],[0,2],[10,12]],2,10)
        self.assertEqual(centers, [[0.0,1.0],[10.0,11.0]])
        self.assertEqual(labels,[0,1,0,1])

    def test_single_cluster(self):
        centers, labels = k_means([[0,0],[10,10]],1,10)
        self.assertEqual(centers, [[5.0,5.0]])
        self.assertEqual(labels,[0,0])

    def test_invalid(self):
        for points,k,iterations in (([],1,10),
                                    ([[1],[2]],0,10),
                                    ([[1],[2]],3,10),
                                    ([[1],[2]],1,0),
                                    ([[0],[0],[2]],2,10),
                                    ([[0,0],[1]],1,10),
                                    ([[float("nan")],[1]],1,10),
                                    ([[1e308],[1e308]],1,10),
                                    ([[True],[1]],1,10)):
            with self.subTest(points=points,k=k,iterations=iterations):
                with self.assertRaises(ValueError):
                    k_means(points,k,iterations)


if __name__ == "__main__":
    unittest.main()
