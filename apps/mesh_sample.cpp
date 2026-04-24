#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>
#include <geometrycentral/surface/poisson_disk_sampler.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -n <num>   Target number of samples (default: 100)" << std::endl;
    std::cout << "  -m <dist> Minimum distance (default: auto)" << std::endl;
    std::cout << "  -c <num>  Candidates per iteration (default: 30)" << std::endl;
    std::cout << "  -o <file> Output file (default: samples.pts)" << std::endl;
    std::cout << "  -v        Verbose output" << std::endl;
    std::cout << "  -h        Show this help" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath;
    int targetSamples = 100;
    double minDist = 0.0;
    int candidates = 30;
    std::string outputPath = "samples.pts";
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-n" && i + 1 < argc) {
                std::istringstream(argv[++i]) >> targetSamples;
            } else if (arg == "-m" && i + 1 < argc) {
                std::istringstream(argv[++i]) >> minDist;
            } else if (arg == "-c" && i + 1 < argc) {
                std::istringstream(argv[++i]) >> candidates;
            } else if (arg == "-o" && i + 1 < argc) {
                outputPath = argv[++i];
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

    if (verbose) std::cout << "Mesh: " << mesh->nVertices() << " verts, " << mesh->nFaces() << " faces" << std::endl;

    double surfaceArea = 0.0;
    for (auto f : mesh->faces()) {
        surfaceArea += geometry->faceArea(f);
    }
    if (verbose) std::cout << "Surface area: " << surfaceArea << std::endl;

    if (minDist <= 0.0) {
        minDist = std::sqrt(surfaceArea / targetSamples);
    }

    if (verbose) {
        std::cout << "Target: " << targetSamples << " samples" << std::endl;
        std::cout << "Min distance: " << minDist << std::endl;
    }

    geometry->requireVertexPositions();
    VertexData<Vector3>& positions = geometry->vertexPositions;

    PoissonDiskSampler sampler(*mesh, *geometry);
    
    PoissonDiskOptions opts;
    opts.minDist = minDist;
    opts.kCandidates = candidates;

    std::cout << "Running Poisson disk sampling..." << std::endl;
    std::vector<SurfacePoint> samples = sampler.sample(opts);

    std::cout << "Generated " << samples.size() << " samples" << std::endl;

    std::ofstream outFile(outputPath);
    outFile << "# Poisson disk samples\n";
    outFile << "# Mesh: " << inputPath << "\n";
    outFile << "# Count: " << samples.size() << "\n";
    outFile << samples.size() << " 3\n";
    
    for (const auto& sp : samples) {
        Vector3 pos = sp.interpolate(positions);
        outFile << pos.x << " " << pos.y << " " << pos.z << "\n";
    }
    outFile.close();

    if (verbose) std::cout << "Written to: " << outputPath << std::endl;

    geometry->unrequireVertexPositions();

    return 0;
}