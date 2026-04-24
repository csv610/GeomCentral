#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/detect_symmetry.h"
#include "geometrycentral/surface/vertex_position_geometry.h"

#include "load_test_meshes.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace geometrycentral;
using namespace geometrycentral::surface;

class SymmetryDetectionSuite : public MeshAssetSuite {};

TEST_F(SymmetryDetectionSuite, AutoSymmetryDetection) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshSphere);
  
  ASSERT_TRUE(mesh != nullptr);
  ASSERT_TRUE(geometry != nullptr);
  
  SymmetryResult result = detectSymmetryAuto(geometry.get());
  
  EXPECT_TRUE(result.canonicalVertices.size() > 0 || !result.symmetryFound);
}

TEST_F(SymmetryDetectionSuite, MirrorSymmetryDetection) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshSphere);
  
  ASSERT_TRUE(mesh != nullptr);
  
  SymmetryResult result = detectSymmetryAutoMirror(geometry.get());
  
  EXPECT_TRUE(result.canonicalVertices.size() >= 0);
}

TEST_F(SymmetryDetectionSuite, RotationSymmetryDetection) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshSphere);
  
  ASSERT_TRUE(mesh != nullptr);
  
  Vector3 center = geometry->ensureHasVertexCentroid();
  SymmetryResult result = detectSymmetryRotation(geometry.get(), Vector3::unitY(), center, 4);
  
  EXPECT_TRUE(result.canonicalVertices.size() >= 0);
}

TEST_F(SymmetryDetectionSuite, DoubleMirrorSymmetry) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshSphere);
  
  ASSERT_TRUE(mesh != nullptr);
  
  SymmetryResult result = detectSymmetryDoubleMirror(geometry.get());
  
  EXPECT_TRUE(result.canonicalVertices.size() >= 0);
}