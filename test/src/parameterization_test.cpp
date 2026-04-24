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
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshAPath);
  
  ASSERT_TRUE(mesh != nullptr);
  ASSERT_TRUE(geometry != nullptr);
  
  BFF bff(*mesh, *geometry);
  VertexData<Vector2> uv = bff.flatten();
  
  ASSERT_EQ(uv.size(), mesh->nVertices());
  
  for (auto v : mesh->vertices()) {
    EXPECT_TRUE(std::isfinite(uv[v].x));
    EXPECT_TRUE(std::isfinite(uv[v].y));
  }
}

TEST_F(ParameterizationSuite, LSCMParameterization) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshAPath);
  
  ASSERT_TRUE(mesh != nullptr);
  
  VertexData<Vector2> uv = parameterizeLSCM(*mesh, *geometry);
  
  ASSERT_EQ(uv.size(), mesh->nVertices());
  
  for (auto v : mesh->vertices()) {
    EXPECT_TRUE(std::isfinite(uv[v].x));
    EXPECT_TRUE(std::isfinite(uv[v].y));
  }
}

TEST_F(ParameterizationSuite, HarmonicParameterization) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshAPath);
  
  ASSERT_TRUE(mesh != nullptr);
  
  VertexData<Vector2> uv = parameterizeHarmonic(*mesh, *geometry);
  
  ASSERT_EQ(uv.size(), mesh->nVertices());
  
  for (auto v : mesh->vertices()) {
    EXPECT_TRUE(std::isfinite(uv[v].x));
    EXPECT_TRUE(std::isfinite(uv[v].y));
  }
}