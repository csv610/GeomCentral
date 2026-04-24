cmake_minimum_required(VERSION 3.8.0)

project(geometry-central-apps)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EIGEN3_INCLUDE_DIR /Users/csv610/Projects/Mesh/geometry-central/build/deps/eigen-src CACHE PATH "Eigen3 include path")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

find_package(Eigen3 3.3 REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  ${EIGEN3_INCLUDE_DIR}
  /opt/homebrew/include/suitesparse
)

set(GC_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../build/src/libgeometry-central.a)

set(APP_LIBS 
  ${GC_LIB}
  /opt/homebrew/lib/libcholmod.dylib
  /opt/homebrew/lib/libumfpack.dylib
  /opt/homebrew/lib/libspqr.dylib
  /opt/homebrew/lib/libamd.dylib
  /opt/homebrew/lib/libcamd.dylib
  /opt/homebrew/lib/libcolamd.dylib
  /opt/homebrew/lib/libccolamd.dylib
  /opt/homebrew/lib/libsuitesparseconfig.dylib
)

function(add_gc_app app_name)
  add_executable(${app_name} ${ARGN})
  target_link_libraries(${app_name} ${APP_LIBS})
  install(TARGETS ${app_name} RUNTIME DESTINATION bin)
endfunction()

add_gc_app(mesh_info 
  mesh_info.cpp
)

add_gc_app(geodesic_distance 
  geodesic_distance.cpp
)

add_gc_app(mesh_sample 
  mesh_sample.cpp
)

add_gc_app(mesh_curvature 
  mesh_curvature.cpp
)

add_gc_app(mesh_convert 
  mesh_convert.cpp
)

add_gc_app(mesh_remesh 
  mesh_remesh.cpp
)

add_gc_app(direction_fields
  direction_fields.cpp
)

add_gc_app(parameterize
  parameterize.cpp
)

add_gc_app(subdivide
  subdivide.cpp
)

add_gc_app(mesh_normals
  mesh_normals.cpp
)

add_gc_app(simplify
  simplify.cpp
)

add_gc_app(symmetry_detect
  symmetry_detect.cpp
)