#include "ofxSequencer.h"

//-------
void ofxSequencer::setup(int rows, int cols, int beatsPerMinute, int beatsPerBar) {
    setSize(rows, cols);
    setBpm(beatsPerMinute, beatsPerBar);
    ofAddListener(bpm.beatEvent, this, &ofxSequencer::play);
    setVisible(true);
    setPosition(0, 0, 120, 40);
}

//-------
void ofxSequencer::setBpm(int beatsPerMinute, int beatsPerBar) {
    bpm.setBpm(beatsPerMinute);
    bpm.setBeatPerBar(beatsPerBar);
}

//-------
void ofxSequencer::start() {
    bpm.start();
}

//-------
void ofxSequencer::stop() {
    bpm.stop();
}

//-------
void ofxSequencer::reset() {
    bpm.reset();
}

//-------
void ofxSequencer::setSize(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    value.clear();
    for (int c=0; c<cols; c++) {
        vector<float> colValues;
        value.push_back(colValues);
        for (int r=0; r<rows; r++) {
            value[c].push_back(0.0);
        }
    }
    column = 0;
}

//-------
void ofxSequencer::setValue(int row, int col, float val) {
    value[col][row] = val;
    redraw();
}

//-------
vector<float> ofxSequencer::getRow(int row) {
    vector<float> theRow;
    for (int i=0; i<cols; i++) {
        theRow.push_back(value[i][row]);
    }
    return theRow;
}

//-------
vector<float> ofxSequencer::getColumn(int col) {
    return value[col];
}

//-------
void ofxSequencer::play(void) {
    column = (column + 1) % cols;
    ofNotifyEvent(sequencerEvent, value[column], this);
}

//-------
void ofxSequencer::setPosition(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0, 0);
    fbo.end();
    redraw();
}

//-------
void ofxSequencer::draw() {
    if (!visible)  return;
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(x, y);
    
    fbo.draw(0, 0, width, height);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetLineWidth(4);
    ofSetColor(255, 0, 0);
    ofNoFill();
    ofRect(cellWidth * column, 0, cellWidth, height);
    
    ofPopStyle();
    ofPopMatrix();
}

//-------
void ofxSequencer::setVisible(bool visible) {
    this->visible = visible;
    setMouseActive(visible);
}

//-------
void ofxSequencer::toggleVisible() {
    setVisible(!visible);
}

//-------
void ofxSequencer::setMouseActive(bool active) {
    if (active) {
        ofAddListener(ofEvents().mousePressed, this, &ofxSequencer::mousePressed);
        ofAddListener(ofEvents().mouseReleased, this, &ofxSequencer::mouseReleased);
        ofAddListener(ofEvents().mouseDragged, this, &ofxSequencer::mouseDragged);
    }
    else {
        ofRemoveListener(ofEvents().mousePressed, this, &ofxSequencer::mousePressed);
        ofRemoveListener(ofEvents().mouseReleased, this, &ofxSequencer::mouseReleased);
        ofRemoveListener(ofEvents().mouseDragged, this, &ofxSequencer::mouseDragged);
    }
}

//-------
void ofxSequencer::mousePressed(ofMouseEventArgs &evt){
    ofRectangle seqRect(x, y, width, height);
    if (seqRect.inside(evt.x, evt.y)){
        activeCell.set(floor((evt.x - x) / cellWidth),
                       floor((evt.y - y) / cellHeight));
        mousePos.set(evt.x, evt.y);
        draggingCell = true;
    }
}

//-------
void ofxSequencer::mouseDragged(ofMouseEventArgs &evt){
    if (draggingCell) {
        value[activeCell.x][activeCell.y] = ofClamp(value[activeCell.x][activeCell.y] - 0.005*(evt.y-mousePos.y), 0.0, 1.0);
        mousePos.set(evt.x, evt.y);
        draggingFrames++;
        redraw();
    }
}

//-------
void ofxSequencer::mouseReleased(ofMouseEventArgs &evt){
    if (draggingCell && draggingFrames==0) {
        if (value[activeCell.x][activeCell.y] > 0.5) {
            value[activeCell.x][activeCell.y] = 0.0;
        }
        else {
            value[activeCell.x][activeCell.y] = 1.0;
        }
    }
    else {
        draggingFrames = 0;
    }
    draggingCell = false;
    redraw();
}

//-------
void ofxSequencer::redraw() {
    cellWidth = (float) width / cols;
    cellHeight = (float) height / rows;
    
    fbo.begin();
    
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(0);
    ofFill();
    ofRect(0, 0, width, height);
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofTranslate(0.5*cellWidth, 0.5*cellHeight);
    for (int r=0; r<rows; r++) {
        for (int c=0; c<cols; c++) {
            ofSetColor(0, 255, 0);
            ofRect(0, 0, cellWidth * value[c][r], cellHeight * value[c][r]);
            ofTranslate(cellWidth, 0);
        }
        ofTranslate(-cols*cellWidth, cellHeight);
    }
    ofTranslate(-0.5*cellWidth, (-0.5-rows)*cellHeight);
    
    ofSetColor(255);
    ofSetLineWidth(2);
    for (int r=1; r<rows; r++) {
        ofLine(0, r*cellHeight, width, r*cellHeight);
    }
    for (int c=1; c<cols; c++) {
        ofLine(c*cellWidth, 0, c*cellWidth, height);
    }
    
    ofPopStyle();
    ofPopMatrix();
    
    fbo.end();
}

