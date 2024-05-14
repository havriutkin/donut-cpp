# Donut in C++

This project implements a rotating 3D torus (donut) displayed in the console using C++ and Windows API.

## Table of Contents
- [Installation](#installation)
- [Mathematical Explanation](#mathematical-explanation)

## Installation

### Prerequisites

- Windows operating system
- C++ compiler (e.g., MSVC, MinGW)

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
3. Run compiled file in a teminal window:
    ```bash
    ./donut.exe
    ```

## Mathematical Explanation

The 3D torus (donut) visualization involves several mathematical concepts:

### 1. Torus Parametric Equations

A torus can be described parametrically with two angles, \( \theta \) and \( \phi \), representing the position on the torus:

$$ 
\begin{align*}
x &= (R + r \cos(\phi)) \cos(\theta) \\
y &= (R + r \cos(\phi)) \sin(\theta) \\
z &= r \sin(\phi)
\end{align*}
$$

Here:
-  $R$ is the major radius (distance from the center of the tube to the center of the torus).
-  $r$ is the minor radius (radius of the tube).

### 2. Rotation Matrices

To rotate the torus, we use rotation matrices for the X, Y, and Z axes. These matrices allow us to rotate points in 3D space.

#### Rotation around X-axis:
$$
\begin{bmatrix}
1 & 0 & 0 \\
0 & \cos(\alpha) & -\sin(\alpha) \\
0 & \sin(\alpha) & \cos(\alpha) 
\end{bmatrix}
$$

#### Rotation around Y-axis:
$$
\begin{bmatrix}
\cos(\beta) & 0 & \sin(\beta) \\
0 & 1 & 0 \\
-\sin(\beta) & 0 & \cos(\beta)
\end{bmatrix}
$$

#### Rotation around Z-axis:
$$
\begin{bmatrix}
\cos(\gamma) & -\sin(\gamma) & 0 \\
\sin(\gamma) & \cos(\gamma) & 0 \\
0 & 0 & 1 
\end{bmatrix}
$$

### 3. Projection onto 2D Plane

To display the 3D torus on a 2D console, we project 3D points onto a 2D plane using the perspective projection formula:

$$
\begin{align*}
x_{\text{2D}} &= \frac{f \cdot x}{z} \\
y_{\text{2D}} &= \frac{f \cdot y}{z}
\end{align*}
$$

Here, $f$ is the focal length, representing the distance from the viewer to the projection plane. This projection simulates depth by scaling the $x$ and $y$ coordinates based on their distance $z$ from the viewer.

### 4. Frame Rate Control
The torus rotation and drawing are controlled to achieve a consistent frame rate. This involves calculating the time taken to process each frame and adjusting the sleep time to maintain a stable frame rate, typically 60 frames per second.

By combining these mathematical concepts, we achieve a rotating 3D torus that is projected and visualized in the console.

## Contacts
Checkout my other projects at my portfolio

