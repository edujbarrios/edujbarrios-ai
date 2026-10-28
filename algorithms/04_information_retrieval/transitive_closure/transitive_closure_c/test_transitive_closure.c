#include "transitive_closure.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const uint8_t chain[]={0,1,0, 0,0,1, 0,0,0};
    const uint8_t expected[]={1,1,1, 0,1,1, 0,0,1};
    const uint8_t cycle[]={0,1,1,0};
    const uint8_t self[]={1};
    const uint8_t isolated[]={0,0,0,0};
    const uint8_t bad[]={0,2,0,0};
    uint8_t out[9]={99,99,99,99,99,99,99,99,99};
    assert(transitive_closure(chain,3,out));
    for(size_t i=0;i<9;++i) assert(out[i]==expected[i]);
    assert(transitive_closure(cycle,2,out));
    for(size_t i=0;i<4;++i) assert(out[i]==1);
    assert(transitive_closure(isolated,2,out));
    assert(out[0]==1 && out[1]==0 && out[2]==0 && out[3]==1);
    assert(transitive_closure(self,1,out) && out[0]==1);
    out[0]=99;
    assert(!transitive_closure(bad,2,out));
    assert(!transitive_closure(chain,0,out));
    assert(!transitive_closure(chain,65,out));
    assert(!transitive_closure(NULL,1,out));
    assert(!transitive_closure(chain,3,NULL));
    assert(out[0]==99);
    puts("C transitive closure tests passed");
    return 0;
}
