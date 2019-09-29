#pragma once

namespace ty::Easings {

enum class Type { Zero, Linear, Exponential, Smoothstep };

float Ease(float a, float b, float t, Type type);

}  // namespace ty::Easing