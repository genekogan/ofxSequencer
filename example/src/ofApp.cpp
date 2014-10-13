#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // setup the sequencer with 6 rows, 8 columns, at 120 bpm (4 beats per bar)
    sequencer.setup(6, 8);
    sequencer.setBpm(120, 4);
    
    // where to draw it on the screen
    sequencer.setPosition(100, 100, 500, 200);
    
    // event listener
    sequencer.addBeatListener(this, &ofApp::sequencerStep);
    sequencer.start();
    
    // set values
    for (int r=0; r<sequencer.getNumberRows(); r++) {
        for (int c=0; c<sequencer.getNumberColumns(); c++) {
            sequencer.setValue(r, c, ofRandom(1));
        }
    }
    
    // try dragging your mouse over the sequencer to change the values
}

void ofApp::sequencerStep(vector<float> &column) {
    cout << ofToString(column) << endl;
}

void ofApp::keyPressed(int key) {
    if (key=='g') {
        sequencer.toggleVisible();
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    sequencer.draw();
}
