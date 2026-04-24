#include <iostream>
#include <sstream>
#include <algorithm>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>
#include <geometrycentral/surface/subdivide.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> <output> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -m <method>  Subdivision method (loop, catmull, linear)" << std::endl;
    std::cout << "  -i <num>     Number of iterations (default: 1)" << std::endl;
    std::cout << "  -v           Verbose output" << std::endl;
    std::cout << "  -h           Show this help message" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath;
    std::string outputPath;
    std::string method = "loop";
    int iterations = 1;
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-m" && i + 1 < argc) method = argv[++i];
            else if (arg == "-i" && i + 1 < argc) iterations = std::stoi(argv[++i]);
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
    std::cout << "Subdivision method: " << method << std::endl;
    std::cout << "Iterations: " << iterations << std::endl;

    for (int iter = 0; iter < iterations; iter++) {
        std::cout << "Subdividing (iteration " << (iter + 1) << ")..." << std::endl;
        
        if (method == "loop") {
            loopSubdivide(*mesh, *geometry);
        } else if (method == "catmull") {
            catmullClarkSubdivide(*mesh, *geometry);
        } else if (method == "linear") {
            linearSubdivide(*mesh, *geometry);
        } else {
            std::cerr << "Error: Unknown method '" << method << "'" << std::endl;
            return 1;
        }

        if (verbose) {
            std::cout << "  -> " << mesh->nVertices() << "V, " << mesh->nFaces() << "F" << std::endl;
        }
    }

    writeSurfaceMesh(outputPath, *mesh, *geometry);
    std::cout << "Written to: " << outputPath << std::endl;

    return 0;
}