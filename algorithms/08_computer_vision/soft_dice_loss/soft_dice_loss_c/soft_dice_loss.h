#ifndef EDUJBARRIOS_AI_SOFT_DICE_LOSS_H
#define EDUJBARRIOS_AI_SOFT_DICE_LOSS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Binary probabilistic Dice loss with fixed epsilon=1, n 1..512.
   Input is unchanged and output is untouched on validation failure. */
bool soft_dice_loss(const double *predictions, const uint8_t *targets,
                    size_t n, double *output);
#endif
