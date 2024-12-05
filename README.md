# Randomized Incremental Convex Hull Visualization

This utility sends a convex hull's points and edges to a Python server for real-time visualization using HTTP.

## Prerequisites

Make sure you have the following installed on your system:

1. **C++ Compiler**: GCC or Clang with C++20 support.
2. **libcurl**: Library for HTTP requests.
3. **JsonCpp**: Library for JSON handling.

### MacOS
```bash
brew install libcurl jsoncpp
```

### Ubuntu/Debian
```bash
sudo apt-get install libcurl4-openssl-dev libjsoncpp-dev
```

---

## Setting Up the Project

1. Clone the repository or copy the code.
2. Save the provided code in a file named `utility.cpp`.

---

## Compilation

Use cmake and clion to compile the code.

---

## Running the Utility

1. **Start the Python Visualization Server**:
    - Run the Flask server on port `8000`.
    - Use the provided `visualize_hull.py` script.

   Example command to start the server:
   ```bash
   python visualize_hull.py
   ```

2. **Run the C++ Utility**:
    - Execute the compiled binary to send points and edges.

---

## How It Works

1. The Python server listens for POST requests on `http://127.0.0.1:8000/update`.
2. The C++ utility sends the graph (points and hull edges) as JSON data to the server.
3. The server updates the visualization in real-time.

---

## Example Output

- The Python server visualizes the convex hull dynamically as new data is sent.
