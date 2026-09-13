# Tree-Indexed VectorStore in C++

A custom-built, in-memory high-dimensional vector store implemented in **C++17**. The system employs a dual-tree indexing mechanism, combining an **AVL Tree** and a **Red-Black Tree (RBT)** to optimize semantic search, multi-metric nearest-neighbor queries, and distance range filtering on vector embeddings.

Developed as part of the **Data Structures and Algorithms (CO2003)** curriculum at Ho Chi Minh City University of Technology (HCMUT).

## Architecture & Search Pipeline
```text
                          [Query Vector q]
                                 │
                     Compute norm: nq = ||q||
                     Estimate radius: D via Linear-in-k
                                 │
                   ┌─────────────┴─────────────┐
                   ▼                           ▼
        [Secondary Index: RBT]       [Primary Index: AVL Tree]
        - Keyed by: Euclidean Norm   - Keyed by: Distance to Ref Vector
        - Range filtering via:       - Maintains distance order
          [nq - D, nq + D]           - Distance-based range queries
                   │                           │
                   ▼                           ▼
            m Candidates                  Range IDs
        (m << n search space)
                   │
                   ▼
      Precise Distance Evaluation
       (Cosine / L1 / L2 Metric)
                   │
                   ▼
             Top-k Results
```

### 1. Primary Index: AVL Tree (Distance-to-Reference)
* **Key:** Euclidean distance from each vector to a global `referenceVector`.
* **Balance:** Height balance factor kept within `[-1, 1]` via single and double rotations, guaranteeing O(log n) operations.
* **Root Selection Heuristic:** Dynamically elects the vector closest to the global average distance (average distance d) as the root, optimizing tree topology for bounded range searches.

### 2. Secondary Index: Red-Black Tree (Norm Pre-filtering)
* **Key:** Euclidean norm (`||v||_2`) of the vector.
* **Invariant Maintenance:** Preserves color and black-height properties through node recoloring and rotations upon insertion/deletion.
* **Candidate Pruning:** Executes `lowerBound` and `upperBound` lookups to isolate a norm window `[||q|| - D, ||q|| + D]`. This reduces high-dimensional metric computations from `O(n * d)` down to `O(m * d + m log k)`, where `m << n`.
  
## Features
### 1. Distance Metrics:
Cosine Similarity: Angle-based similarity for normalized embeddings.
Euclidean Distance (L2): Straight-line geometric distance.
Manhattan Distance (L1): Grid-based coordinate differences.
### 2. Query Types:
topKNearest: Norm-pruned candidate extraction followed by top-k ranking.
findNearest: 1-NN retrieval under selected metric.
rangeQueryFromRoot & rangeQuery: Radius-bounded search.
boundingBoxQuery: Orthogonal spatial query across min/max bounds.
### 3. Resource Management: Strict RAII and manual pointer cleanup across dynamic rebalancing, reconstruction, and deallocations.

## Project Structure

```text
├── main.cpp            # Driver code, test scenarios, and benchmarking
├── main.h              # Common definitions, headers, and type aliases
├── VectorStore.h       # Class declarations (AVLNode, AVLTree, RBTNode, RedBlackTree, VectorStore)
├── VectorStore.cpp     # Implementation of tree structures and vector store algorithms
└── README.md           # Documentation
```

## Build & Run
Prerequisites: GCC/G++ supporting C++17 (g++ >= 9.0) or Clang (clang++ >= 10.0)

Compile
g++ -o main main.cpp VectorStore.cpp -I . -std=c++17 -Wall -Wextra -O2

Execute
./main
