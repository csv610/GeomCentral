#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/subdivide.h"
#include "geometrycentral/surface/vertex_position_geometry.h"

#include "load_test_meshes.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace geometrycentral;
using namespace geometrycentral::surface;

class SubdivisionSuite : public MeshAssetSuite {};

TEST_F(SubdivisionSuite, LoopSubdivide) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshTriangle);
  
  ASSERT_TRUE(mesh != nullptr);
  ASSERT_TRUE(geometry != nullptr);
  
  size_t origV = mesh->nVertices();
  size_t origF = mesh->nFaces();
  
  loopSubdivide(*mesh, *geometry);
  
  EXPECT_GT(mesh->nVertices(), origV);
  EXPECT_GT(mesh->nFaces(), origF);
}

TEST_F(SubdivisionSuite, CatmullClarkSubdivide) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshTriangle);
  
  ASSERT_TRUE(mesh != nullptr);
  
  size_t origV = mesh->nVertices();
  size_t origF = mesh->nFaces();
  
  catmullClarkSubdivide(*mesh, *geometry);
  
  EXPECT_GT(mesh->nVertices(), origV);
  EXPECT_GT(mesh->nFaces(), origF);
}

TEST_F(SubdivisionSuite, LinearSubdivide) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshTriangle);
  
  ASSERT_TRUE(mesh != nullptr);
  
  size_t origV = mesh->nVertices();
  size_t origF = mesh->nFaces();
  
  linearSubdivide(*mesh, *geometry);
  
  EXPECT_GT(mesh->nVertices(), origV);
  EXPECT_GT(mesh->nFaces(), origF);
}

TEST_F(SubdivisionSuite, MultipleIterations) {
  std::unique_ptr<ManifoldSurfaceMesh> mesh;
  std::unique_ptr<VertexPositionGeometry> geometry;
  std::tie(mesh, geometry) = readSurfaceMesh(testMeshTriangle);
  
  ASSERT_TRUE(mesh != nullptr);
  
  size_t origV = mesh->nVertices();
  
  for (int i = 0; i < 3; i++) {
    loopSubdivide(*mesh, *geometry);
  }
  
  EXPECT_GT(mesh->nVertices(), origV);
}