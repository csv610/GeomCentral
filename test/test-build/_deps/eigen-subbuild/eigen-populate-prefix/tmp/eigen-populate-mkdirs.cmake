# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/csv610/Projects/Mesh/geometry-central/test-build/lib/deps/eigen-src")
  file(MAKE_DIRECTORY "/Users/csv610/Projects/Mesh/geometry-central/test-build/lib/deps/eigen-src")
endif()
file(MAKE_DIRECTORY
  "/Users/csv610/Projects/Mesh/geometry-central/test-build/lib/deps/eigen-build"
  "/Users/csv610/Projects/Mesh/geometry-central/test-build/_deps/eigen-subbuild/eigen-populate-prefix"
  "/Users/csv610/Projects/Mesh/geometry-central/test-build/_deps/eigen-subbuild/eigen-populate-prefix/tmp"
  "/Users/csv610/Projects/Mesh/geometry-central/test-build/_deps/eigen-subbuild/eigen-populate-prefix/src/eigen-populate-stamp"
  "/Users/csv610/Projects/Mesh/geometry-central/test-build/_deps/eigen-subbuild/eigen-populate-prefix/src"
  "/Users/csv610/Projects/Mesh/geometry-central/test-build/_deps/eigen-subbuild/eigen-populate-prefix/src/eigen-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/csv610/Projects/Mesh/geometry-central/test-build/_deps/eigen-subbuild/eigen-populate-prefix/src/eigen-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/csv610/Projects/Mesh/geometry-central/test-build/_deps/eigen-subbuild/eigen-populate-prefix/src/eigen-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
