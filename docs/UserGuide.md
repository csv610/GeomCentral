# Geometry Central CLI Applications

A collection of command-line tools for mesh processing built on top of the geometry-central library.

## Prerequisites

- C++14 compiler
- Eigen3 (>= 3.3)
- SuiteSparse (for geodesic distance computations)
- Building the main library: `cd build && make`

## Building Apps

```bash
cd apps/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

The compiled binaries will be in `build/bin/`.

## Applications

### mesh_info

Prints comprehensive information about a mesh file.

```bash
./mesh_info <input> [options]
```

**Options:**
- `-v, --verbose` - Print verbose information
- `-s, --stats` - Print detailed statistics
- `-h, --help` - Show help

**Example:**
```bash
./mesh_info mesh.obj -v
```

Output:
```
Loading mesh from: mesh.obj

=== Mesh Information ===
Vertices: 1250
Halfedges: 3750
Edges: 1875
Faces: 1250
Corners: 3750
Boundary loops: 0

=== Mesh Properties ===
Manifold: Yes
Oriented: Yes
Has boundary: No
Genus: 1
Euler characteristic: 0
```

---

### geodesic_distance

Computes geodesic distances from a source vertex to all other vertices using the heat method.

```bash
./geodesic_distance <input> [options]
```

**Options:**
- `-s <idx>` - Source vertex index (default: 0)
- `-t <idx>` - Target vertex index (default: auto)
- `-T <val>` - Time coefficient (default: 1.0)
- `-r` - Use robust Laplacian
- `-v` - Verbose output
- `-h` - Show help

**Example:**
```bash
./geodesic_distance mesh.ply -s 0 -t 100 -v
```

Output:
```
Loading mesh from: mesh.ply
Mesh loaded: 1250 vertices
Computing geodesics from vertex 0
Distance from vertex 0 to 100: 2.345678

Distance statistics from vertex 0:
  Min: 0
  Max: 15.234567
  Avg: 5.678901
```

---

### mesh_sample

Performs Poisson disk sampling on a mesh surface to generate well-distributed sample points.

```bash
./mesh_sample <input> [options]
```

**Options:**
- `-n <num>` - Target number of samples (default: 100)
- `-m <dist>` - Minimum distance between samples (default: auto-computed)
- `-c <num>` - Candidates per iteration (default: 30)
- `-o <file>` - Output file (default: samples.pts)
- `-v` - Verbose output
- `-h` - Show help

**Example:**
```bash
./mesh_sample mesh.ply -n 500 -m 0.1 -o points.pts -v
```

Output:
```
Loading mesh from: mesh.ply
Mesh: 1250 verts, 2480 faces
Surface area: 15.678901
Target: 500 samples
Min distance: 0.1
Running Poisson disk sampling...
Generated 500 samples
Written to: points.pts
```

The output file format (`.pts`):
```
# Poisson disk samples
# Mesh: mesh.ply
# Count: 500
500 3
x1 y1 z1
x2 y2 z2
...
```

---

### mesh_curvature

Computes curvature statistics for a mesh.

```bash
./mesh_curvature <input> [options]
```

**Options:**
- `-o <file>` - Output file for curvature data
- `-v` - Verbose output
- `-h` - Show help

**Example:**
```bash
./mesh_curvature sphere.ply -v
```

Output:
```
Loading mesh from: sphere.ply
Computing curvatures...

=== Curvature Statistics ===
Gaussian curvature:
  Min: 0.123456
  Max: 0.234567
  Avg: 0.178901
Integral Gaussian curvature: 12.566370
Expected (closed genus 0): 12.566371
Deviation: 0.000001
```

---

### mesh_convert

Converts mesh files between different formats (OBJ, PLY, OFF, etc.).

```bash
./mesh_convert <input> <output> [options]
```

**Options:**
- `-f` - Force overwrite
- `-h` - Show help

**Example:**
```bash
./mesh_convert mesh.obj mesh.ply
```

Output:
```
Loading: mesh.obj
Mesh: 1250V, 1875E, 1250F
Writing: mesh.ply
Conversion complete.
```

---

### mesh_remesh

Remeshes a triangle mesh to achieve a target edge length.

```bash
./mesh_remesh <input> <output> [options]
```

**Options:**
- `-l <len>` - Target edge length (default: auto)
- `-i <num>` - Iterations (default: 10)
- `-v` - Verbose output
- `-h` - Show help

**Example:**
```bash
./mesh_remesh high_res.obj output.obj -l 0.5 -i 20 -v
```

Output:
```
Loading mesh from: high_res.obj
Mesh: 5000V, 10000F
Average edge length: 0.15
Target edge length: 0.5
Iterations: 20
Remeshing...
Remeshed: 850V, 1680F
Writing to: output.obj
New average edge length: 0.48
```

---

### direction_fields

Computes smooth direction fields on a mesh surface.

```bash
./direction_fields <input> [options]
```

**Options:**
- `-n <num>` - Number of symmetry directions (default: 1)
- `-f` - Compute face-based field instead of vertex-based
- `-a` - Align to curvature (nSym=2)
- `-b` - Align to boundary
- `-o <file>` - Output file (default: field.vec)
- `-v` - Verbose output
- `-h` - Show help

**Example:**
```bash
./direction_fields mesh.ply -n 4 -a -o field.vec -v
```

Output:
```
Loading mesh from: mesh.ply
Mesh: 1250V, 2480F
Computing direction field (nSym=4)...
Written to: field.vec

=== Field Indices ===
Positive: 45
Negative: 38
Zero: 2397
```

---

### parameterize

Computes UV parameterization/flattening of a mesh surface.

```bash
./parameterize <input> [options]
```

**Options:**
- `-m <method>` - Parameterization method (bff, lscm, harmonic)
- `-o <file>` - Output UV file (default: mesh.uv)
- `-v` - Verbose output
- `-h` - Show help

**Example:**
```bash
./parameterize mesh.ply -m bff -o mesh.uv -v
```

Output:
```
Loading mesh from: mesh.ply
Mesh: 1250V, 2480F
Computing BFF parameterization...
Written to: mesh.uv

=== UV Bounds ===
U: [0, 1.234567]
V: [0, 0.987654]
```

---

### subdivide

Subdivides a mesh to increase resolution.

```bash
./subdivide <input> <output> [options]
```

**Options:**
- `-m <method>` - Subdivision method (loop, catmull, linear)
- `-i <num>` - Number of iterations (default: 1)
- `-v` - Verbose output
- `-h` - Show help

**Example:**
```bash
./subdivide low_res.obj high_res.obj -m loop -i 2 -v
```

Output:
```
Loading mesh from: low_res.obj
Mesh: 500V, 996F
Subdivision method: loop
Iterations: 2
Subdividing (iteration 1)...
  -> 1996V, 3992F
Subdividing (iteration 2)...
  -> 7988V, 15976F
Written to: high_res.obj
```

---

### mesh_normals

Computes vertex or face normals for a mesh.

```bash
./mesh_normals <input> [options]
```

**Options:**
- `-f` - Compute face normals instead of vertex normals
- `-a` - Compute area-weighted normals
- `-o <file>` - Output normals file (default: normals.nrm)
- `-s` - Print statistics
- `-v` - Verbose output
- `-h` - Show help

**Example:**
```bash
./mesh_normals mesh.ply -v -s
```

Output:
```
Loading mesh from: mesh.ply
Mesh: 1250V, 2480F
Written to: normals.nrm

=== Geometry Statistics ===
Surface area: 15.678901
Vertex area - Min: 0.001234, Max: 0.056789, Avg: 0.012543
```

---

### simplify

Simplifies a mesh using quadric error metrics.

```bash
./simplify <input> <output> [options]
```

**Options:**
- `-t <val>` - Quadric error tolerance (default: 0.05)
- `-v` - Verbose output
- `-h` - Show help

**Example:**
```bash
./simplify high_res.obj low_res.obj -t 0.1 -v
```

Output:
```
Loading mesh from: high_res.obj
Mesh: 5000V, 9996F
Quadric error tolerance: 0.1
Simplified: 5000V -> 850V
         9996F -> 1680F
Reduction: 83.0% vertices, 83.2% faces
Written to: low_res.obj
```

---

### symmetry_detect

Detects rotational and mirror symmetries in a mesh.

```bash
./symmetry_detect <input> [options]
```

**Options:**
- `-m <type>` - Symmetry type (auto, mirror, rotation, double)
- `-n <num>` - For rotation: number of symmetries (default: 2)
- `-o <file>` - Output symmetry file (default: symmetry.txt)
- `-v` - Verbose output
- `-h` - Show help

**Example:**
```bash
./symmetry_detect mesh.ply -m auto -v
```

Output:
```
Loading mesh from: mesh.ply
Mesh: 1250V, 2480F
Detecting auto symmetry...
Written to: symmetry.txt

=== Symmetry Detection ===
Symmetry found: Yes
Canonical vertices: 625
```

---

## Supported Mesh Formats

- **OBJ** - Wavefront OBJ
- **PLY** - Polygon File Format (ASCII and binary)
- **OFF** - Object File Format
- **STL** - STL (ASCII and binary)

---

## Notes

1. Most tools require the mesh to be a manifold surface. Use `mesh_info` to check mesh properties.
2. Geodesic distance computation requires SuiteSparse libraries.
3. For large meshes, consider using the `-v` flag to monitor progress.