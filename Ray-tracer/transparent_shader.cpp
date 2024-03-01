// Student Name: Ibraheem Abiani
// Student ID: 862189517
#include "transparent_shader.h"
#include "parse.h"
#include "ray.h"
#include "render_world.h"
#include <cmath> //added

Transparent_Shader::
Transparent_Shader(const Parse* parse,std::istream& in)
{
    in>>name>>index_of_refraction>>opacity;
    shader=parse->Get_Shader(in);
    assert(index_of_refraction>=1.0);
}

// Use opacity to determine the contribution of this->shader and the Schlick
// approximation to compute the reflectivity.  This routine shades transparent
// objects such as glass.  Note that the incoming and outgoing indices of
// refraction depend on whether the ray is entering the object or leaving it.
// You may assume that the object is surrounded by air with index of refraction
// 1.
vec3 Transparent_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    //TODO;
    if(dot(ray.direction, normal) < 0) { //entering
        Pixel_Print("Ray entering");
        double n1;
        double n2;
        n1 = 1;
        n2 = index_of_refraction;
        vec3 v = (ray.endpoint - intersection_point).normalized(); //view ray direction
        vec3 r = ((2 * dot(normal, v)) * normal) - v; //reflected ray direction
        Ray reflect_ray(intersection_point, r); //reflected ray
        vec3 cr = render_world.Cast_Ray(reflect_ray, recursion_depth + 1); //color of object reflection
        Pixel_Print("reflected ray: (end: ", intersection_point, "; dir: ", r, "; reflected color: ", cr, ";");
        double cos1 = dot(normal, v);
        double sin1 = std::sqrt(1 - std::pow(cos1, 2));
        double sin2 = (n1/n2) * sin1; 
        double cos2 = std::sqrt(1 - std::pow(sin2, 2));
        vec3 b = (v - (cos1 * normal)) / sin1; //perpindecular to normal
        vec3 t = (-1 * cos2 * normal) - (sin2 * b); //transmitted vector direction
        vec3 ct;
        if(cos2 <= 1) { //to avoid total internal reflection
            Ray trans_ray(intersection_point, t); //transmission ray
            ct = render_world.Cast_Ray(trans_ray, recursion_depth + 1); //color of object's tranparency
        }
        double k = 1 - opacity; 
        double r0 = std::pow((n1 - n2) / (n1 + n2), 2);
        double rcof = r0 + ((1 - r0) * pow((1 - cos1), 5)); //specific to test case 3
        double beta = k * rcof;
        double gamma = k * (1 - rcof);
        vec3 com_color = (rcof * cr) + ((1 - rcof) * ct);
        Pixel_Print("transmitted ray: (end: ", intersection_point, "; dir: ", t, "); transmitted color: ", ct, "; Schlick reflectivity: ", rcof, "; combined color: ", com_color);
        vec3 co = shader->Shade_Surface(render_world, ray, hit, intersection_point, normal, recursion_depth); //color of object
        vec3 color = co + (beta * (cr - co)) + (gamma * (ct - co)); //calculate final color
        Pixel_Print("object color: ", co, "; final color: ", color);
        return color;
    }
    if(dot(ray.direction, normal) > 0){ //exiting
        Pixel_Print("Ray exiting");
        double n1;
        double n2;
        n1 = index_of_refraction;
        n2 = 1;
        //vec3 v = (ray.endpoint - intersection_point).normalized(); //view ray direction
        vec3 v = ray.direction; //changed to ray direction
        vec3 r = ((2 * dot(normal, v)) * normal) - v; //reflected ray direction opposite
        r = -1.0 * r; //adjusted for exit
        Ray reflect_ray(intersection_point, r); //reflected ray
        vec3 cr = render_world.Cast_Ray(reflect_ray, recursion_depth + 1); //color of object reflection
        Pixel_Print("reflected ray: (end: ", intersection_point, "; dir: ", r, "; reflected color: ", cr, ";");
        double cos1 = dot(normal, v);
        double sin1 = std::sqrt(1 - std::pow(cos1, 2));
        double sin2 = (n1/n2) * sin1; 
        double cos2 = std::sqrt(1 - std::pow(sin2, 2));
        vec3 b = (v - (cos1 * normal)) / sin1; //perpindecular to normal
        vec3 t = (-1 * cos2 * normal) - (sin2 * b); //transmitted vector direction
        t = -1.0 * t; //adjusted for exit
        vec3 ct;
        if(cos2 <= 1) { //to avoid total internal reflection
            Ray trans_ray(intersection_point, t); //transmission ray
            ct = render_world.Cast_Ray(trans_ray, recursion_depth + 1); //color of object's tranparency
        }
        else{
            Pixel_Print("complete internal reflection");
            vec3 co = shader->Shade_Surface(render_world, ray, hit, intersection_point, normal, recursion_depth); //color of object
            Pixel_Print("object color: ", co, "; final color: ", cr);
            return cr; //POSSILE FUTURE BUG HERE (not sure)
        }
        double k = 1 - opacity; 
        double r0 = std::pow((n1 - n2) / (n1 + n2), 2);
        double rcof = r0 + ((1 - r0) * pow((1 - dot(normal, t)), 5));
        double beta = k * rcof;
        double gamma = k * (1 - rcof);
        vec3 com_color = (rcof * cr) + ((1 - rcof) * ct);
        Pixel_Print("transmitted ray: (end: ", intersection_point, "; dir: ", t, "); transmitted color: ", ct, "; Schlick reflectivity: ", rcof, "; combined color: ", com_color);
        vec3 co = shader->Shade_Surface(render_world, ray, hit, intersection_point, normal, recursion_depth); //color of object
        vec3 color = co + (beta * (cr - co)) + (gamma * (ct - co)); //calculate final color
        Pixel_Print("object color: ", co, "; final color: ", color);
        return color;
    }
    return {};
}
