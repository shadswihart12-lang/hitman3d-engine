# O3DE Development Agent Instructions

## Cursor Cloud specific instructions

### Overview

O3DE (Open 3D Engine) is a large C++ game engine. It uses CMake + Ninja for building, and ships a bundled Python 3.10 for build scripts, the `o3de` CLI, and test tooling. There is no traditional web service — this is a desktop engine with a build-and-test workflow.

### Key commands

| Task | Command |
|---|---|
| Bootstrap Python | `bash python/get_python.sh` |
| Register engine | `bash scripts/o3de.sh register --this-engine` |
| CMake configure | `cmake --preset linux-default -DLY_3RDPARTY_PATH=$HOME/.o3de/3rdParty` |
| Build a target | `cmake --build build/linux --target <TARGET> --config profile -j $(nproc)` |
| Run CTest smoke | `cd build/linux && ctest -C profile -L SUITE_smoke --output-on-failure` |
| Run Python tests | `python/python.sh -m pytest scripts/o3de/tests/ --timeout=60` |
| Run C++ tests | `build/linux/bin/profile/AzTestRunner build/linux/bin/profile/lib<Test>.so AzRunUnitTests` |
| O3DE CLI | `python/python.sh scripts/o3de.py <subcommand>` |

### Non-obvious caveats

- **Bundled Python**: Always use `python/python.sh` (not system `python3`) so that the correct Python 3.10 venv with O3DE's pinned dependencies is used.
- **libstdc++-14-dev**: On Ubuntu 24.04 (Noble), clang 18 looks for libstdc++ in the gcc-14 directory but only gcc-13 dev libs ship by default. You must install `libstdc++-14-dev` or linking will fail.
- **3rd-party packages**: CMake configure auto-downloads ~40 packages to `$HOME/.o3de/3rdParty`. First configure takes several minutes; subsequent runs are fast if the cache persists.
- **No GPU in Cloud VMs**: CTest excludes `REQUIRES_gpu` tests by default in the test preset. The Editor and AssetProcessor also need a display server (Xvfb) if you need to run them headlessly.
- **Build times**: A full engine build takes hours on 4 cores. For fast iteration, build only the specific target you need (e.g., `AzCore.Tests`).
- **Pre-existing test failures**: `scripts/o3de/tests/test_repo_properties.py` has 27 failures due to Windows-style paths (`F:/repotest`) being tested on Linux. These are pre-existing and not caused by environment issues.
- **ROS2 packages**: The noble package list includes `ros-jazzy-*` packages, but these are optional and require the ROS2 apt repository. They are not needed for core engine development.
