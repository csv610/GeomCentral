#include <iostream>
#include <sstream>
#include <algorithm>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -v, --verbose    Print verbose information" << std::endl;
    std::cout << "  -s, --stats     Print detailed statistics" << std::endl;
    std::cout << "  -h, --help     Show this help message" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath;
    bool verbose = false;
    bool stats = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-v" || arg == "--verbose") verbose = true;
            else if (arg == "-s" || arg == "--stats") stats = true;
            else if (arg == "-h" || arg == "--help") {
                printUsage(argv[0]);
                return 0;
            }
        } else {
            inputPath = arg;
        }
    }

    if (inputPath.empty()) {
        std::cerr << "Error: No input file specified" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    std::cout << "Loading mesh from: " << inputPath << std::endl;
    
    std::unique_ptr<ManifoldSurfaceMesh> mesh;
    std::unique_ptr<VertexPositionGeometry> geometry;
    std::tie(mesh, geometry) = readManifoldSurfaceMesh(inputPath);
    
    if (!mesh) {
        std::cerr << "Error: Failed to load mesh (must be manifold)" << std::endl;
        return 1;
    }

    std::cout << "\n=== Mesh Information ===" << std::endl;
    std::cout << "Vertices: " << mesh->nVertices() << std::endl;
    std::cout << "Halfedges: " << mesh->nHalfedges() << std::endl;
    std::cout << "Edges: " << mesh->nEdges() << std::endl;
    std::cout << "Faces: " << mesh->nFaces() << std::endl;
    std::cout << "Corners: " << mesh->nCorners() << std::endl;
    std::cout << "Boundary loops: " << mesh->nBoundaryLoops() << std::endl;

    if (verbose) {
        std::cout << "\n=== Element Counts ===" << std::endl;
        std::cout << "Interior halfedges: " << mesh->nInteriorHalfedges() << std::endl;
        std::cout << "Exterior halfedges: " << mesh->nExteriorHalfedges() << std::endl;
        
        size_t nTri = 0, nQuad = 0, nOther = 0;
        for (auto f : mesh->faces()) {
            size_t deg = f.degree();
            if (deg == 3) nTri++;
            else if (deg == 4) nQuad++;
            else nOther++;
        }
        std::cout << "Triangular faces: " << nTri << std::endl;
        std::cout << "Quad faces: " << nQuad << std::endl;
        std::cout << "Other faces: " << nOther << std::endl;
    }

    bool isOriented = mesh->isOriented();
    bool hasBoundary = mesh->hasBoundary();

    std::cout << "\n=== Mesh Properties ===" << std::endl;
    std::cout << "Manifold: Yes" << std::endl;
    std::cout << "Oriented: " << (isOriented ? "Yes" : "No") << std::endl;
    std::cout << "Has boundary: " << (hasBoundary ? "Yes" : "No") << std::endl;

    long genus = mesh->genus();
    long eulerChar = mesh->eulerCharacteristic();
    
    if (!hasBoundary) {
        std::cout << "Genus: " << genus << std::endl;
        std::cout << "Euler characteristic: " << eulerChar << std::endl;
    } else {
        std::cout << "Euler characteristic: " << eulerChar << std::endl;
        std::cout << "Genus: " << genus << " (partial, closed extension)" << std::endl;
    }

    if (stats) {
        std::cout << "\n=== Detailed Statistics ===" << std::endl;
        std::cout << "Connected components: " << mesh->nConnectedComponents() << std::endl;
        
        double area = 0.0;
        for (auto f : mesh->faces()) {
            area += geometry->faceArea(f);
        }
        std::cout << "Surface area: " << area << std::endl;
    }

    return 0;
}