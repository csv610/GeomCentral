#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -f         Compute face normals instead of vertex normals" << std::endl;
    std::cout << "  -a         Compute area-weighted normals" << std::endl;
    std::cout << "  -o <file>  Output normals file (default: normals.nrm)" << std::endl;
    std::cout << "  -s         Print statistics" << std::endl;
    std::cout << "  -v         Verbose output" << std::endl;
    std::cout << "  -h         Show this help message" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath;
    std::string outputPath = "normals.nrm";
    bool faceNormals = false;
    bool areaWeighted = false;
    bool stats = false;
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-f") faceNormals = true;
            else if (arg == "-a") areaWeighted = true;
            else if (arg == "-o" && i + 1 < argc) outputPath = argv[++i];
            else if (arg == "-s") stats = true;
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

    std::ofstream out(outputPath);
    out << "# Normals" << std::endl;
    out << "# Mesh: " << inputPath << std::endl;
    out << "# Type: " << (faceNormals ? "face" : "vertex") << std::endl;

    if (faceNormals) {
        out << mesh->nFaces() << " 3" << std::endl;
        for (auto f : mesh->faces()) {
            Vector3 n = geometry->faceNormal(f);
            if (areaWeighted) n *= geometry->faceArea(f);
            n = n.normalize();
            out << n.x << " " << n.y << " " << n.z << std::endl;
        }
    } else {
        geometry->requireVertexNormals();
        out << mesh->nVertices() << " 3" << std::endl;
        for (auto v : mesh->vertices()) {
            Vector3 n = geometry->vertexNormal(v);
            if (areaWeighted) n *= geometry->vertexArea(v);
            n = n.normalize();
            out << n.x << " " << n.y << " " << n.z << std::endl;
        }
    }
    out.close();

    std::cout << "Written to: " << outputPath << std::endl;

    if (stats || verbose) {
        double totalArea = 0.0;
        double minAngle = 1e20, maxAngle = -1e20;
        
        for (auto f : mesh->faces()) {
            totalArea += geometry->faceArea(f);
        }

        std::cout << "\n=== Geometry Statistics ===" << std::endl;
        std::cout << "Surface area: " << std::fixed << std::setprecision(6) << totalArea << std::endl;
        
        if (faceNormals) {
            double minArea = 1e20, maxArea = -1e20, avgArea = 0.0;
            for (auto f : mesh->faces()) {
                double a = geometry->faceArea(f);
                minArea = std::min(minArea, a);
                maxArea = std::max(maxArea, a);
                avgArea += a;
            }
            avgArea /= mesh->nFaces();
            std::cout << "Face area - Min: " << minArea << ", Max: " << maxArea << ", Avg: " << avgArea << std::endl;
        } else {
            double minArea = 1e20, maxArea = -1e20, avgArea = 0.0;
            for (auto v : mesh->vertices()) {
                double a = geometry->vertexArea(v);
                minArea = std::min(minArea, a);
                maxArea = std::max(maxArea, a);
                avgArea += a;
            }
            avgArea /= mesh->nVertices();
            std::cout << "Vertex area - Min: " << minArea << ", Max: " << maxArea << ", Avg: " << avgArea << std::endl;
        }
    }

    return 0;
}