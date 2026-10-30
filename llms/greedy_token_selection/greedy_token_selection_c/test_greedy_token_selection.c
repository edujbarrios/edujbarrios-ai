#include "greedy_token_selection.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double tied[] = {2, 5, 5, -1};
    const double negative[] = {-4, -2, -3};
    const double singleton[] = {-1e308};
    const double extreme[] = {-1e308, 1e308};
    const double bad[] = {1, NAN};
    const double infinity[] = {INFINITY};
    size_t selected = 99;
    assert(greedy_token_selection(tied, 4, &selected) && selected == 1);
    assert(greedy_token_selection(negative, 3, &selected) && selected == 1);
    assert(greedy_token_selection(singleton, 1, &selected) && selected == 0);
    assert(greedy_token_selection(extreme, 2, &selected) && selected == 1);
    selected = 99;
    assert(!greedy_token_selection(bad, 2, &selected) && selected == 99);
    assert(!greedy_token_selection(infinity, 1, &selected) && selected == 99);
    assert(!greedy_token_selection(tied, 0, &selected));
    assert(!greedy_token_selection(NULL, 1, &selected));
    assert(!greedy_token_selection(tied, 4, NULL));
    puts("C greedy token selection tests passed");
    return 0;
}
