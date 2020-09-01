#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(10);
    ofEnableDepthTest();

    cam.setNearClip(0.001);
    cam.setFarClip(3000);
    cam.setDistance(800);
        
    numCellOneSide = 12;
    numCellSidePixels = 50;
    complexity = 1.2;
    speedAnimation = 4.0;
        
    int _num = pow(numCellOneSide, 3);
    cell.resize(_num);
    
    setParticles(4000);
    
    randomFactor = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
    setElementPos(elementPos);
    setUpdateCell();
    
}



//--------------------------------------------------------------
void ofApp::setParticles(int _num) {
    
    particles.resize(_num);
    int _halfLength = numCellOneSide * numCellSidePixels * 0.5;
    
    for(int i = 0; i < particles.size(); i++) {
        Particle _p;
        _p.pos = glm::vec3(ofRandom(-_halfLength, _halfLength), ofRandom(-_halfLength, _halfLength), ofRandom(-_halfLength, _halfLength));
        _p.path.setMode(OF_PRIMITIVE_LINE_STRIP);
        particles[i] = _p;
    }

}



//--------------------------------------------------------------
void ofApp::setElementPos(vector<ofVec3f> & _ePos) {
    
    int _x = 0;
    int _y = 0;
    int _z = 0;
    float _f = -numCellSidePixels * numCellOneSide * 0.5 + numCellSidePixels * 0.5;
    ofVec3f _offSet(_f, _f, _f);
    
    for (int i = 0; i < cell.size(); i++) {
        
        if (_x > numCellOneSide) {
            return;
        }
        if (_z >= numCellOneSide) {
            _y += 1;
        }
        if (_y >= numCellOneSide) {
            _x += 1;
        }
        
        _y %= numCellOneSide;
        _z %= numCellOneSide;
        
        ofVec3f _pos(_x, _y, _z);
        _pos *= numCellSidePixels;
        _pos += _offSet;
        
        _ePos.push_back(_pos);
        
        _z += 1;
    }
    
}




//--------------------------------------------------------------
void ofApp::setUpdateCell() {
    
    for (int i = 0; i < cell.size(); i++) {
        
        ofVec3f _pos = elementPos[i];
        ofVec3f _dir(getNoiseCellField(_pos).x, getNoiseCellField(_pos).y, getNoiseCellField(_pos).z);
        _dir *= numCellSidePixels * 0.5;
        
        ofVec3f _o(0, 0, 0);
        float _b = _dir.distance(_o);
                
        Cell _c;
        _c.pos = _pos;
        _c.dir = _dir;
        _c.size = _b;
        cell[i] = _c;
        
    }
    
}



//--------------------------------------------------------------
glm::vec3 ofApp::getNoiseCellField(const glm::vec3& position) {
    
    int _halfLength = numCellOneSide * numCellSidePixels * 0.5;
    float _normX = ofNormalize(position.x, -_halfLength, _halfLength);
    float _normY = ofNormalize(position.y, -_halfLength, _halfLength);
    float _normZ = ofNormalize(position.z, -_halfLength, _halfLength);
    
    float _u = ofNoise(_normX * complexity + PI, _normY * complexity + PI, _normZ * complexity + PI, randomFactor.x);
    float _v = ofNoise(_normX * complexity - PI, _normY * complexity - PI, _normZ * complexity - PI, randomFactor.y);
    float _w = ofNoise(_normX * complexity, _normY * complexity, _normZ * complexity, randomFactor.z);
    
    glm::vec3 _bv(_u, _v, _w);
    _bv *= 2.0;
    _bv += glm::vec3(-1.0, -1.0, -1.0);
    
    return _bv;
    
}





//--------------------------------------------------------------
void ofApp::update(){
    
    setUpdateCell();
    updateParticles();
    removeParticle();
    
}



//--------------------------------------------------------------
void ofApp::updateParticles() {
    
    for(int i = 0; i < particles.size(); i++) {
        
        ofVec3f _v = particles[i].pos;
        glm::vec3 field = getNoiseCellField(_v) * speedAnimation;
        
        _v += field;
        
        float _bound = numCellOneSide * numCellSidePixels * 0.5;
        if(_v.x < -_bound || _v.x > _bound || _v.y < -_bound || _v.y > _bound || _v.z < -_bound || _v.z > _bound) {
            float _x = ofRandom(-_bound, _bound);
            float _y = ofRandom(-_bound, _bound);
            float _z = ofRandom(-_bound, _bound);
            _v = ofVec3f(_x, _y, _z);
            particles[i].path.clear();
        }

        particles[i].pos = _v;
        particles[i].path.addVertex(_v);
        
    }
    
}




//--------------------------------------------------------------
void ofApp::removeParticle() {
    
    for(int i = 0; i < particles.size(); i++) {
        if (particles[i].path.getNumVertices() > 100) {
            particles[i].path.removeVertex(0);
        }
    }
    
}



//--------------------------------------------------------------
void ofApp::draw(){
        
    cam.begin();
            
    drawVector();
    drawNet();
    drawParticlePath();
    drawParticles();
        
    cam.end();
    
}



//--------------------------------------------------------------
void ofApp::drawParticles() {
    
    ofPushStyle();
    ofSetColor(255, 255);
    
    for (int i = 0; i < particles.size(); i++) {
        ofDrawBox(particles[i].pos, 2);
    }
    
    ofPopStyle();
    
}


//--------------------------------------------------------------
void ofApp::drawParticlePath() {
    
    ofPushStyle();
    ofSetColor(255, 150, 120, 70);
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i].path.draw();
    }
    
    ofPopStyle();
    
}




//--------------------------------------------------------------
void ofApp::drawVector() {
    
    ofPushStyle();
    ofSetColor(120, 255, 255, 120);
    
    for (int i = 0; i < cell.size(); i++) {
        ofVec3f _pos = cell[i].pos;
        ofVec3f _dir = cell[i].pos + cell[i].dir;
        ofDrawLine(_pos, _dir);
    }
    
    ofPopStyle();
    
}


//--------------------------------------------------------------
void ofApp::drawNet() {
    
    ofPushStyle();
    ofSetColor(120, 200, 255, 100);
    
    for (int i = 0; i < cell.size(); i++) {
        if (i % numCellOneSide <= numCellOneSide - 2) {
            ofVec3f _dirS = cell[i].pos + cell[i].dir;
            ofVec3f _dirE = cell[i + 1].pos + cell[i + 1].dir;
            ofDrawLine(_dirS, _dirE);
        }
    }
    
    for (int i = 0; i < cell.size(); i++) {
        if (i % (numCellOneSide * numCellOneSide) <= numCellOneSide * (numCellOneSide - 1) - 1) {
            ofVec3f _dirS = cell[i].pos + cell[i].dir;
            ofVec3f _dirE = cell[i + numCellOneSide].pos + cell[i + numCellOneSide].dir;
            ofDrawLine(_dirS, _dirE);
        }
    }
    
    for (int i = 0; i < cell.size(); i++) {
        if (i <= numCellOneSide * numCellOneSide * (numCellOneSide - 1) - 1) {
            ofVec3f _dirS = cell[i].pos + cell[i].dir;
            ofVec3f _dirE = cell[i + numCellOneSide * numCellOneSide].pos + cell[i + numCellOneSide * numCellOneSide].dir;
            ofDrawLine(_dirS, _dirE);
        }
    }
    
    ofPopStyle();
    
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){
        
    if (key == 'r') {
        randomFactor = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
        for(int i = 0; i < particles.size(); i++) {
            particles[i].path.clear();
        }
        setUpdateCell();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
