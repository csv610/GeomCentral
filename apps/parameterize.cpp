#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>
#include <geometrycentral/surface/boundary_first_flattening.h>
#include <geometrycentral/surface/parameterize.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -m <method>  Parameterization method (bff, lscm, harmonic)" << std::endl;
    std::cout << "  -o <file>    Output UV file (default: mesh.uv)" << std::endl;
    std::cout << "  -v           Verbose output" << std::endl;
    std::cout << "  -h           Show this help message" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath;
    std::string outputPath = "mesh.uv";
    std::string method = "bff";
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-m" && i + 1 < argc) method = argv[++i];
            else if (arg == "-o" && i + 1 < argc) outputPath = argv[++i];
            else if (arg == "-v") verbose = true;
            else if (arg == "-h") { printUsage(argv[0]); return 0; }
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
        std::cerr << "Error: Failed to load mesh" << std::endl;
        return 1;
    }

    std::cout << "Mesh: " << mesh->nVertices() << "V, " << mesh->nFaces() << "F" << std::endl;

    VertexData<Vector2> uv;
    
    if (method == "bff") {
        std::cout << "Computing BFF parameterization..." << std::endl;
        BFF bff(*mesh, *geometry);
        uv = bff.flatten();
    } else if (method == "lscm") {
        std::cout << "Computing LSCM parameterization..." << std::endl;
        uv = parameterizeLSCM(*mesh, *geometry);
    } else if (method == "harmonic") {
        std::cout << "Computing harmonic parameterization..." << std::endl;
        uv = parameterizeHarmonic(*mesh, *geometry);
    } else {
        std::cerr << "Error: Unknown method '" << method << "'" << std::endl;
        return 1;
    }

    std::ofstream out(outputPath);
    out << "# UV coordinates" << std::endl;
    out << "# Mesh: " << inputPath << std::endl;
    out << "# Method: " << method << std::endl;
    out << mesh->nVertices() << " 2" << std::endl;
    for (auto v : mesh->vertices()) {
        out << uv[v].x << " " << uv[v].y << std::endl;
    }
    out.close();

    std::cout << "Written to: " << outputPath << std::endl;

    if (verbose) {
        double minU = 1e20, maxU = -1e20, minV = 1e20, maxV = -1e20;
        for (auto v : mesh->vertices()) {
            minU = std::min(minU, uv[v].x);
            maxU = std::max(maxU, uv[v].x);
            minV = std::min(minV, uv[v].y);
            maxV = std::max(maxV, uv[v].y);
        }
        std::cout << "\n=== UV Bounds ===" << std::endl;
        std::cout << "U: [" << minU << ", " << maxU << "]" << std::endl;
        std::cout << "V: [" << minV << ", " << maxV << "]" << std::endl;
    }

    return 0;
}