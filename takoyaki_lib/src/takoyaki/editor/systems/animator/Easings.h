#pragma once

namespace ty::Easings {

enum Type : unsigned char { Zero, Linear, Exponential, Smoothstep, Count };

Type NextType(Type type);

float Ease(float a, float b, float t, Type type);

}  // namespace ty::Easings