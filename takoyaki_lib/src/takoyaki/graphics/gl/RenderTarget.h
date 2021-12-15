#pragma once

namespace ty {

class RenderTarget {
public:
	RenderTarget(const glm::ivec2& size);
	~RenderTarget();

	void Resize(const glm::ivec2& size);
	void SetAsTarget();

	const glm::ivec2& GetSize() const { return mSize; }
	GLuint GetFramebuffer() const { return mFramebuffer; }
	GLuint GetRenderTexture() const { return mRenderTexture; }

	std::vector<std::uint8_t> GetPixels(); 

private:
	void Init();
	void Destroy();

	GLuint mFramebuffer;
	GLuint mRenderTexture;
	glm::ivec2 mSize;
};

}  // namespace ty