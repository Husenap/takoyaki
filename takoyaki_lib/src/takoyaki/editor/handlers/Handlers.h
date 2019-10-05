#pragma once

namespace ty {

// Editor handlers
using UniformsChangedHandler = std::function<void()>;

// Input handlers
using CameraCaptureInputHandler = std::function<void()>;
using CameraReleaseInputHandler = std::function<void()>;

}  // namespace ty