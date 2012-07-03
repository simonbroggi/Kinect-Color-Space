#include "DynamicGridMesh.h"
/*
DynamicGridMesh::DynamicGridMesh(){
    DynamicGridMesh(640, 480);
}
*/
DynamicGridMesh::DynamicGridMesh(){

    w = 640;
    h = 480;

    nVerts = w*h;

    v = new ofVec3f[nVerts];//allocate the large arrays on heap to avoid stack overflow, need to be deleted later
    n = new ofVec3f[nVerts];

    int nQuads = (w-1)*(h-1);
    nIndices = nQuads * 3 * 2;//2 triangles times 3 vertices

    //ofIndexType Faces[nIndices];
    ofIndexType* Faces = new ofIndexType[nIndices];

    //ofFloatColor c[nVerts];
    c = new ofFloatColor[nVerts];

    scalePlane = 4.0f;

    int x, y;
    int facei = 0;
    for(y=0; y<h; y++){
        for(x=0; x<w; x++){
            int vert_i = y*w +x;

            v[vert_i].x = scalePlane * x - scalePlane*w/2;
            v[vert_i].y = scalePlane * y - scalePlane*h/2;
            v[vert_i].z = 0.0f;

            n[vert_i] = ofVec3f(0.0f, 0.0f, -1.0f);

            if(y<25 && x<25){ //white square shows that 0/0 is top left
                c[vert_i].r = 0.0f;
                c[vert_i].g = 0.0f;
                c[vert_i].b = 0.0f;
            }
            else{
                c[vert_i].r = ofRandom(1.0);
                c[vert_i].g = ofRandom(1.0);
                c[vert_i].b = ofRandom(1.0);
            }

            if(x<w-1 && y<h-1){
                int right_i = y*w+x+1;
                int low_i = (y+1)*w+x;
                int lowRight_i = (y+1)*w+x+1;

                int rightI = (x+1)*w + y;
                int lowI = x*w + (y+1);
                int lowRightI = (x+1)*w + (y+1);

                Faces[facei]=low_i;
                facei++;
                Faces[facei]=vert_i;
                facei++;
                Faces[facei]=lowRight_i;
                facei++;
                Faces[facei]=lowRight_i;
                facei++;
                Faces[facei]=vert_i;
                facei++;
                Faces[facei]=right_i;
                facei++;
            }
        }
    }

    vbo.setIndexData( &Faces[0], nIndices, GL_STATIC_DRAW );

    vbo.setVertexData( &v[0], nVerts, GL_DYNAMIC_DRAW );
    vbo.setColorData( &c[0], nVerts, GL_DYNAMIC_DRAW );
    vbo.setNormalData( &n[0], nVerts, GL_DYNAMIC_DRAW );

    //delete large arrays allocated on heap
    //delete[] v;
    //delete[] n;
    delete[] Faces;
    //delete[] c;

}

DynamicGridMesh::~DynamicGridMesh(){
    delete[] v;
    delete[] n;
    delete[] c;
}

void DynamicGridMesh::setZ(int x, int y, float newZ){
    int vert_i = y*w +x;
    v[vert_i].z = newZ;
}
void DynamicGridMesh::setVertAndCol(int x, int y, ofVec3f worldCoord, ofColor color){
    int vert_i = y*w +x;
    v[vert_i] = worldCoord;

    ofFloatColor floatColor(color);
    c[vert_i] = floatColor;
}
void DynamicGridMesh::reinit(){
    for(int y=0; y<h; y++){
        for(int x=0; x<w; x++){
            clearVert(x, y);

        }
    }
}
void DynamicGridMesh::clearVert(int x, int y){
    int vert_i = y*w +x;
    float f = 10;

    //v[vert_i].x = 0;
    //v[vert_i].y = 0;
    //v[vert_i].z = 100000;//set z way back....

    v[vert_i].x = scalePlane * x - scalePlane*w/2;
    v[vert_i].y = scalePlane * y - scalePlane*h/2;
    v[vert_i].z = ofRandom(200.0);

    // = ofVec3f(x*100, y*100, 1000);
    c[vert_i].r = ofRandom(1.0);
    c[vert_i].g = ofRandom(1.0);
    c[vert_i].b = ofRandom(1.0);
}
void DynamicGridMesh::doneSetting(){
    vbo.updateVertexData( &v[0], nVerts);
    vbo.updateColorData( &c[0], nVerts);

    //calculate normals (ignore a boarder of 1)
    //todo: optimize performance! do it on gpu
    /*
    int x, y;
    for(y=1; y<h-1; y++){
        for(x=1; x<w-1; x++){
            int vert_i = y*w +x;
            int vertUp_i = (y-1)*w +x;
            int vertDown_i = (y+1)*w +x;
            int vertLeft_i = y*w +x-1;
            int vertRight_i = y*w +x+1;

            ofVec3f temp1 = v[vertUp_i] - v[vert_i];
            ofVec3f temp2 = v[vertLeft_i] - v[vert_i];
            n[vert_i] = temp1.getCrossed(temp2);

            temp1 = v[vertUp_i] - v[vert_i];
            temp2 = v[vertRight_i] - v[vert_i];
            n[vert_i] -= temp1.getCrossed(temp2);

            temp1 = v[vertDown_i] - v[vert_i];
            temp2 = v[vertLeft_i] - v[vert_i];
            n[vert_i] -= temp1.getCrossed(temp2);

            temp1 = v[vertDown_i] - v[vert_i];
            temp2 = v[vertRight_i] - v[vert_i];
            n[vert_i] += temp1.getCrossed(temp2);
        }
    }
    vbo.updateNormalData( &n[0], nVerts);
    */
}
void DynamicGridMesh::draw(){

	ofPushMatrix();
	glEnable(GL_DEPTH_TEST);

	ofScale(1, -1, -1); //the projected points are 'upside down' and 'backwards'
    ofTranslate(0, 0, -1000); // center the points a bit

    //ofRotate(ofGetElapsedTimef() * 50.0, 1, 1, 0); //rotation over time

    /* //light to check the normals
    ofVec3f lightPosition = ofVec3f(0.0f, 0.0f, 0.0f);
    ofEnableLighting();
    ofSphere(lightPosition, 100.0);
    light.setPosition(lightPosition);
    light.setSpecularColor(ofFloatColor(1.0f, 0.0f, 0.0f));
    ofEnableSeparateSpecularLight();
    light.setDiffuseColor(ofFloatColor(0.005f, 0.005f, 0.005f));
    //light.setPointLight();
    light.enable();
    */

    // draw point clowd
    //glPointSize(1);
    //vbo.drawElements( GL_POINTS, nIndices);

    // draw lines
    //vbo.drawElements( GL_LINES, nIndices);

    // draw geometry

    vbo.drawElements( GL_TRIANGLES, nIndices);

    glDisable(GL_DEPTH_TEST);
    ofPopMatrix();
}

