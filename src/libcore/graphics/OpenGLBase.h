#ifndef OPENGL_BASE_HEADER
#define OPENGL_BASE_HEADER

#include "../export_symbols.hpp"
#include "JuceHeader.h"

/* This is still needed because we are using old glu utilities such as glbegin.
This may also cause the "gl.h and gl3.h are both included" warning on MacOS because glu.h includes gl.h, whereas juce_opengl.h calls gl3.h if OpenGL 3 is present.
Those includes can (and should?) be avoided if the old glu functions are replaced with openGLcontext.extensions functions
*/
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
//#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define JUCE_OPENGL_ALLOW_NON_POWER_OF_TWO_TEXTURES 1
namespace visr_production_suite
{
namespace core
{
namespace graphics
{

void static inline drawCircle(float xc, float yc, float radius, int num_segments, float scale, juce::Colour c1, float gradient, juce::Colour c2 = juce::Colour())
{
    
    float theta = 2 * juce::float_Pi / float(num_segments);
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;
    
    /************ DRAW BACKGROUND RADIAL GRADIENT ***********************************/
    
    float x = radius;//we start at angle = 0
    float y = 0;
    //    std::cout<<"opengl3 " << JUCE_OPENGL3 <<std::endl;
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(c1.getFloatRed(),c1.getFloatGreen(),c1.getFloatBlue(),c1.getFloatAlpha());
    glVertex2f(xc*scale,yc*scale);
    
    if(gradient)
        glColor4f(c2.getFloatRed(),c2.getFloatGreen(),c2.getFloatBlue(),c2.getFloatAlpha());
    
    for(int ii = 0; ii <= num_segments; ii++)
    {
        glVertex2f((x + xc)*scale, (y + yc)*scale);//output vertex
        
        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}

class SolidSphere
{
protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;
    
public:
    SolidSphere(float radius, unsigned int rings, unsigned int sectors)
    {
        float const R = 1.f/(float)(rings-1);
        float const S = 1.f/(float)(sectors-1);
        int r, s;
        
        vertices.resize(rings * sectors * 3);
        normals.resize(rings * sectors * 3);
        texcoords.resize(rings * sectors * 2);
        std::vector<GLfloat>::iterator v = vertices.begin();
        std::vector<GLfloat>::iterator n = normals.begin();
        std::vector<GLfloat>::iterator t = texcoords.begin();
        for(r = 0; r < (int)rings; r++) for(s = 0; s < (int)sectors; s++) {
            float const y = sinf( -juce::float_Pi/2.f + juce::float_Pi * r * R );
            float const x = cosf(2* juce::float_Pi * s * S) * sinf(juce::float_Pi * r * R );
            float const z = sinf(2* juce::float_Pi * s * S) * sinf(juce::float_Pi * r * R );
            
            *t++ = s*S;
            *t++ = r*R;
            
            *v++ = x * radius;
            *v++ = y * radius;
            *v++ = z * radius;
            
            *n++ = x;
            *n++ = y;
            *n++ = z;
        }
        
        indices.resize(rings * sectors * 4);
        std::vector<GLushort>::iterator i = indices.begin();
        for(r = 0; r < (int)rings-1; r++) for(s = 0; s < (int)sectors-1; s++) {
            *i++ = r * sectors + s;
            *i++ = r * sectors + (s+1);
            *i++ = (r+1) * sectors + (s+1);
            *i++ = (r+1) * sectors + s;
        }
    }
    
    void draw(GLfloat x, GLfloat y, GLfloat z)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(x,y,z);
        //        glTranslatef(x-0.01f,y-0.005f,z);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glNormalPointer(GL_FLOAT, 0, &normals[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
        glPopMatrix();
    }
};

/**
 * Graphical User Interface base component for drawing a sphere with guidelines
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL OpenGLBase : public juce::Component
{
public:
    OpenGLBase();
    ~OpenGLBase();
    
protected:
    void resized() override;
    
    /**
     * Function to calculate top-left corner of the rectangle surrounding the ellipse,
     * given its centre and semi-major and semi-minor axes
     */
    void ellipseCentreToTopLeft(float& x, float& y, float& ra, float& rb);
    
    float mPhi;
    float mTheta;
    float mRadius;
    float x,y,z;
    float rotX;
    float rotY;
    float rotAngle;
    float ssize;
    bool isFirst;
    bool drawFromImage =false;
    
    float hfwidth;
    float hfheight;
    float width;
    float height;
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif //OPENGL_BASE_HEADER
