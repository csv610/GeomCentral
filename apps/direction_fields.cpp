#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>
#include <geometrycentral/surface/direction_fields.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -n <num>    Number of symmetry directions (default: 1)" << std::endl;
    std::cout << "  -f         Compute face-based field instead of vertex-based" << std::endl;
    std::cout << "  -a         Align to curvature (nSym=2, requires intrinsic geom)" << std::endl;
    std::cout << "  -b         Align to boundary" << std::endl;
    std::cout << "  -o <file>  Output file for field (default: field.vec)" << std::endl;
    std::cout << "  -v         Verbose output" << std::endl;
    std::cout << "  -h         Show this help message" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath;
    std::string outputPath = "field.vec";
    int nSym = 1;
    bool faceBased = false;
    bool alignCurvature = false;
    bool alignBoundary = false;
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-n" && i + 1 < argc) nSym = std::stoi(argv[++i]);
            else if (arg == "-f") faceBased = true;
            else if (arg == "-a") { alignCurvature = true; nSym = 2; }
            else if (arg == "-b") alignBoundary = true;
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
    std::cout << "Computing direction field (nSym=" << nSym << ")..." << std::endl;

    VertexData<Vector2> field;

    if (faceBased) {
        if (alignCurvature) {
            field = computeSmoothestFaceDirectionField(*geometry, nSym);
        } else if (alignBoundary) {
            field = computeSmoothestBoundaryAlignedFaceDirectionField(*geometry, nSym);
        } else {
            field = computeSmoothestFaceDirectionField(*geometry, nSym);
        }
    } else {
        if (alignCurvature) {
            field = computeCurvatureAlignedVertexDirectionField(*geometry, nSym);
        } else if (alignBoundary) {
            field = computeSmoothestBoundaryAlignedVertexDirectionField(*geometry, nSym);
        } else {
            field = computeSmoothestVertexDirectionField(*geometry, nSym);
        }
    }

    std::ofstream out(outputPath);
    out << "# Direction field" << std::endl;
    out << "# Mesh: " << inputPath << std::endl;
    out << "# Symmetries: " << nSym << std::endl;
    out << mesh->nVertices() << " 2" << std::endl;
    for (auto v : mesh->vertices()) {
        out << field[v].x << " " << field[v].y << std::endl;
    }
    out.close();

    std::cout << "Written to: " << outputPath << std::endl;

    if (verbose) {
        FaceData<int> faceIndices = computeFaceIndex(*geometry, field, nSym);
        int nPositive = 0, nNegative = 0, nZero = 0;
        for (auto f : mesh->faces()) {
            if (faceIndices[f] > 0) nPositive++;
            else if (faceIndices[f] < 0) nNegative++;
            else nZero++;
        }
        std::cout << "\n=== Field Indices ===" << std::endl;
        std::cout << "Positive: " << nPositive << std::endl;
        std::cout << "Negative: " << nNegative << std::endl;
        std::cout << "Zero: " << nZero << std::endl;
    }

    return 0;
}