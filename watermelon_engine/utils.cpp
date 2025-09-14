#include "utils.hpp"

float normalizeAxis(float axis, float maxValue)
{
    return axis / (axis < 0 ? maxValue + 1 : maxValue);
}