// Student Name: Ibraheem Abiani
// Student ID: 862189517
#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

#include <iostream>

extern bool enable_acceleration;

Render_World::~Render_World()
{
    for(auto a:all_objects) delete a;
    for(auto a:all_shaders) delete a;
    for(auto a:all_colors) delete a;
    for(auto a:lights) delete a;
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
std::pair<Shaded_Object,Hit> Render_World::Closest_Intersection(const Ray& ray) const
{
    //TODO;
    double t = std::numeric_limits<double>::max();
    Hit minHit;
    Hit curHit;
    minHit.dist = -1;
    unsigned long int objNum = 0;
    unsigned long int i = 0;
    unsigned long int icounter = 0;
    for(auto o:objects) {
        curHit = o.object->Intersection(ray, 0);
        if(curHit.dist >= small_t) {
            Debug_Scope scope;
            Pixel_Print("intersect test with ",  o.object->name, "; hit: (dist: ", curHit.dist, "; triangle: ", curHit.triangle, "; uv: ", curHit.uv, ")");
            if(icounter == 0) {
                minHit.dist = t;
            }
            icounter++;
        }
        else {
            Debug_Scope scope;
            Pixel_Print("no intersection with ", o.object->name);
        }
        if(curHit.dist < minHit.dist && curHit.dist >= small_t) {
            minHit = curHit;
            objNum = i;
        }
        i++;
    }
    if(minHit.dist >= small_t) {
        Debug_Scope scope;
        Pixel_Print("closest intersection; obj: ", objects[objNum].object->name, "; hit: (dist: ", minHit.dist, "; triangle: ", minHit.triangle, "; uv: ", minHit.uv, ")");
    }
    else {
        Debug_Scope scope;
        Pixel_Print("closest intersection; none");
    }
    return {objects[objNum], minHit};
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    //TODO; // set up the initial view ray here
    Ray ray;
    ray.endpoint = camera.position;
    ray.direction = (camera.World_Position(pixel_index) - camera.position).normalized();
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth) const
{
    Debug_Scope scope; 
    Pixel_Print("cast ray (end: ", ray.endpoint, "; dir: ", ray.direction, ")");
    vec3 color;
    if(recursion_depth >= recursion_depth_limit + 1) {
        Pixel_Print("ray too deep; return black");
        color.make_zero();
        return color;
    }
    //TODO; // determine the color here
    std::pair<Shaded_Object,Hit> closeHit (Closest_Intersection(ray));
    if(closeHit.second.Valid()){
        Pixel_Print("call Shade_Surface with location ", ray.Point(closeHit.second.dist), "; normal: ", closeHit.first.object->Normal(ray, closeHit.second));
        color = closeHit.first.shader->Shade_Surface(*this, ray, closeHit.second, ray.Point(closeHit.second.dist), 
        closeHit.first.object->Normal(ray, closeHit.second), recursion_depth);
        //Debug_Scope scope; 
        //Pixel_Print("final color ", color);
    }
    else if(background_shader != nullptr) {
        vec3 temp;
        Pixel_Print("call Shade_Surface with location ", temp, "; normal: ", temp);
        color = background_shader->Shade_Surface(*this, ray, closeHit.second, temp, temp, recursion_depth);
        Debug_Scope scope; 
        Pixel_Print("final color ", color);
    }
    else {
        color.make_zero();
        Debug_Scope scope; 
        Pixel_Print("no background; return black");
    }
    
    return color;
}
