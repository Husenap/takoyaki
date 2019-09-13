#pragma once

namespace ty{

using NewFileHandler = std::function<void()>;
using OpenFileHandler = std::function<void()>;
using SaveFileHandler = std::function<void()>;

}