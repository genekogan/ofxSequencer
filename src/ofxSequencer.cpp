#include "ofxSequencer.h"

//-------
void ofxSequencer::setup(int rows, int cols, bool discrete, int beatsPerMinute, int beatsPerBar) {
    setSize(rows, cols);
    setRange(0.0, 1.0);
    setDiscrete(discrete);
    setBpm(beatsPerMinute, beatsPerBar);
    setVisible(true);
    setPosition(0, 0, 120, 40);
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
    redraw();
}

//-------
void ofxSequencer::setDiscrete(bool discrete) {
    this->discrete = discrete;
    if (discrete) {
        for (int r=0; r<rows; r++) {
            for (int c=0; c<cols; c++) {
                if (value[c][r] > 0.5 * (minValue+maxValue)) {
                    value[c][r] = 1.0;
                }
                else {
                    value[c][r] = 0.0;
                }
            }
        }
        minValue = 0.0;
        maxValue = 1.0;
    }
    redraw();
}

//-------
void ofxSequencer::setBpm(int beatsPerMinute, int beatsPerBar) {
    bpm.setBpm(beatsPerMinute);
    bpm.setBeatPerBar(beatsPerBar);
}

//-------
void ofxSequencer::start() {
    ofAddListener(bpm.beatEvent, this, &ofxSequencer::play);
    bpm.start();
}

//-------
void ofxSequencer::stop() {
    ofRemoveListener(bpm.beatEvent, this, &ofxSequencer::play);
    bpm.stop();
}

//-------
void ofxSequencer::reset() {
    bpm.reset();
    column = 0;
}

//-------
void ofxSequencer::randomize() {
    for (int r=0; r<rows; r++) {
        for (int c=0; c<cols; c++) {
            setValue(r, c, ofRandom(1));
        }
    }
}

//-------
void ofxSequencer::setRange(float minValue, float maxValue) {
    this->minValue = minValue;
    this->maxValue = maxValue;
    redraw();
}

//-------
void ofxSequencer::setValue(int row, int col, float val) {
    value[col][row] = discrete ? (val > 0.5) : val;
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
    advance();
}

//-------
void ofxSequencer::advance() {
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
    ofSetColor(255);
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
    if (discrete)   return;
    if (draggingCell) {
        value[activeCell.x][activeCell.y] = ofClamp(value[activeCell.x][activeCell.y] - 0.005 * (maxValue - minValue) * (evt.y-mousePos.y), minValue, maxValue);
        mousePos.set(evt.x, evt.y);
        draggingFrames++;
        redraw();
    }
}

//-------
void ofxSequencer::mouseReleased(ofMouseEventArgs &evt){
    if (draggingCell && draggingFrames==0) {
        if (discrete ) {
            value[activeCell.x][activeCell.y] = 1.0 - value[activeCell.x][activeCell.y];
        }
        else {
            if (value[activeCell.x][activeCell.y] > 0.5 * (minValue + maxValue)) {
                value[activeCell.x][activeCell.y] = minValue;
            }
            else {
                value[activeCell.x][activeCell.y] = maxValue;
            }
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
    cellWidth  = (float) width  / cols;
    cellHeight = (float) height / rows;
    float rectMult = 1.0 / (maxValue - minValue);
    
    fbo.begin();
    
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(0);
    ofFill();
    ofRect(0, 0, width+120, height);
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofTranslate(0.5*cellWidth, 0.5*cellHeight);
    for (int r=0; r<rows; r++) {
        ofSetColor(255);
        for (int c=0; c<cols; c++) {
            ofRect(0, 0,
                   cellWidth  * rectMult * (value[c][r] - minValue),
                   cellHeight * rectMult * (value[c][r] - minValue));
            ofTranslate(cellWidth, 0);
        }
        ofTranslate(-cols*cellWidth, cellHeight);
    }
    ofTranslate(-0.5*cellWidth, (-0.5-rows)*cellHeight);
    
    ofSetColor(100);
    ofSetLineWidth(1);
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

//-------
ofxSequencer::~ofxSequencer() {
    stop();
    setVisible(false);  // clean up mouse listeners
}
