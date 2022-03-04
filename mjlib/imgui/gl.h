// Copyright 2020 Josh Pieper, jjp@pobox.com.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <GL/gl3w.h>

#include <cstdlib>

#include <fmt/format.h>

namespace mjlib {
namespace imgui {

inline void TraceGlError(const char* file, int line) {
  const auto v = glGetError();
  if (v == GL_NO_ERROR) { return; }

  fmt::print(stderr, "{}:{} glError()={}\n", file, line,v);
  std::abort();
}

#ifdef MJLIB_ENABLE_MJ_TRACE_GL_ERROR
#define MJ_TRACE_GL_ERROR() ::mjlib::imgui::TraceGlError(__FILE__, __LINE__)
#else
#define MJ_TRACE_GL_ERROR()
#endif

}
}
