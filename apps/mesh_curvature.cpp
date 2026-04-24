#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <limits>

#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -o <file> Output file for curvature data" << std::endl;
    std::cout << "  -v        Verbose output" << std::endl;
    std::cout << "  -h        Show this help" << std::endl;
}

int main(int argc, char** argv) {
    std::string inputPath;
    std::string outputPath;
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-o" && i + 1 < argc) {
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

    if (verbose) std::cout << "Computing curvatures..." << std::endl;

    geometry->requireVertexGaussianCurvatures();
    VertexData<double>& gaussCurv = geometry->vertexGaussianCurvatures;

    double minKg = 1e30, maxKg = -1e30, avgKg = 0;
    double integralGauss = 0;
    int count = 0;

    for (auto v : mesh->vertices()) {
        double kg = gaussCurv[v];
        if (kg < minKg) minKg = kg;
        if (kg > maxKg) maxKg = kg;
        avgKg += kg;
        count++;
    }
    avgKg /= count;

    geometry->requireFaceAreas();
    for (auto f : mesh->faces()) {
        integralGauss += geometry->faceArea(f) * geometry->faceGaussianCurvatures[f];
    }

    std::cout << "\n=== Curvature Statistics ===" << std::endl;
    std::cout << "Gaussian curvature:" << std::endl;
    std::cout << "  Min: " << minKg << std::endl;
    std::cout << "  Max: " << maxKg << std::endl;
    std::cout << "  Avg: " << avgKg << std::endl;
    std::cout << "Integral Gaussian curvature: " << integralGauss << std::endl;
    std::cout << "Expected (closed genus 0): " << 4 * M_PI << std::endl;
    std::cout << "Deviation: " << std::abs(integralGauss - 4 * M_PI) << std::endl;

    if (!outputPath.empty()) {
        std::ofstream outFile(outputPath);
        outFile << "# Curvature data\n# vIdx gaussian\n";
        
        int idx = 0;
        for (auto v : mesh->vertices()) {
            outFile << idx << " " << gaussCurv[v] << "\n";
            idx++;
        }
        outFile.close();
        std::cout << "\nCurvature data written to: " << outputPath << std::endl;
    }

    geometry->unrequireVertexGaussianCurvatures();
    geometry->unrequireFaceAreas();

    return 0;
}