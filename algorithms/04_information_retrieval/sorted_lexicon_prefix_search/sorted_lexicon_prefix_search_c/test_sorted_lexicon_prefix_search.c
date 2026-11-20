#include "sorted_lexicon_prefix_search.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    const char *words[] = {"ant", "antenna", "ape", "bat", "batch", "cat"};
    const char *empty[] = {NULL};
    const char *unsorted[] = {"bat", "ant"};
    const char *duplicate[] = {"ant", "ant"};
    const char *bad[] = {"Ant"};
    size_t indices[6] = {999}, count = 777;
    assert(sorted_lexicon_prefix_search(words, 6, "an", indices, &count));
    assert(count == 2 && indices[0] == 0 && indices[1] == 1);
    assert(sorted_lexicon_prefix_search(words, 6, "ba", indices, &count));
    assert(count == 2 && indices[0] == 3 && indices[1] == 4);
    assert(sorted_lexicon_prefix_search(words, 6, "zzz", indices, &count));
    assert(count == 0);
    assert(sorted_lexicon_prefix_search(words, 6, "", indices, &count));
    assert(count == 6 && indices[5] == 5);
    assert(sorted_lexicon_prefix_search(empty, 0, "", indices, &count));
    assert(count == 0);
    count = 777;
    indices[0] = 999;
    assert(!sorted_lexicon_prefix_search(unsorted, 2, "a", indices, &count));
    assert(!sorted_lexicon_prefix_search(duplicate, 2, "a", indices, &count));
    assert(!sorted_lexicon_prefix_search(bad, 1, "a", indices, &count));
    assert(!sorted_lexicon_prefix_search(words, 6, "A", indices, &count));
    assert(!sorted_lexicon_prefix_search(words, 6, NULL, indices, &count));
    assert(!sorted_lexicon_prefix_search(words, 6, "a", NULL, &count));
    assert(count == 777 && indices[0] == 999);
    puts("C sorted lexicon prefix search tests passed");
    return 0;
}
