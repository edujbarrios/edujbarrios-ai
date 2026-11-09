#include "group_normalization.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void check(const double *x,size_t c,size_t s,size_t groups,
                  const double *gamma,const double *beta,double eps,
                  const double *want) {
    double out[2048]={0};
    assert(group_normalization(x,c,s,groups,gamma,beta,eps,out));
    for(size_t i=0;i<c*s;++i)assert(fabs(out[i]-want[i])<1e-10);
}
int main(void) {
    const double x[]={1,2,3,4,5,6,7,8};
    const double gain[]={1,1,1,1},bias[]={0,0,0,0};
    double expected[8];
    for(size_t i=0;i<8;++i)
        expected[i]=((double)(i%4)-1.5)/sqrt(1.5);
    check(x,4,2,2,gain,bias,0.25,expected);
    const double one_per_channel[]={10,20},g2[]={3,4},b2[]={5,6};
    check(one_per_channel,2,1,2,g2,b2,1e-5,b2);
    const double constant[]={7,7,7,7},constant_expect[]={5,5,6,6};
    check(constant,2,2,2,g2,b2,1e-5,constant_expect);
    const double singleton[]={9},single_gain[]={2},single_bias[]={3};
    check(singleton,1,1,1,single_gain,single_bias,1e-5,single_bias);
    const double affine_x[]={1,3,5,7},affine_gamma[]={2,0.5};
    const double affine_beta[]={1,-1};
    const double affine_want[]={1-6/sqrt(6.),1-2/sqrt(6.),
                                -1+0.5/sqrt(6.),-1+1.5/sqrt(6.)};
    check(affine_x,2,2,1,affine_gamma,affine_beta,1.0,affine_want);
    double big[2048]={0},big_gain[32],big_bias[32],out[2048];
    for(size_t i=0;i<32;++i){big_gain[i]=1;big_bias[i]=0;}
    for(size_t g=1;g<=32;g*=2) {
        assert(group_normalization(big,32,64,g,big_gain,big_bias,1e-5,out));
        for(size_t i=0;i<2048;++i)assert(out[i]==0);
    }
    out[0]=777;
    const double invalid[]={NAN},inf[]={INFINITY},high[]={1001};
    assert(!group_normalization(NULL,1,1,1,single_gain,single_bias,1e-5,out));
    assert(!group_normalization(singleton,0,1,1,single_gain,single_bias,1e-5,out));
    assert(!group_normalization(singleton,1,0,1,single_gain,single_bias,1e-5,out));
    assert(!group_normalization(singleton,33,1,1,single_gain,single_bias,1e-5,out));
    assert(!group_normalization(singleton,1,65,1,single_gain,single_bias,1e-5,out));
    assert(!group_normalization(singleton,1,1,0,single_gain,single_bias,1e-5,out));
    assert(!group_normalization(x,3,1,2,gain,bias,1e-5,out));
    assert(!group_normalization(singleton,1,1,1,single_gain,single_bias,0,out));
    assert(!group_normalization(singleton,1,1,1,single_gain,single_bias,2,out));
    assert(!group_normalization(singleton,1,1,1,single_gain,single_bias,NAN,out));
    assert(!group_normalization(invalid,1,1,1,single_gain,single_bias,1e-5,out));
    assert(!group_normalization(high,1,1,1,single_gain,single_bias,1e-5,out));
    assert(!group_normalization(singleton,1,1,1,inf,single_bias,1e-5,out));
    assert(!group_normalization(singleton,1,1,1,single_gain,invalid,1e-5,out));
    assert(!group_normalization(singleton,1,1,1,NULL,single_bias,1e-5,out));
    assert(!group_normalization(singleton,1,1,1,single_gain,NULL,1e-5,out));
    assert(!group_normalization(singleton,1,1,1,single_gain,single_bias,1e-5,NULL));
    double alias[]={9};
    assert(!group_normalization(alias,1,1,1,single_gain,single_bias,1e-5,alias));
    assert(out[0]==777);
    puts("C group normalization tests passed");
    return 0;
}
