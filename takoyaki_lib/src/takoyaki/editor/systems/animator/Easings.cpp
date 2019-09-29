#include "Easings.h"

namespace ty::Easings {

float Lerp(float a, float b, float t) {
	return a + (b - a) * t;
}
float EaseLinear(float a, float b, float t) {
	return Lerp(a, b, t);
}
float EaseExponential(float a, float b, float t) {
	return Lerp(a, b, t * t);
}
float EaseSmoothstep(float a, float b, float t) {
	return Lerp(a, b, t * t * (3.f - 2.f * t));
}

float Ease(float a, float b, float t, Type type) {
	t = std::clamp(t, 0.f, 1.f);

	switch (type) {
	case Type::Zero:
		return a;
	case Type::Linear:
		return EaseLinear(a, b, t);
	case Type::Exponential:
		return EaseExponential(a, b, t);
	case Type::Smoothstep:
		return EaseSmoothstep(a, b, t);
	default:
		return a;
	}
}

}  // namespace ty::Easings
