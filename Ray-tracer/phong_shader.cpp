// Student Name: Ibraheem Abiani
// Student ID: 862189517
#include "light.h"
#include "parse.h"
#include "object.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include <algorithm> //added
#include <math.h> //added

Phong_Shader::Phong_Shader(const Parse* parse,std::istream& in)
{
    in>>name;
    color_ambient=parse->Get_Color(in);
    color_diffuse=parse->Get_Color(in);
    color_specular=parse->Get_Color(in);
    in>>specular_power;
}

vec3 Phong_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    vec3 color;
    //for ambient light
    vec3 p_ambient;
    if(render_world.ambient_color != nullptr) {
        vec3 ra = render_world.ambient_color->Get_Color(hit.uv);
        vec3 la = color_ambient->Get_Color(hit.uv);
        double intensity = render_world.ambient_intensity;
        //color[0] = color[0] + (ra[0] * la[0] * intensity);
        p_ambient = ra * la * intensity;
        color = color + (ra * la * intensity);
    }
    Debug_Scope scope;
    Pixel_Print("ambient: ",  p_ambient);

    double x = 0;
    double y = -1;
    for(long unsigned int i = 0; i < render_world.lights.size(); i++) {
        bool shadow_exist = false;
        vec3 ldir = (render_world.lights[i]->position - intersection_point).normalized();
        if(render_world.enable_shadows) {
            double t_light = (render_world.lights[i]->position - intersection_point).magnitude(); //distance of light ray
            Ray shadow_ray(intersection_point, ldir);
            std::pair<Shaded_Object,Hit> lightHit (render_world.Closest_Intersection(shadow_ray));
            if(lightHit.second.dist < t_light && lightHit.second.dist != -1) {
                Pixel_Print("light ", render_world.lights[i]->name, " not visible; obscured by object ",  lightHit.first.object->name, " at location ", shadow_ray.Point(lightHit.second.dist));
                shadow_exist = true;
            }
            else{
                Pixel_Print("light ", render_world.lights[i]->name, " visible; closest object on ray too far away (light dist: ", t_light, "; object dist: ", lightHit.second.dist, ")");
            }
        }
        if(!shadow_exist) {
            vec3 vtol = intersection_point - render_world.lights[i]->position;
            vec3 v = y * ray.direction;
            vec3 r = ((2 * dot(normal, ldir)) * normal) - ldir;
            vec3 rd = color_diffuse->Get_Color(hit.uv);
            vec3 rs = color_specular->Get_Color(hit.uv);
            vec3 ld = render_world.lights[i]->Emitted_Light(vtol);
            //TODO; //determine the color
            //color[0] = color[0] + (rd[0] * ld[0] * std::max(dot(ldir, normal), x)) + (rs[0] * ld[0] * std::pow(std::max(dot(v, r), x), specular_power));
            color = color + (rd * ld * std::max(dot(ldir, normal), x)) + (rs * ld * std::pow(std::max(dot(v, r), x), specular_power));
            Pixel_Print("shading for light ", render_world.lights[i]->name, ": diffuse: ", (rd * ld * std::max(dot(ldir, normal), x)),
            "; specular: ", (rs * ld * std::pow(std::max(dot(v, r), x), specular_power)));
        }
    }
    Pixel_Print("final color ", color);
    return color;
}
