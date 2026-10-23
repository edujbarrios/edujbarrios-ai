#include "binary_cross_entropy.h"
#include <math.h>
bool binary_cross_entropy(const double *y,const double *p,size_t n,double *out) {
 if (!y||!p||!out||!n) return false;
 double total=0;
 for(size_t i=0;i<n;i++){
  if(!isfinite(y[i])||!isfinite(p[i])||(y[i]!=0.0&&y[i]!=1.0)||p[i]<0||p[i]>1) return false;
  double term=y[i]==1.0 ? -log(p[i]) : -log1p(-p[i]);
  if(!isfinite(term)) return false;
  total+=term;
  if(!isfinite(total)) return false;
 }
 double loss=total/(double)n;
 if(!isfinite(loss)) return false;
 *out=loss;return true;
}
