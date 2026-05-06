# GeomCentral

**GeomCentral** is a modern C++ library for geometry processing, focusing on surface meshes and point clouds. This repository is a port of the original [geometry-central](https://github.com/nmwsharp/geometry-central) library, maintained to ensure compatibility and ease of use for various geometry processing tasks.

---

## 🚀 Enhancing the CLI Experience

One of the primary goals of this port is to bridge the gap between the library's vast internal capabilities and their availability through command-line tools. 

In the original software, many of the powerful algorithms were only accessible by writing C++ code. This version includes an expanded suite of CLI tools, making the library's features more accessible to researchers and developers who need to process meshes directly from the terminal. 

New and improved CLI tools include:
- **`mesh_info`**: Detailed statistics and sanity checks for mesh data.
- **`mesh_convert`**: Efficient format conversion.
- **`geodesic_distance`**: Compute distances across surfaces.
- **`mesh_remesh` & `simplify`**: Tools for modifying and optimizing mesh density.
- **`mesh_curvature` & `mesh_normals`**: Direct extraction of geometric properties.
- **`parameterize` & `subdivide`**: Advanced surface processing tools.

---

## 🔍 Exploration & Comparisons

This project is part of a broader exploration of **simplicity and usability** in geometry processing software. 

We are actively investigating the streamlined, data-centric philosophy found in libraries like **libIGL** and applying those lessons to this port. A key focus is evaluating how **GeomCentral** performs relative to industry standards like **libIGL** and **CGAL**, specifically from an end-user experience perspective. 

Our goal is to combine the deep, specialized algorithms of Geometry Central with the developer-friendly simplicity that makes modern geometry processing accessible to a wider audience.

---

## 🌟 Features

GeomCentral provides a comprehensive suite of tools for researchers and developers working with geometric data:

- **Surface Mesh Data Structure**: A high-performance, flexible halfedge mesh implementation with efficient element containers.
- **Geometric Quantities**: Built-in support for normals, curvatures, tangent bases, and discrete differential geometry operators (Laplacian, etc.).
- **Advanced Algorithms**: 
  - Geodesic distance computation (Heat Method, etc.)
  - Direction field generation and processing.
  - Intrinsic Delaunay triangulations and signpost intrinsic triangulations.
  - Parameterization and remeshing tools.
- **Numerical Tools**: A set of sparse linear algebra utilities built on top of [Eigen](http://eigen.tuxfamily.org/), with support for SuiteSparse when available.

---

## 🛠 Getting Started

### Prerequisites
- A modern C++ compiler (supporting C++20 or later).
- [CMake](https://cmake.org/) (version 3.14+ recommended).

### Building the Library and Tools
To build GeomCentral and the expanded set of CLI tools:

```bash
mkdir build && cd build
cmake ..
make -j4
```
The compiled binaries will be available in the `apps/` directory within your build folder.

### Basic Usage Example (C++ API)
Here is a quick snippet showing how to load a mesh and compute vertex areas:

```cpp
#include "geometrycentral/surface/surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/vertex_position_geometry.h"

using namespace geometrycentral;
using namespace geometrycentral::surface;

int main() {
    // Load a mesh and its geometry
    std::unique_ptr<SurfaceMesh> mesh;
    std::unique_ptr<VertexPositionGeometry> geometry;
    std::tie(mesh, geometry) = readSurfaceMesh("mesh.obj"); 

    // Compute face areas
    geometry->requireFaceAreas();

    // Use a VertexData container to store results
    VertexData<double> vertexAreas(*mesh);

    for(Vertex v : mesh->vertices()) {
        double area = 0.0;
        for(Face f : v.adjacentFaces()) {
            area += geometry->faceAreas[f] / 3.0; // Simple approximation
        }
        vertexAreas[v] = area;
    }

    return 0;
}
```

---

## 🤝 Contributions

Contributions are welcome! If you find bugs, have feature requests, or want to contribute code—especially new CLI tools to further bridge the gap—please get involved:

1. **Bug Reports**: Please use the GitHub issues tracker to report bugs.
2. **Pull Requests**: For code contributions, please fork the repository and submit a pull request. Ensure your code follows the existing style and includes tests where applicable.
3. **Feedback**: We value your input on how to make the library better for the community.

---

## 📜 Credits & Citation

This library is a port of the original **geometry-central**, which was primarily developed by [Nicholas Sharp](http://nmwsharp.com) along with contributions from Keenan Crane, Yousuf Soliman, Mark Gillespie, Rohan Sawhney, and others.

If you use this library in an academic publication, please cite the original work:

```bibtex
@article{geometrycentral,
  title={GeometryCentral: A modern C++ library of data structures and algorithms for geometry processing},
  author={Nicholas Sharp and Keenan Crane and others},
  howpublished="\url{https://geometry-central.net/}",
  year={2019}
}
```

---

## 📄 License

GeomCentral is released under the [MIT License](LICENSE). 

For more detailed information, tutorials, and documentation, visit the original documentation site at [geometry-central.net](http://geometry-central.net).
