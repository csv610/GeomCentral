#pragma once 

#include "geometrycentral/surface/geometry.h"
#include "geometrycentral/surface/embedded_geometry_interface.h"

#include <vector>

namespace geometrycentral {
namespace surface {

struct SymmetryResult {
  bool symmetryFound = false;                  // was a symmetry found? is this data valid?
  std::vector<Vertex> canonicalVertices;       // a representative entry from each
                                               // set of symmetry pairs
  VertexData<std::vector<Vertex>> symmetrySet; // for each unique vertex,
                                               // all others vertices that
                                               // are symmetry pairs
};

// Look for a symmetry about a mirror plane
SymmetryResult detectSymmetryMirror(EmbeddedGeometryInterface* geom, Vector3 planeNormal, Vector3 planePoint);

// Look for a rotational symmetry
SymmetryResult detectSymmetryRotation(EmbeddedGeometryInterface* geom, Vector3 rotAxis, Vector3 rotPoint, int nSym);

// Automatically search for the typical mirror and rotation symmetries about the
// shape center
// Returns any symmetry which is found.
SymmetryResult detectSymmetryAuto(EmbeddedGeometryInterface* geom);
SymmetryResult detectSymmetryAutoRotation(EmbeddedGeometryInterface* geom);
SymmetryResult detectSymmetryAutoMirror(EmbeddedGeometryInterface* geom);

// Look for symmetry which is mirrored over the y and z planes
SymmetryResult detectSymmetryDoubleMirror(EmbeddedGeometryInterface* geom);

} // namespace surface
} // namespace geometrycentral
