#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>
#include <geometrycentral/surface/detect_symmetry.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -m <type>   Symmetry type (auto, mirror, rotation, double)" << std::endl;
    std::cout << "  -n <num>   For rotation: number of symmetries (default: 2)" << std::endl;
    std::cout << "  -o <file>  Output symmetry file (default: symmetry.txt)" << std::endl;
    std::cout << "  -v         Verbose output" << std::endl;
    std::cout << "  -h         Show this help message" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath;
    std::string outputPath = "symmetry.txt";
    std::string symType = "auto";
    int nSym = 2;
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-m" && i + 1 < argc) symType = argv[++i];
            else if (arg == "-n" && i + 1 < argc) nSym = std::stoi(argv[++i]);
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
    std::cout << "Detecting " << symType << " symmetry..." << std::endl;

    SymmetryResult result;
    
    if (symType == "auto") {
        result = detectSymmetryAuto(geometry.get());
    } else if (symType == "mirror") {
        result = detectSymmetryAutoMirror(geometry.get());
    } else if (symType == "rotation") {
        Vector3 center = geometry->ensureHasVertexCentroid();
        result = detectSymmetryRotation(geometry.get(), Vector3::unitY(), center, nSym);
    } else if (symType == "double") {
        result = detectSymmetryDoubleMirror(geometry.get());
    } else {
        std::cerr << "Error: Unknown symmetry type '" << symType << "'" << std::endl;
        return 1;
    }

    std::ofstream out(outputPath);
    out << "# Symmetry detection result" << std::endl;
    out << "# Mesh: " << inputPath << std::endl;
    out << "# Type: " << symType << std::endl;
    out << "symmetry_found: " << (result.symmetryFound ? "true" : "false") << std::endl;
    out << "canonical_vertices: " << result.canonicalVertices.size() << std::endl;
    
    for (size_t i = 0; i < result.canonicalVertices.size(); i++) {
        out << "canonical " << i << ": " << result.canonicalVertices[i] << std::endl;
    }
    
    out.close();

    std::cout << "Written to: " << outputPath << std::endl;

    if (verbose || result.symmetryFound) {
        std::cout << "\n=== Symmetry Detection ===" << std::endl;
        std::cout << "Symmetry found: " << (result.symmetryFound ? "Yes" : "No") << std::endl;
        if (result.symmetryFound) {
            std::cout << "Canonical vertices: " << result.canonicalVertices.size() << std::endl;
        }
    }

    if (!result.symmetryFound) {
        std::cout << "No symmetry detected." << std::endl;
    }

    return 0;
}