#include "integral_image.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    uint32_t result[4096] = {0};
    const uint8_t square[] = {1, 2, 3, 4};
    assert(integral_image(square, 2, 2, result));
    assert(result[0] == 1 && result[1] == 3 &&
           result[2] == 4 && result[3] == 10);
    const uint8_t line[] = {1, 2, 3};
    assert(integral_image(line, 1, 3, result));
    assert(result[0] == 1 && result[1] == 3 && result[2] == 6);
    assert(integral_image(line, 3, 1, result));
    assert(result[0] == 1 && result[1] == 3 && result[2] == 6);
    const uint8_t zero[] = {0, 0, 0, 0};
    assert(integral_image(zero, 2, 2, result));
    assert(result[3] == 0);
    uint8_t full[4096];
    memset(full, 255, sizeof(full));
    assert(integral_image(full, 64, 64, result));
    assert(result[0] == 255 && result[63] == 64u * 255u);
    assert(result[4095] == 64u * 64u * 255u);
    result[0] = 777;
    assert(!integral_image(full, 0, 64, result));
    assert(!integral_image(full, 65, 1, result));
    assert(!integral_image(full, 1, 0, result));
    assert(!integral_image(full, 1, 65, result));
    assert(!integral_image(NULL, 1, 1, result));
    assert(!integral_image(full, 1, 1, NULL));
    assert(result[0] == 777);
    puts("C integral image tests passed");
    return 0;
}
