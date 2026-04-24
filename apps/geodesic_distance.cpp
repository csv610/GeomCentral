#include <iostream>
#include <sstream>
#include <cmath>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>
#include <geometrycentral/surface/heat_method_distance.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -s <idx>    Source vertex index (default: 0)" << std::endl;
    std::cout << "  -t <idx>    Target vertex index (default: auto)" << std::endl;
    std::cout << "  -T <val>    Time coefficient (default: 1.0)" << std::endl;
    std::cout << "  -r          Use robust Laplacian" << std::endl;
    std::cout << "  -h          Show this help" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath;
    int sourceIdx = 0;
    int targetIdx = -1;
    double timeCoef = 1.0;
    bool robust = false;
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-s" && i + 1 < argc) {
                std::istringstream(argv[++i]) >> sourceIdx;
            } else if (arg == "-t" && i + 1 < argc) {
                std::istringstream(argv[++i]) >> targetIdx;
            } else if (arg == "-T" && i + 1 < argc) {
                std::istringstream(argv[++i]) >> timeCoef;
            } else if (arg == "-r") {
                robust = true;
            } else if (arg == "-v") {
                verbose = true;
            } else if (arg == "-h") {
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

    if (verbose) std::cout << "Loading mesh from: " << inputPath << std::endl;

    std::unique_ptr<ManifoldSurfaceMesh> mesh;
    std::unique_ptr<VertexPositionGeometry> geometry;
    std::tie(mesh, geometry) = readManifoldSurfaceMesh(inputPath);
    
    if (!mesh) {
        std::cerr << "Error: Failed to load mesh (must be manifold)" << std::endl;
        return 1;
    }

    if (verbose) std::cout << "Mesh loaded: " << mesh->nVertices() << " vertices" << std::endl;

    if (sourceIdx < 0 || sourceIdx >= (int)mesh->nVertices()) {
        std::cerr << "Error: Invalid source vertex index" << std::endl;
        return 1;
    }

    if (targetIdx < 0 && mesh->nVertices() > 1) {
        targetIdx = (sourceIdx + 1) % mesh->nVertices();
        if (verbose) std::cout << "Using target vertex: " << targetIdx << std::endl;
    }

    if (targetIdx < 0 || targetIdx >= (int)mesh->nVertices()) {
        if (verbose) std::cout << "No target specified, computing full distance field" << std::endl;
    }

    if (verbose) std::cout << "Computing geodesics from vertex " << sourceIdx << std::endl;

    HeatMethodDistanceSolver solver(*geometry, timeCoef, robust);
    VertexData<double> distances = solver.computeDistance(mesh->vertex(sourceIdx));

    if (targetIdx >= 0) {
        double dist = distances[mesh->vertex(targetIdx)];
        std::cout << "Distance from vertex " << sourceIdx << " to " << targetIdx << ": " << dist << std::endl;
    } else {
        double minDist = 1e30, maxDist = 0, avgDist = 0;
        int count = 0;
        for (auto v : mesh->vertices()) {
            double d = distances[v];
            if (d < minDist) minDist = d;
            if (d > maxDist) maxDist = d;
            avgDist += d;
            count++;
        }
        avgDist /= count;
        
        std::cout << "Distance statistics from vertex " << sourceIdx << ":" << std::endl;
        std::cout << "  Min: " << minDist << std::endl;
        std::cout << "  Max: " << maxDist << std::endl;
        std::cout << "  Avg: " << avgDist << std::endl;
    }

    return 0;
}