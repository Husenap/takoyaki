#include "RenderTarget.h"

namespace ty {
RenderTarget::RenderTarget(const glm::ivec2& size)
    : mSize(size) {
	glGenFramebuffers(1, &mFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);

	glGenTextures(1, &mRenderTexture);
	glBindTexture(GL_TEXTURE_2D, mRenderTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderTexture, 0);
	GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FAILED to create framebuffer!" << std::endl;
	}
}
RenderTarget::~RenderTarget() {
	glDeleteTextures(1, &mRenderTexture);
	glDeleteFramebuffers(1, &mFramebuffer);
}
void RenderTarget::Resize(const glm::ivec2& size) {}

void RenderTarget::SetAsTarget() {
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
}

}  // namespace ty