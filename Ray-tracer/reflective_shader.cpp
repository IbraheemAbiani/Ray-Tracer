// Student Name: Ibraheem Abiani
// Student ID: 862189517
#include "reflective_shader.h"
#include "parse.h"
#include "ray.h"
#include "render_world.h"

Reflective_Shader::Reflective_Shader(const Parse* parse,std::istream& in)
{
    in>>name;
    shader=parse->Get_Shader(in);
    in>>reflectivity;
    reflectivity=std::max(0.0,std::min(1.0,reflectivity));
}

vec3 Reflective_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    //TODO; // determine the color
    vec3 color;
    vec3 rdir = (ray.endpoint - intersection_point).normalized(); //view ray direction 
    vec3 r = ((2 * dot(normal, rdir)) * normal) - rdir; //reflected ray direction
    Ray reflect_ray(intersection_point, r); //reflected ray
    vec3 co = shader->Shade_Surface(render_world, ray, hit, intersection_point, normal, recursion_depth); //color of object
    vec3 cr = render_world.Cast_Ray(reflect_ray, recursion_depth + 1); //color of reflectd object
    color = ((1 - reflectivity) * co) + (reflectivity * cr);
    Pixel_Print("reflected ray: (end: ", intersection_point, "; dir: ", r, "; reflected color: ", cr, "; object color: ", co, "; final color: ", color);
    return color;
}
