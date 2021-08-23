#include <math.h>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
//#include <emscripten/emscripten.h>

extern "C" {

    struct Sphere {
        double x, y, z, radius;
        Sphere(double _x, double _y, double _z, double _radius) {
            x = _x;
            y = _y;
            z = _z;
            radius = _radius;
        }
    };

    std::random_device rd;     
    std::mt19937 rng(rd());    
    std::uniform_int_distribution<int> uni(-1000, 1000); 

    double randBetween(int min, int max) {
        return uni(rng);
    }

    bool sphereToSphereCollision(struct Sphere sphere1, struct Sphere sphere2) {
        return (sphere1.radius + sphere2.radius) > hypot(hypot(sphere1.x - sphere2.x, sphere1.y - sphere2.y), sphere1.z - sphere2.z);
    }

    //EMSCRIPTEN_KEEPALIVE
        double runSphereToSphereCollision(int loops) {
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::duration;
        using std::chrono::milliseconds;

        std::vector<Sphere> spheres;
        for (size_t i = 0; i < loops*2; i++)
        {
            spheres.push_back({ randBetween(-1000, 1000), randBetween(-1000, 1000), randBetween(-1000, 1000), randBetween(-1000, 1000) });   
        }

        auto t1 = high_resolution_clock::now();
        double total = 0.0;
        for (size_t i = 0; i < loops; i+=2)
        {
            total += sphereToSphereCollision(spheres[i], spheres[i+1]);
        }
        auto t2 = high_resolution_clock::now();

        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "duration was ";
        std::cout << ms_double.count() << '\n';
        std::cout << "total was ";
        std::cout << total << '\n';
        return ms_double.count();
    }
}

