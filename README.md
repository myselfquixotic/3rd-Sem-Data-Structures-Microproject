# Cyclone Grid Restoration - Decision Support System
**3rd Semester Data Structures Microproject**

This repository contains the C++ implementation of a disaster response decision support system. It uses graph data structures, a Max-Heap Priority Queue, Dijkstra's Shortest Path, and Prim's Minimum Spanning Tree algorithms to route parallel repair crews through a damaged power grid.

## Repository Structure
This repository contains two distinct versions of the project:

1. **`Microproject Basic/`**: The pure, terminal-only algorithmic version. It executes the restoration logic and randomized weather events, outputting the step-by-step routing directly to the console.
2. **`Microproject Advanced with Simulation/`**: The professional-grade Decision Support System. It runs the C++ backend to generate an event-driven JSON log, which is then rendered by a custom HTML5/Canvas web dashboard to visualize the multi-crew dispatch in real-time.

---

## Prerequisites
To compile and run this project, your system must have:
* **GCC Compiler (g++)** supporting C++17.
* **Visual Studio Code** (Recommended).
* **Live Server Extension** (VS Code) - *Required for the Advanced version's web UI to bypass CORS restrictions when fetching the JSON log.*

---

## How to Run: Version 1 (Basic Terminal Version)

1. Open your terminal and navigate to the basic folder:
   ```bash
   cd "Microproject Basic"
   ```
2. Compile the C++ code using the following command:
   ```bash
   g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Grid\Grid.cpp Priority\Priority.cpp RestorationEngine\RestorationEngine.cpp DynamicUpdate\DynamicUpdate.cpp -o main.exe
   ```
3. Execute the simulation:
   ```bash
   ./main.exe
   ```

---

## How to Run: Version 2 (Advanced with Web Simulation)

### Step A: Generate the Simulation Log
1. Open your terminal and navigate to the advanced folder:
   ```bash
   cd "Microproject Advanced with Simulation"
   ```
2. Compile the C++ backend:
   ```bash
   g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Grid\Grid.cpp Priority\Priority.cpp RestorationEngine\RestorationEngine.cpp DynamicUpdate\DynamicUpdate.cpp -o main.exe
   ```
3. Run the executable to simulate the disaster and generate the `disaster_log.json` file:
   ```bash
   ./main.exe
   ```

### Step B: Launch the Web Visualizer**

Because the JavaScript uses the fetch() API to load the JSON log, you cannot simply double-click the HTML file (browsers block local file fetching for security).

1. Open the Microproject Advanced with Simulation folder inside VS Code.
2. (In VS Code)You can click the "Go Live" button at the bottom right corner to open the dashboard or
3. Right-click on index.html in the file explorer.
4. Select "Open with Live Server".
5. The dashboard will open in your default web browser. Use the media controls at the bottom to play, pause, or scrub through the restoration timeline.

---

## Module Architecture (High Cohesion / Low Coupling)

- PowerGrid: Manages the physical map using an Adjacency List.

- PriorityQueue: Ranks unpowered nodes (Level 10-100) using a custom Max-Heap.

- RestorationEngine: Handles algorithmic logic. Uses Dijkstra's for critical infrastructure (Priority >= 50) and Prim's MST for mass-connection of residential areas (Priority < 50).

- DynamicUpdate: Simulates randomized secondary weather failures, dynamically altering edge weights and forcing the priority queue to rebuild.