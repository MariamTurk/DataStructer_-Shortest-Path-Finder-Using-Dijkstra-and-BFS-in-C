# 🗺️ Shortest Path Finder Using Dijkstra and BFS in C

### 👤 Author: Mariam Turk  
 



## 📚 Overview

This C program implements a **shortest path finding system** between cities using:
- **Dijkstra’s Algorithm** (based on weighted graph)
- **Breadth-First Search (BFS)** (based on unweighted traversal)

It reads a list of cities and distances from a file, builds a graph, and then allows the user to choose a source and destination to compute the path and total travel distance. The results are printed and optionally saved to a file.



## 🧰 Features

- 📁 Load city-to-city connections from file
- 🏙️ Enter **source** and **destination** cities
- 🧠 Run:
  - ✅ Dijkstra’s Algorithm (Shortest Distance)
  - ✅ Breadth-First Search (Minimum Hops)
- 🖨️ Print the path and total distance
- 💾 Save shortest path results to `shortest_distance.txt`

---

## 📂 File Structure

. ├── main.c # Complete source code ├── cities.txt # Example input file (user-provided) ├── shortest_distance.txt # Output file for results └── README.md # Project documentation


## 📄 Input File Format (e.g. `cities.txt`)

Each line should follow this format:

CityA CityB 10km CityB CityC 5km CityA CityD 20km

✅ Note: All distances must be followed by "km".

🧭 Menu Options
pgsql
Copy
Edit
1. Load cities
2. Enter source city
3. Enter destination city
4. Exit (and write shortest path to file)

📤 Example Output
Dijkstra path from Ramallah to Nablus:
Ramallah -> (15km) Birzeit -> (18km) Nablus     Total distance: 33km

BFS path from Ramallah to Nablus:
Ramallah -> (15km) Birzeit -> (18km) Nablus     Total distance: 33km


📄 Output File: shortest_distance.txt
Contents:
Dijkstra path from Ramallah to Nablus:
Ramallah -> (15km) Birzeit -> (18km) Nablus
Total distance: 33km

🔍 Algorithms Used
Algorithm	Description	Use Case
Dijkstra	Finds shortest weighted path	Accurate travel distance
Breadth-First	Finds shortest path in hops (unweighted)	Simpler path exploration

🛠️ To Do
 Add input validation for malformed city files

 Display all reachable cities from a given source

 Allow undirected/one-way connections toggle
