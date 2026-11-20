#include "minhash_signature.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    const uint32_t tokens[] = {1, 2, 3}, reordered[] = {3, 1, 2, 1};
    const uint32_t a[] = {1, 3}, b[] = {0, 7};
    const uint32_t maximum[] = {65520}, extreme[] = {65520};
    const uint32_t bad_tokens[] = {65521}, bad_a[] = {0}, bad_b[] = {65521};
    uint32_t out[] = {999, 999};
    assert(minhash_signature(tokens, 3, a, b, 2, out));
    assert(out[0] == 1 && out[1] == 10);
    assert(minhash_signature(reordered, 4, a, b, 2, out));
    assert(out[0] == 1 && out[1] == 10);
    assert(minhash_signature(tokens, 0, a, b, 2, out));
    assert(out[0] == 65521 && out[1] == 65521);
    assert(minhash_signature(maximum, 1, extreme, extreme, 1, out));
    assert(out[0] == 0);
    out[0] = 999;
    assert(!minhash_signature(bad_tokens, 1, a, b, 2, out));
    assert(!minhash_signature(tokens, 3, bad_a, b, 1, out));
    assert(!minhash_signature(tokens, 3, a, bad_b, 1, out));
    assert(!minhash_signature(tokens, 3, a, b, 0, out));
    assert(!minhash_signature(tokens, 3, a, b, 2, NULL));
    assert(out[0] == 999);
    puts("C MinHash signature tests passed");
    return 0;
}
