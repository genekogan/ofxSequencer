#pragma once

#include "ofMain.h"
#include "ofxSequencer.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void sequencerStep(vector<float> &column);
		
    void keyPressed(int key);
    
    ofxSequencer sequencer;
};
