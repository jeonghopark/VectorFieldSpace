#pragma once

#include "ofMain.h"


struct Cell {
    ofVec3f pos;
    ofVec3f dir;
    float size;
};


struct Particle {
    ofVec3f pos;
    ofMesh path;
};



class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    ofEasyCam cam;
    
    int numCellOneSide;
    int numCellSidePixels;
    int cellDiagonal;
    int sideStepPixel;
    float speedAnimation;
    vector<ofVec3f> elementVector;
    
    vector<ofVec3f> elementPos;
    void setElementPos(vector<ofVec3f> & _ePos);
    vector<Cell> cell;
    void setUpdateCell();
    glm::vec3 getNoiseCellField(const glm::vec3& _pos);
    
    void drawNet();
    
    void drawVector();
    float complexity;

    bool debugMode;
    bool viewNet;
    bool viewParticle;
    
    void setParticles(int _num);
    vector<Particle> particles;
    ofVec3f randomFactor;
    void updateParticles();
    void removeParticle();
    
    void drawParticles();
    void drawParticlePath();
    
    ofVec3f rotateFactor;
    
};
