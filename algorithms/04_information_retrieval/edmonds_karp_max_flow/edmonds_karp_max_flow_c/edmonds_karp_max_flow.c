#include "edmonds_karp_max_flow.h"

bool edmonds_karp_max_flow(
    const uint16_t *capacities, size_t n, size_t source, size_t sink,
    uint32_t *flow_out) {
    if (!capacities || !flow_out || n<2 || n>16 || source>=n || sink>=n ||
        source==sink) return false;
    uint32_t residual[256];
    for(size_t u=0;u<n;++u) {
        for(size_t v=0;v<n;++v) {
            const uint16_t value=capacities[u*n+v];
            if (value>1000 || (u==v && value!=0)) return false;
            residual[u*n+v]=value;
        }
    }
    uint32_t flow=0;
    for(;;) {
        int parent[16];
        for(size_t i=0;i<n;++i) parent[i]=-1;
        parent[source]=(int)source;
        size_t queue[16],head=0,tail=0;
        queue[tail++]=source;
        while(head<tail && parent[sink]<0) {
            const size_t u=queue[head++];
            for(size_t v=0;v<n;++v) {
                if(parent[v]<0 && residual[u*n+v]>0) {
                    parent[v]=(int)u;
                    queue[tail++]=v;
                }
            }
        }
        if(parent[sink]<0) {
            *flow_out=flow;
            return true;
        }
        uint32_t augment=1000;
        for(size_t v=sink;v!=source;v=(size_t)parent[v]) {
            const size_t u=(size_t)parent[v];
            const uint32_t value=residual[u*n+v];
            if(value<augment) augment=value;
        }
        for(size_t v=sink;v!=source;v=(size_t)parent[v]) {
            const size_t u=(size_t)parent[v];
            residual[u*n+v]-=augment;
            residual[v*n+u]+=augment;
        }
        flow+=augment;
    }
}
