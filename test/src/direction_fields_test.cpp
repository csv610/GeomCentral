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
  MeshAsset a = getAsset("spot.ply", true);
  
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.geometry != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  VertexData<Vector2> field = computeSmoothestVertexDirectionField(*a.geometry, 1);
  
  ASSERT_EQ(field.size(), a.mesh->nVertices());
  
  for (auto v : a.mesh->vertices()) {
    double mag = field[v].norm();
    EXPECT_NEAR(mag, 1.0, 1e-6);
  }
}

TEST_F(DirectionFieldsSuite, ComputeSymmetryField) {
  MeshAsset a = getAsset("spot.ply", true);
  
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  VertexData<Vector2> field = computeSmoothestVertexDirectionField(*a.geometry, 4);
  
  for (auto v : a.mesh->vertices()) {
    double mag = field[v].norm();
    EXPECT_NEAR(mag, 1.0, 1e-6);
  }
}

TEST_F(DirectionFieldsSuite, FaceField) {
  MeshAsset a = getAsset("spot.ply", true);
  
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  FaceData<Vector2> field = computeSmoothestFaceDirectionField(*a.geometry, 1);
  
  ASSERT_EQ(field.size(), a.mesh->nFaces());
  
  for (auto f : a.mesh->faces()) {
    double mag = field[f].norm();
    EXPECT_NEAR(mag, 1.0, 1e-6);
  }
}

TEST_F(DirectionFieldsSuite, ComputeIndices) {
  MeshAsset a = getAsset("spot.ply", true);
  
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  VertexData<Vector2> field = computeSmoothestVertexDirectionField(*a.geometry, 1);
  FaceData<int> faceIndices = computeFaceIndex(*a.geometry, field, 1);
  
  ASSERT_EQ(faceIndices.size(), a.mesh->nFaces());
}