#pragma once

namespace ty{

using OpenFileHandler = std::function<void()>;
using SaveFileHandler = std::function<void()>;
using SaveFileAsHandler = std::function<void()>;

}