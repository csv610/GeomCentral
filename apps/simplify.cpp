#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>
#include <geometrycentral/surface/quadric_error_simplification.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> <output> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -t <val>   Quadric error tolerance (default: 0.05)" << std::endl;
    std::cout << "  -v         Verbose output" << std::endl;
    std::cout << "  -h         Show this help message" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath;
    std::string outputPath;
    double tolerance = 0.05;
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-t" && i + 1 < argc) tolerance = std::stod(argv[++i]);
            else if (arg == "-v") verbose = true;
            else if (arg == "-h") { printUsage(argv[0]); return 0; }
        } else {
            if (inputPath.empty()) inputPath = arg;
            else outputPath = arg;
        }
    }

    if (inputPath.empty() || outputPath.empty()) {
        std::cerr << "Error: Input and output files required" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    std::cout << "Loading mesh from: " << inputPath << std::endl;
    
    std::unique_ptr<ManifoldSurfaceMesh> mesh;
    std::unique_ptr<VertexPositionGeometry> geometry;
    std::tie(mesh, geometry) = readManifoldSurfaceMesh(inputPath);
    
    if (!mesh) {
        std::cerr << "Error: Failed to load mesh" << std::endl;
        return 1;
    }

    std::cout << "Mesh: " << mesh->nVertices() << "V, " << mesh->nFaces() << "F" << std::endl;
    std::cout << "Quadric error tolerance: " << tolerance << std::endl;

    size_t originalV = mesh->nVertices();
    size_t originalF = mesh->nFaces();

    quadricErrorSimplify(*mesh, *geometry, tolerance);

    size_t finalV = mesh->nVertices();
    size_t finalF = mesh->nFaces();

    std::cout << "Simplified: " << originalV << "V -> " << finalV << "V" << std::endl;
    std::cout << "         " << originalF << "F -> " << finalF << "F" << std::endl;
    std::cout << "Reduction: " << std::fixed << std::setprecision(1) 
              << (100.0 * (originalV - finalV) / originalV) << "% vertices, "
              << (100.0 * (originalF - finalF) / originalF) << "% faces" << std::endl;

    writeSurfaceMesh(outputPath, *mesh, *geometry);
    std::cout << "Written to: " << outputPath << std::endl;

    return 0;
}