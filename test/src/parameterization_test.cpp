#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/boundary_first_flattening.h"
#include "geometrycentral/surface/parameterize.h"
#include "geometrycentral/surface/vertex_position_geometry.h"

#include "load_test_meshes.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace geometrycentral;
using namespace geometrycentral::surface;

class ParameterizationSuite : public MeshAssetSuite {};

TEST_F(ParameterizationSuite, BFFParameterization) {
  MeshAsset a = getAsset("spot.ply", true);
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.geometry != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  BFF bff(*a.manifoldMesh, *a.geometry);
  VertexData<Vector2> uv = bff.flatten();
  
  ASSERT_EQ(uv.size(), a.mesh->nVertices());
  
  for (auto v : a.mesh->vertices()) {
    EXPECT_TRUE(std::isfinite(uv[v].x));
    EXPECT_TRUE(std::isfinite(uv[v].y));
  }
}

TEST_F(ParameterizationSuite, LSCMParameterization) {
  MeshAsset a = getAsset("spot.ply", true);
  
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  VertexData<Vector2> uv = parameterizeDisk(*a.manifoldMesh, *a.geometry);
  
  ASSERT_EQ(uv.size(), a.mesh->nVertices());
  
  for (auto v : a.mesh->vertices()) {
    EXPECT_TRUE(std::isfinite(uv[v].x));
    EXPECT_TRUE(std::isfinite(uv[v].y));
  }
}

TEST_F(ParameterizationSuite, HarmonicParameterization) {
  MeshAsset a = getAsset("spot.ply", true);
  
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  VertexData<Vector2> uv = parameterizeDisk(*a.manifoldMesh, *a.geometry);
  
  ASSERT_EQ(uv.size(), a.mesh->nVertices());
  
  for (auto v : a.mesh->vertices()) {
    EXPECT_TRUE(std::isfinite(uv[v].x));
    EXPECT_TRUE(std::isfinite(uv[v].y));
  }
}