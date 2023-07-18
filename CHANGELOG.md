# Changelog

All notable changes to this project will be documented in this file.

## Unreleased

### Changed

- Set default compilation mode to optimized rather than fast build

### Fixed

- Bazel: only run lint tests when ``--config lint`` is provided

## [1.2.1] - 2023/07/07

### Added

- Improve test coverage

### Fixed

- Bullet interface now reports the IMU orientation in the proper ARS frame

## [1.2.0] - 2023/06/06

### Added

- Bullet: floor parameter
- Bullet: gravity parameter
- Bullet: initial orientation parameter
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

- CPU temperature observer: add disabled state
- Distribute a PyPI package
- Spine state machine documentation
- Unit tests for ``ObserverError``
- Unit tests for ``Source``

### Changed

- Bazel: update PyPI dependencies to a requirements lock
- CI: merge into a single workflow with multiple jobs
- CPU temperature observer: read from custom file
- Moved Python `serialize` function to utils
- Moved logging to [mpacklog](https://github.com/tasts-robots/mpacklog/)
- Removed unused ``//models``
- Switch from aiorate to loop-rate-limiters
- Update mpacklog dependency

### Fixed

- Bazel: clean up unused models and redundant targets
- Bullet interface test
- Clean up empty observation and observers directories
- Fix calls to mpacklog logger functions

## [1.0.0] - 2022/06/10

First release of the project.
