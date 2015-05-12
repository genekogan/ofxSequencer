#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // setup the sequencer with 6 rows, 8 columns
    sequencer.setup(6, 8);
    
    // set to 120 bpm (4 beats per bar)
    sequencer.setBpm(120, 4);
    
    // set for continuous values (default is continuous)
    sequencer.setDiscrete(false);
    
    // set value range (only for continuous/non-discrete sequencers)
    sequencer.setRange(5.0, 10.0);
    
    // where to draw it on the screen
    sequencer.setPosition(100, 100, 500, 200);
    
    // event notifier
    ofAddListener(sequencer.sequencerEvent, this, &ofApp::sequencerStep);
    
    // start the sequencer
    sequencer.start();
    
    // set values
    for (int r=0; r<sequencer.getNumberRows(); r++) {
        for (int c=0; c<sequencer.getNumberColumns(); c++) {
            sequencer.setValue(r, c, ofRandom(5, 10));
        }
    }
    
    // try dragging your mouse over the sequencer to change the values
}

void ofApp::sequencerStep(vector<float> &column)
{
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
