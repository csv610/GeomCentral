#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/direction_fields.h"
#include "geometrycentral/surface/vertex_position_geometry.h"

#include "load_test_meshes.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace geometrycentral;
using namespace geometrycentral::surface;

class DirectionFieldsSuite : public MeshAssetSuite {};

TEST_F(DirectionFieldsSuite, ComputeSmoothestVertexField) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshAPath);
  
  ASSERT_TRUE(mesh != nullptr);
  ASSERT_TRUE(geometry != nullptr);
  
  VertexData<Vector2> field = computeSmoothestVertexDirectionField(*geometry, 1);
  
  ASSERT_EQ(field.size(), mesh->nVertices());
  
  for (auto v : mesh->vertices()) {
    double mag = field[v].norm();
    EXPECT_NEAR(mag, 1.0, 1e-6);
  }
}

TEST_F(DirectionFieldsSuite, ComputeSymmetryField) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshAPath);
  
  ASSERT_TRUE(mesh != nullptr);
  
  VertexData<Vector2> field = computeSmoothestVertexDirectionField(*geometry, 4);
  
  for (auto v : mesh->vertices()) {
    double mag = field[v].norm();
    EXPECT_NEAR(mag, 1.0, 1e-6);
  }
}

TEST_F(DirectionFieldsSuite, FaceField) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshAPath);
  
  ASSERT_TRUE(mesh != nullptr);
  
  FaceData<Vector2> field = computeSmoothestFaceDirectionField(*geometry, 1);
  
  ASSERT_EQ(field.size(), mesh->nFaces());
  
  for (auto f : mesh->faces()) {
    double mag = field[f].norm();
    EXPECT_NEAR(mag, 1.0, 1e-6);
  }
}

TEST_F(DirectionFieldsSuite, ComputeIndices) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshAPath);
  
  ASSERT_TRUE(mesh != nullptr);
  
  VertexData<Vector2> field = computeSmoothestVertexDirectionField(*geometry, 1);
  FaceData<int> faceIndices = computeFaceIndex(*geometry, field, 1);
  
  ASSERT_EQ(faceIndices.size(), mesh->nFaces());
}