#ifndef DYNAMICGRIDMESH_H
#define DYNAMICGRIDMESH_H
#include "ofMain.h"

class DynamicGridMesh
{
    public:
        /** Default constructor */
        DynamicGridMesh();
        ~DynamicGridMesh();
        //DynamicGridMesh(int x, int y);

        void setZ(int x, int y, float newZ);
        void setVertAndCol(int x, int y, ofVec3f worldCoord, ofColor color);
        void clearVert(int x, int y);
        void doneSetting();
        void draw();
        void reinit();
        float scalePlane; // just for fun! a plane centered at the cameras origin where points are set to when they-re to far or to close from the cam

    protected:
    private:
        int w, h;
        int nVerts;
        ofVbo vbo;
        int nIndices;
        ofVec3f *v, *n;
        ofFloatColor* c;
        ofLight	light; //realy dosn't belong here, just to test normals

};

#endif // DYNAMICGRIDMESH_H
