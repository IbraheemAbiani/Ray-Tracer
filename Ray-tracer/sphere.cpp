// Student Name: Ibraheem Abiani
// Student ID: 862189517
#include "sphere.h"
#include "ray.h"
#include <cmath>

Sphere::Sphere(const Parse* parse, std::istream& in)
{
    in>>name>>center>>radius;
}

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    //TODO;
    double t1;
    double t2;
    vec3 w = ray.endpoint - center;
    vec3 u = ray.direction;
    t1 = (-1 * dot(w, u)) + sqrt(pow(dot(w, u), 2) - (dot(w, w) - pow(radius, 2)));
    t2 = (-1 * dot(w, u)) - sqrt(pow(dot(w, u), 2) - (dot(w, w) - pow(radius, 2)));
    //Pixel_Print("t1: ", t1, " t2: ", t2);
    if(t1 >= small_t && t2 >= small_t) {
        if(t1 < t2) {
            return {t1, -1, {}};
        }
        else {
            return {t2, -1, {}};
        }
    }
    if(t1 >= small_t) {
        return {t1, -1, {}};
    }
    if(t2 >= small_t) {
        return {t2, -1, {}};
    } 
    return {-1, -1, {}};
}

vec3 Sphere::Normal(const Ray& ray, const Hit& hit) const
{
    vec3 normal;
    vec3 x;
    //TODO; // compute the normal direction
    x = ray.Point(hit.dist);
    normal = (x - center).normalized();
    return normal;
}

std::pair<Box,bool> Sphere::Bounding_Box(int part) const
{
    return {{center-radius,center+radius},false};
}
