#include <iostream>
#include <sstream>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>
#include <geometrycentral/surface/remeshing.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> <output> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -l <len>    Target edge length (default: auto)" << std::endl;
    std::cout << "  -i <num>   Iterations (default: 10)" << std::endl;
    std::cout << "  -v         Verbose output" << std::endl;
    std::cout << "  -h         Show this help" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath, outputPath;
    double targetLength = -1.0;
    size_t iterations = 10;
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-l" && i + 1 < argc) {
                std::istringstream(argv[++i]) >> targetLength;
            } else if (arg == "-i" && i + 1 < argc) {
                std::istringstream(argv[++i]) >> iterations;
            } else if (arg == "-v") {
                verbose = true;
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

    if (verbose) std::cout << "Loading mesh from: " << inputPath << std::endl;

    std::unique_ptr<ManifoldSurfaceMesh> mesh;
    std::unique_ptr<VertexPositionGeometry> geometry;
    std::tie(mesh, geometry) = readManifoldSurfaceMesh(inputPath);
    
    if (!mesh) {
        std::cerr << "Error: Failed to load mesh (must be manifold)" << std::endl;
        return 1;
    }

    if (verbose) std::cout << "Mesh: " << mesh->nVertices() << "V, " << mesh->nFaces() << "F" << std::endl;

    geometry->requireVertexPositions();
    VertexData<Vector3>& positions = geometry->vertexPositions;

    double totalLen = 0;
    for (auto e : mesh->edges()) {
        totalLen += geometry->edgeLength(e);
    }
    double avgLength = totalLen / mesh->nEdges();
    
    if (verbose) std::cout << "Average edge length: " << avgLength << std::endl;

    if (targetLength <= 0) {
        targetLength = avgLength * 1.5;
    }

    if (verbose) {
        std::cout << "Target edge length: " << targetLength << std::endl;
        std::cout << "Iterations: " << iterations << std::endl;
    }

    RemeshOptions opts;
    opts.targetEdgeLength = targetLength;
    opts.maxIterations = iterations;

    std::cout << "Remeshing..." << std::endl;

    remesh(*mesh, *geometry, opts);

    std::cout << "Remeshed: " << mesh->nVertices() << "V, " << mesh->nFaces() << "F" << std::endl;

    std::cout << "Writing to: " << outputPath << std::endl;
    writeSurfaceMesh(*mesh, *geometry, outputPath);

    geometry->unrequireVertexPositions();

    return 0;
}