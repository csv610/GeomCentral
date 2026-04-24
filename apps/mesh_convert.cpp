#include <iostream>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> <output> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -f          Force overwrite" << std::endl;
    std::cout << "  -h          Show this help" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath, outputPath;
    bool force = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-f" || arg == "--force") {
                force = true;
            } else if (arg == "-h") {
                printUsage(argv[0]);
                return 0;
            }
        } else if (inputPath.empty()) {
            inputPath = arg;
        } else {
            outputPath = arg;
        }
    }

    if (inputPath.empty() || outputPath.empty()) {
        std::cerr << "Error: Input and output files required" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    std::cout << "Loading: " << inputPath << std::endl;

    std::unique_ptr<ManifoldSurfaceMesh> mesh;
    std::unique_ptr<VertexPositionGeometry> geometry;
    std::tie(mesh, geometry) = readManifoldSurfaceMesh(inputPath);
    
    if (!mesh) {
        std::cerr << "Error: Failed to load mesh (must be manifold)" << std::endl;
        return 1;
    }

    std::cout << "Mesh: " << mesh->nVertices() << "V, " << mesh->nEdges() << "E, " 
              << mesh->nFaces() << "F" << std::endl;

    std::cout << "Writing: " << outputPath << std::endl;

    writeSurfaceMesh(*mesh, *geometry, outputPath);

    std::cout << "Conversion complete." << std::endl;
    return 0;
}