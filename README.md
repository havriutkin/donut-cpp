# Donut in C++

This project implements a rotating 3D torus (donut) displayed in the console using C++ and Windows API.

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
- [Contributing](#contributing)
- [License](#license)

## Installation

### Prerequisites

- Windows operating system
- C++ compiler (e.g., MSVC, MinGW)
- CMake (optional, for building)

### Build Instructions

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/donut-cpp.git
    cd donut-cpp
    ```

2. Compile the code:
    ```bash
    g++ -o donut main.cpp -lgdi32
    ```

   Alternatively, if using CMake:
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

## Usage

Run the executable:
```bash
./donut
