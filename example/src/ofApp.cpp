#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    sequencer.setup(10, 120, 4);
    
    p1.set("param 1 (f)", 0.2, 0, 1);
    p2.set("param 2 (f)", 7, 5, 10);
    p3.set("param 3 (i)", 2, 0, 4);
    p4.set("param 4 (b)", false);
    
    sequencer.addRow(&p1);
    sequencer.addRow(&p2);
    sequencer.addRow(&p3);
    sequencer.addRow(&p4);
    
    sequencer.randomize();
    sequencer.setSmooth(true);
    
    sequencer.start();
    
    sequencer.setPosition(0, 0, 500, 200);
}

//--------------------------------------------------------------
void ofApp::update(){
    sequencer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    sequencer.draw();
    
    p4 ? ofSetColor(255, 0, 0) : ofSetColor(255);
    
    ofDrawCircle(ofMap(p1, p1.getMin(), p1.getMax(), 0, ofGetWidth()), 600, 50);
    ofDrawCircle(ofMap(p2, p2.getMin(), p2.getMax(), 0, ofGetWidth()), 700, 50);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' ') {
        // set row 0, column 1 to random number
        sequencer.setValue<float>(0, 1, ofRandom(1));
        // set row 2, column 3 to random integer (since p3 is an int)
        sequencer.setValue<int>(2, 3, int(ofRandom(4)));
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
