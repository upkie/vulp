# Changelog

All notable changes to this project will be documented in this file.

## Unreleased

### Added

- Bullet: floor parameter
- Bullet: gravity parameter

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
- Moved logging to [mpacklog](https://github.com/stephane-caron/mpacklog/)
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
