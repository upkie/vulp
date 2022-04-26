# -*- python -*-

load("//tools/workspace:github_archive.bzl", "github_archive")

def styleguide_repository(name="styleguide"):
    github_archive(
        name = name,
        repository = "google/styleguide",
        commit = "748ed4dd4e543001421c6618646a60e2a2dca8ea",
        sha256 = "b5981b4a03bb9b1c354b072c2d6b36b88232cc7cbbd2cb49d5b30fb32be9694f",
        build_file = "//tools/workspace/styleguide:package.BUILD",
    )
