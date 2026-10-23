#include "pca_2d.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool pca_2d(const double *points, size_t n, double *mean,
            double *direction, double *projection) {
    if (!points || !mean || !direction || !projection ||
        n < 2 || n > SIZE_MAX/2 || n > SIZE_MAX/sizeof(double)) return false;
    double mu0=0.0,mu1=0.0;
    for (size_t i=0;i<n;++i) {
        const double x=points[2*i], y=points[2*i+1];
        if (!isfinite(x) || !isfinite(y)) return false;
        const double dx=x-mu0,dy=y-mu1;
        if (!isfinite(dx)||!isfinite(dy)) return false;
        mu0+=dx/(double)(i+1);
        mu1+=dy/(double)(i+1);
        if (!isfinite(mu0)||!isfinite(mu1)) return false;
    }
    double a=0.0,b=0.0,c=0.0;
    for (size_t i=0;i<n;++i) {
        const double x=points[2*i]-mu0,y=points[2*i+1]-mu1;
        const double xx=x*x,xy=x*y,yy=y*y;
        if (!isfinite(xx)||!isfinite(xy)||!isfinite(yy)) return false;
        a+=xx; b+=xy; c+=yy;
        if (!isfinite(a)||!isfinite(b)||!isfinite(c)) return false;
    }
    a/=(double)n;b/=(double)n;c/=(double)n;
    if (!isfinite(a)||!isfinite(b)||!isfinite(c) ||
        (a==0.0&&c==0.0)) return false;
    double vx,vy;
    if (b==0.0) {
        vx=a>=c?1.0:0.0;
        vy=a>=c?0.0:1.0;
    } else {
        const double delta=a-c;
        const double twice_b=2.0*b;
        const double disc=hypot(delta,twice_b);
        vx=delta+disc;vy=twice_b;
        const double length=hypot(vx,vy);
        if (!isfinite(delta)||!isfinite(twice_b)||!isfinite(disc)||
            !isfinite(vx)||!isfinite(length)||length<=0.0) return false;
        vx/=length;vy/=length;
        if (vx<0.0||(vx==0.0&&vy<0.0)) {vx=-vx;vy=-vy;}
    }
    double *scratch=malloc(n*sizeof(*scratch));
    if (!scratch) return false;
    for (size_t i=0;i<n;++i) {
        const double dx=points[2*i]-mu0,dy=points[2*i+1]-mu1;
        const double px=dx*vx,py=dy*vy;
        const double result=px+py;
        if (!isfinite(dx)||!isfinite(dy)||!isfinite(px)||
            !isfinite(py)||!isfinite(result)) {free(scratch);return false;}
        scratch[i]=result;
    }
    mean[0]=mu0;mean[1]=mu1;
    direction[0]=vx;direction[1]=vy;
    memcpy(projection,scratch,n*sizeof(*scratch));
    free(scratch);
    return true;
}
