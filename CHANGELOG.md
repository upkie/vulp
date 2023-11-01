# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added

- Log configuration dictionary every time the spine is reset

## [1.5.0] - 2023-10-16

### Added

- BulletInterface: Extend initial state to include the floating base velocity
- SpineInterface: Mention name of the spine we are waiting for at startup

### Changed

- **Breaking:** Add "reset" sub-dictionary to Bullet interface config
- Action dictionaries read by the spine are now fully logged
- BulletInterface: Refactor internal reset functions
- Update mpacklog.cpp to v3.1.0
- Update palimpsest to v2.0.0

### Fixed

- Clear configuration dictionary when resetting the spine

## [1.4.0] - 2023/08/22

### Added

- SpineInterface: ``retries`` parameter to wait for the spine to start
- observers: Keyboard source by @boragokbakan

### Changed

- Bazel: Compile coverage and test targets in debug by default
- Refactor project directories for simpler Bazel and PyPI distributions

## [1.3.0] - 2023/07/24

### Added

- Portable and SSH-compatible Keyboard source
- Support macOS operating systems (with help from @boragokbakan)
- Virtual destructor to the main ``Interface`` class

### Changed

- Set default compilation mode to optimized rather than fast build

### Fixed

- Bazel: Only run lint tests when ``--config lint`` is provided

## [1.2.1] - 2023/07/07

### Added

- Improve test coverage

### Fixed

- Bullet interface now reports the IMU orientation in the proper ARS frame

## [1.2.0] - 2023/06/06

### Added

- Bullet: Floor parameter
- Bullet: Gravity parameter
- Bullet: Initial orientation parameter
- Optional constructor parameter to require the joystick to be present
- Unit tests for the internal agent interface

### Changed

- Avoid the ``posix_ipc.O_RDWR`` flag in the Python spine interface
- Remove ``mypy_integration``
- Update mpacklog to v2.1.0

### Fixed

- Find ``clang-format`` path on macOS (thanks to @boragokbakan)
- Only apply the ``-lrt`` linker flag on Linux
- Shared-memory file permissions

## [1.1.1] - 2023/03/13

### Changed

- Remove setuptools from Python dependencies

## [1.1.0] - 2023/03/06

### Added

- CPU temperature observer: disabled state
- Distribute a PyPI package
- Spine state machine documentation
- Unit tests for ``ObserverError``
- Unit tests for ``Source``

### Changed

- Bazel: Update PyPI dependencies to a requirements lock
- CI: Merge into a single workflow with multiple jobs
- CPU temperature observer: Read from custom file
- Moved Python `serialize` function to utils
- Moved logging to [mpacklog](https://github.com/tasts-robots/mpacklog/)
- Removed unused ``//models``
- Switch from aiorate to loop-rate-limiters
- Update mpacklog dependency

### Fixed

- Bazel: Clean up unused models and redundant targets
- Bullet interface test
- Clean up empty observation and observers directories
- Fix calls to mpacklog logger functions

## [1.0.0] - 2022/06/10

First release of the project.
