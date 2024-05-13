#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <thread>
#include <chrono>

const double PI = 3.14159265358979323846;
const double focal_length = 80;

template <typename T>
bool is_belongs(const std::vector<T> &vec, const T &val) {
    for (const auto &el : vec) {
        if (el == val) {
            return true;
        }
    }

    return false;
}

class Point2D {
private:
    double x;
    double y;

public:
    Point2D() {
        this->x = 0;
        this->y = 0;
    }

    Point2D(double x, double y) {
        this->x = x;
        this->y = y;
    }

    double getX() const { return this->x; }
    double getY() const { return this->y; }

    friend bool operator== (const Point2D &lhs, const Point2D &rhs) {
        return ( (lhs.x == rhs.x) && (lhs.y == rhs.y) );
    }
};

class Point3D {
private:
    double x;
    double y;
    double z;

public:
    Point3D() {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }

    Point3D(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double getX() const { return this->x; }
    double getY() const { return this->y; }
    double getZ() const { return this->z; }
};

class Matrix {
private:
    std::vector<std::vector<double>> matrix;

public:
    Matrix() {
        this->matrix = std::vector<std::vector<double>>();
    }

    Matrix(std::vector<std::vector<double>> matrix) {
        this->matrix = matrix;
    }

    std::vector<std::vector<double>> getMatrix() const { return this->matrix; }

    double get(int i, int j) const { return this->matrix[i][j]; }

    Matrix operator*(const Matrix &rhs) {
        std::vector<std::vector<double>> result;
        for (int i = 0; i < this->matrix.size(); i++) {
            std::vector<double> row;
            for (int j = 0; j < rhs.matrix[0].size(); j++) {
                double sum = 0;
                for (int k = 0; k < this->matrix[0].size(); k++) {
                    sum += this->matrix[i][k] * rhs.matrix[k][j];
                }
                row.push_back(sum);
            }
            result.push_back(row);
        }

        return Matrix(result);
    }

    Matrix operator*(const std::vector<double> vec) {
        // Check if the vector size is equal to the number of columns in the matrix
        if (vec.size() != this->matrix[0].size()) {
            std::cerr << "Vector size must be equal to the number of columns in the matrix!" << std::endl;
            return Matrix();
        }

        std::vector<std::vector<double>> result;
        for (int i = 0; i < this->matrix.size(); i++) {
            std::vector<double> row;
            for (int j = 0; j < vec.size(); j++) {
                row.push_back(this->matrix[i][j] * vec[j]);
            }
            result.push_back(row);
        }

        return Matrix(result);
    }
};

class Console {
private:
    HANDLE handle;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    int width;
    int height;
public:
    Console() {
        this->handle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (!GetConsoleScreenBufferInfo(this->handle, &(this->csbiInfo))) {
            std::cerr << "Failed to get console buffer info!" << std::endl;
            exit(1); // todo: throw error
        }

        this->width = csbiInfo.dwSize.X;
        this->height = csbiInfo.dwSize.Y;
    }

    int getWidth() { return this->width; }
    int getHeight() { return this->height; }

    void draw_points(const std::vector<Point2D> &points) {
        std::string to_draw;
        for (int y = 0; y < this->height; y++) {
            for (int x = 0; x < this->width; x++) {
                to_draw += " ";
            }

            if (y < this->height - 1) {
                to_draw += "\n";
            }
        }

        for (const Point2D &point : points) {
            int x = point.getX();
            int y = point.getY();

            if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
                to_draw[y * this->width + x] = '0';
            }
        }

        std::cout << to_draw;
    }

    void clear() {
        COORD topLeft = {0, 0};
        DWORD length = this->width * this->height;
        DWORD written;
        FillConsoleOutputCharacter(this->handle, TEXT(' '), length, topLeft, &written);
        SetConsoleCursorPosition(this->handle, topLeft);
    }

    std::vector<Point2D> project(const std::vector<Point3D> &points) {
        std::vector<Point2D> result;

        for(const Point3D &point : points) {
            int x = this->width / 2 + point.getX() * (focal_length / point.getZ());
            int y = this->height / 2 + point.getY() * (focal_length / point.getZ());
            result.emplace_back(x, y);
        }

        return result;
    }
};

class Torus {
private:
    Point3D center;
    double R;
    double r;
    std::vector<Point3D> points;

public:
    Torus() {
        this->R = 0.0;
        this->r = 0.0;
    }

    Torus(Point3D center, double R, double r) {
        this->center = center;
        this->R = R;
        this->r = r;

        for(double i = 0; i < 2 * PI; i += PI / 100) {
            for(double j = 0; j < 2 * PI; j += PI / 100) {
                double x = ((R + r * std::cos(j)) * std::cos(i)) + center.getX();
                double y = ((R + r * std::cos(j)) * std::sin(i)) + center.getY();
                double z = (r * std::sin(j)) + center.getZ();
                this->points.emplace_back(x, y, z);
            }
        }
    }

    std::vector<Point3D> getPoints() const { return this->points; }

    void rotate(double angleX, double angleY, double angleZ) {
        std::vector<std::vector<double>> rotationX = {
            {1, 0, 0},
            {0, std::cos(angleX), -std::sin(angleX)},
            {0, std::sin(angleX), std::cos(angleX)}
        };

        std::vector<std::vector<double>> rotationY = {
            {std::cos(angleY), 0, std::sin(angleY)},
            {0, 1, 0},
            {-std::sin(angleY), 0, std::cos(angleY)}
        };

        std::vector<std::vector<double>> rotationZ = {
            {std::cos(angleZ), -std::sin(angleZ), 0},
            {std::sin(angleZ), std::cos(angleZ), 0},
            {0, 0, 1}
        };

        // Rotation matrices
        Matrix rotationXMatrix(rotationX);
        Matrix rotationYMatrix(rotationY);
        Matrix rotationZMatrix(rotationZ);


        // Apply rotations around the center of the torus
        for (int i = 0; i < this->points.size(); i++) {
            Point3D point = this->points[i];
            Matrix pointMatrix({{point.getX() - this->center.getX(), 
                                point.getY() - this->center.getY(), 
                                point.getZ() - this->center.getZ()}});

            Matrix rotatedPointMatrix = pointMatrix * rotationXMatrix * rotationYMatrix * rotationZMatrix;

            this->points[i] = Point3D(rotatedPointMatrix.get(0, 0) + this->center.getX(), 
                                        rotatedPointMatrix.get(0, 1) + this->center.getY(), 
                                        rotatedPointMatrix.get(0, 2) + this->center.getZ());
        }
    }
};


int main() {
    using namespace std::chrono;
    const int fps = 60;  // Desired FPS
    milliseconds msPerFrame(1000 / fps);

    steady_clock::time_point lastFrameTime = steady_clock::now();

    Console myConsole;
    Point3D torus_center(0, 0, -300);
    Torus myTorus(torus_center, 30, 10);
    std::vector<Point2D> points = myConsole.project(myTorus.getPoints());
    
    myConsole.clear(); 
    while (true) {

        myTorus.rotate(0.1, 0.1, 0.1);
        points = myConsole.project(myTorus.getPoints());

        steady_clock::time_point start = steady_clock::now();
        myConsole.clear();
        myConsole.draw_points(points);

        steady_clock::time_point end = steady_clock::now();

        auto duration = duration_cast<milliseconds>(end - start);
        auto sleepTime = msPerFrame - duration;

        if (sleepTime.count() > 0) {
            std::this_thread::sleep_for(sleepTime);
        } else {
            // Frame took longer than our frame time
            std::cerr << "Warning: Frame processing took too long\n";
        }

        lastFrameTime = steady_clock::now();
    }

    return 0;
}