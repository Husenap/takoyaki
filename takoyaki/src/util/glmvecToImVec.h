#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define IM_VEC2_CLASS_EXTRA       \
	ImVec2(const glm::vec2& v) {  \
		x = v.x;                  \
		y = v.y;                  \
	}                             \
	ImVec2(const glm::ivec2& v) { \
		x = v.x;                  \
		y = v.y;                  \
	}

#define IM_VEC4_CLASS_EXTRA       \
	ImVec4(const glm::vec4& v) {  \
		x = v.x;                  \
		y = v.y;                  \
		z = v.z;                  \
		w = v.w;                  \
	}                             \
	ImVec4(const glm::ivec4& v) { \
		x = v.x;                  \
		y = v.y;                  \
		z = v.z;                  \
		w = v.w;                  \
	}