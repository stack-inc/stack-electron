// Copyright (c) 2013 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/common/electron_command_line.h"

#include "base/command_line.h"
#include "uv.h"  // NOLINT(build/include_directory)

namespace electron {

// static
base::CommandLine::StringVector ElectronCommandLine::argv_;

// static
void ElectronCommandLine::Init(int argc, base::CommandLine::CharType** argv) {
  DCHECK(argv_.empty());

  // NOTE: uv_setup_args does nothing on Windows, so we don't need to call it.
  // Otherwise we'd have to convert the arguments from UTF16.
#if !defined(OS_WIN)
  // Hack around with the argv pointer. Used for process.title = "blah"
#if defined(OS_MAC)
  argv = uv_setup_args(argc, argv);
#else
  argv = node::SetupArgs(argc, argv);
#endif
#endif

  argv_.assign(argv, argv + argc);
}

#if defined(OS_LINUX)
// static
void ElectronCommandLine::InitializeFromCommandLine() {
  argv_ = base::CommandLine::ForCurrentProcess()->argv();
}
#endif

}  // namespace electron
