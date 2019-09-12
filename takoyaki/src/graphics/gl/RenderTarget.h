#pragma once

namespace ty {

class RenderTarget {
public:
	RenderTarget(const glm::ivec2& size);
	~RenderTarget();

	void Resize(const glm::ivec2& size);
	void SetAsTarget();

	const glm::ivec2& GetSize() const { return mSize; }
	GLuint GetFramebuffer() { return mFramebuffer; }
	GLuint GetRenderTexture() { return mRenderTexture; }

private:
	GLuint mFramebuffer;
	GLuint mRenderTexture;
	glm::ivec2 mSize;
};

}  // namespace ty