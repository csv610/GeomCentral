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
  MeshAsset a = getAsset("spot.ply", true);
  
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.geometry != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  size_t origV = a.mesh->nVertices();
  size_t origF = a.mesh->nFaces();
  
  loopSubdivide(*a.manifoldMesh, *a.geometry);
  
  EXPECT_GT(a.mesh->nVertices(), origV);
  EXPECT_GT(a.mesh->nFaces(), origF);
}

TEST_F(SubdivisionSuite, CatmullClarkSubdivide) {
  MeshAsset a = getAsset("spot.ply", true);
  
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  size_t origV = a.mesh->nVertices();
  size_t origF = a.mesh->nFaces();
  
  catmullClarkSubdivide(*a.manifoldMesh, *a.geometry);
  
  EXPECT_GT(a.mesh->nVertices(), origV);
  EXPECT_GT(a.mesh->nFaces(), origF);
}

TEST_F(SubdivisionSuite, LinearSubdivide) {
  MeshAsset a = getAsset("spot.ply", true);
  
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  size_t origV = a.mesh->nVertices();
  size_t origF = a.mesh->nFaces();
  
  linearSubdivide(*a.manifoldMesh, *a.geometry);
  
  EXPECT_GT(a.mesh->nVertices(), origV);
  EXPECT_GT(a.mesh->nFaces(), origF);
}

TEST_F(SubdivisionSuite, MultipleIterations) {
  MeshAsset a = getAsset("spot.ply", true);
  
  ASSERT_TRUE(a.mesh != nullptr);
  ASSERT_TRUE(a.manifoldMesh != nullptr);
  
  size_t origV = a.mesh->nVertices();
  
  for (int i = 0; i < 3; i++) {
    loopSubdivide(*a.manifoldMesh, *a.geometry);
  }
  
  EXPECT_GT(a.mesh->nVertices(), origV);
}