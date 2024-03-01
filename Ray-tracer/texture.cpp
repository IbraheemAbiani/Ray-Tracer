// Student Name: Ibraheem Abiani
// Student ID: 862189517
#include "parse.h"
#include "texture.h"
#include "dump_png.h"

Texture::Texture(const Parse* parse,std::istream& in)
{
    std::string filename;
    in>>name>>filename>>use_bilinear_interpolation;
    Read_png(data,width,height,filename.c_str());
}

Texture::~Texture()
{
    delete [] data;
}

// uses nearest neighbor interpolation to determine color at texture coordinates
// (uv[0],uv[1]).  To match the reference image, details on how this mapping is
// done will matter.

// 1. Assume that width=5 and height=4.  Texture coordinates with 0<=u<0.2 and
//    0<=v<0.25 should map to pixel index i=0, j=0.  Texture coordinates with
//    0.8<=u<1 and 0.75<=v<1 should map to pixel index i=4, j=3.

// 2. Texture coordinates should "wrap around."  Some of the objects contain
//    (u,v) coordinates that are less than 0 or greater than 1.  u=0.4 and u=1.4
//    and u=-0.6 should be considered equivalent.  There is a wrap function in
//    misc.h that you may use.

// 3. Be very careful about indexing out of bounds.  For example, u=0.999999
//    should result in i=width-1, not i=width.  The latter is out of bounds.

// 4. Be careful with your rounding.  For example, u=-0.0001 should map to
//    i=width-1 in accordance to the wrapping rule.  Remember that casting from
//    float to integer rounds towards zero, so that (int)u would produce 0.  You
//    may find the cmath functions floor, ceil, and rint to be helpful, as they
//    provide precise control over rounding.

// 5. Although there is a flag called use_bilinear_interpolation, none of the
//    test cases exercise this feature.  You do not need to implement it, though
//    of course you are welcome to do so if you like.  You may assume nearest
//    neighbor interpolation.

vec3 Texture::Get_Color(const vec2& uv) const
{
    //TODO;
    //uv = wrap();
    int textCoordi = floor(uv[0] * width);
    int textCoordj = floor(uv[1] * height);
    textCoordi = wrap(textCoordi, width); //wrap
    textCoordj = wrap(textCoordj, height); //wrap
    vec3 color;
    color = From_Pixel(data[textCoordj * width +  textCoordi]);
    //Pixel_Print("uv with wrap: ", wrap(textCoordi, width), " ", wrap(textCoordj, height));
    //Debug_Scope scope;
    Pixel_Print("texture (u,v): ", uv, " (i,j): " , textCoordi, " ", textCoordj, "  color: ", color);
    return color;
}
