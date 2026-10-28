#include "cosine_annealing_learning_rate.h"
#include <math.h>

bool cosine_annealing_learning_rate(double max_rate, double min_rate,
                                    size_t step, size_t total_steps, double *out) {
    if (!out || !isfinite(max_rate) || !isfinite(min_rate) ||
        min_rate<0.0 || max_rate<min_rate || max_rate>1000000.0 ||
        total_steps==0 || total_steps>100000 || step>total_steps)
        return false;
    const double pi=acos(-1.0);
    const double ratio=(double)step/(double)total_steps;
    const double fraction=(1.0+cos(pi*ratio))*0.5;
    double result=min_rate+(max_rate-min_rate)*fraction;
    if (!isfinite(result)) return false;
    *out=fmax(min_rate,fmin(max_rate,result));
    return true;
}
