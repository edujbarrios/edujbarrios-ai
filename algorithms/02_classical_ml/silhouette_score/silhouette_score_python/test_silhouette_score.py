import math
import unittest
from silhouette_score import silhouette_score

class SilhouetteTests(unittest.TestCase):
    def test_known_partition(self):
        pts=[[0],[1],[5],[6]]
        self.assertAlmostEqual(silhouette_score(pts,[0,0,1,1]),79/99)
        self.assertAlmostEqual(silhouette_score(pts,[1,1,3,3]),79/99)
        self.assertEqual(silhouette_score([[0],[0],[0],[0]],[0,0,1,1]),0.0)
        self.assertTrue(math.isfinite(silhouette_score(pts,[0,0,1,2])))
    def test_invalid(self):
        pts=[[0],[1],[5],[6]]
        cases=((pts,[0,1,2,3]),(pts,[0]*4),(pts,[0,0,1,4]),
               (pts,[0,0,1]),(pts,[0,0,1,True]),
               ([[0],[1],[math.nan],[6]],[0,0,1,1]),
               ([[-1.7e308],[1.7e308],[0],[1]],[0,0,1,1]),
               ([[0],[1,2],[5],[6]],[0,0,1,1]))
        for rows,labels in cases:
            with self.subTest(rows=rows,labels=labels),self.assertRaises(ValueError):
                silhouette_score(rows,labels)

if __name__=="__main__":unittest.main()
