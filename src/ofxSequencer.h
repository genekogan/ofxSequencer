#pragma once

#include "ofMain.h"
#include "ofxSequencerBpm.h"


struct ofxSequencerRowBase
{
    ofxSequencerRowBase(int cols);
    
    virtual string getName() { }
    template<class T> T getMin();
    template<class T> T getMax();
    
    virtual void update(int column) { }
    virtual void update(float cursor) { }
    virtual void randomize() { }

    virtual void mousePressed(int col, int x, int y) { }
    virtual void mouseDragged(int col, int y) { }
    virtual void mouseReleased(int col) { }
    
    virtual void draw(int col, int cellWidth, int cellHeight) { }

    int cols;
};


template<typename T>
struct ofxSequencerRow : public ofxSequencerRowBase
{
    ofxSequencerRow(ofParameter<T> * parameter, int cols);
    
    string getName() {return parameter->getName();}
    T getMin() {return parameter->getMin();}
    T getMax() {return parameter->getMax();}

    void update(int column);
    void update(float cursor);
    void randomize();
    
    void mousePressed(int col, int x, int y);
    void mouseDragged(int col, int y);
    void mouseReleased(int col);
    
    void draw(int col, int cellWidth, int cellHeight);
    
    ofParameter<T> * parameter;
    vector<T> values;
    T pValue;
    ofPoint pMouse;
};


template<class T>
ofxSequencerRow<T>::ofxSequencerRow(ofParameter<T> * parameter, int cols) : ofxSequencerRowBase(cols)
{
    this->parameter = parameter;
    for (int c=0; c<cols; c++) {
        values.push_back(parameter->get());
    }
}

template<class T>
void ofxSequencerRow<T>::update(int column)
{
    *parameter = values[column];
}

template<class T>
void ofxSequencerRow<T>::update(float cursor)
{
    *parameter = ofLerp(values[(int) floor(cursor)], values[(int) ceil(cursor) % values.size()], cursor - floor(cursor));
}

template<class T>
void ofxSequencerRow<T>::randomize()
{
    for (int i = 0; i < values.size(); i++) {
        values[i] = ofRandom(parameter->getMin(), parameter->getMax());
    }
}

template<class T>
void ofxSequencerRow<T>::mousePressed(int col, int x, int y)
{
    pMouse.set(x, y);
    pValue = values[col];
}

template<class T>
void ofxSequencerRow<T>::mouseDragged(int col, int y)
{
    
}

template<>
inline void ofxSequencerRow<float>::mouseDragged(int col, int y)
{
    values[col] = ofClamp(pValue - 0.01 * (y - pMouse.y) * (parameter->getMax() - parameter->getMin()), parameter->getMin(), parameter->getMax());
}

template<>
inline void ofxSequencerRow<int>::mouseDragged(int col, int y)
{
    values[col] = ofClamp(pValue - 0.01 * (y - pMouse.y) * (parameter->getMax() - parameter->getMin()), parameter->getMin(), parameter->getMax());
}

template<class T>
void ofxSequencerRow<T>::draw(int col, int cellWidth, int cellHeight)
{
    float rectMult = 1.0 / (parameter->getMax() - parameter->getMin());
    ofRect(0, 0,
           cellWidth  * rectMult * (values[col] - parameter->getMin()),
           cellHeight * rectMult * (values[col] - parameter->getMin()));
}

template<>
inline void ofxSequencerRow<bool>::draw(int col, int cellWidth, int cellHeight)
{
    if (values[col]) {
        ofRect(0, 0, cellWidth, cellHeight);
    }
}

template<class T>
void ofxSequencerRow<T>::mouseReleased(int col)
{
    if (values[col] > 0.5 * (parameter->getMin() + parameter->getMax())){
        values[col] = parameter->getMin();
    }
    else {
        values[col] = parameter->getMax();
    }
}

template<>
inline void ofxSequencerRow<bool>::mouseReleased(int col)
{
    values[col] = 1.0 - values[col];
}

template<class T> T ofxSequencerRowBase::getMin() { return dynamic_cast<ofxSequencerRow<T>&>(*this).getMin(); }
template<class T> T ofxSequencerRowBase::getMax() { return dynamic_cast<ofxSequencerRow<T>&>(*this).getMax(); }



class ofxSequencer
{
public:
    ~ofxSequencer();
    
    void setup(int cols, int beatsPerMinute=120, int beatsPerBar=4);
    void setBpm(int beatsPerMinute, int beatsPerBar=4);
    void setSmooth(bool smooth) {this->smooth = smooth;}
    
    int getBpm() {return beatsPerMinute;}
    bool getSmooth() {return smooth;}
    
    
    template<class T>
    void addRow(ofParameter<T> * parameter);
    
    void start();
    void advance();
    void stop();
    void reset();
    void randomize();
    
    void update();
    void draw();
    
    void setPosition(int x, int y, int width, int height);
    void setVisible(bool visible);
    void toggleVisible();
    void setMouseActive(bool active);
    
    int getColumn() {return column;}
    vector<ofxSequencerRowBase*> & getRows() {return rows;}
    
    ofEvent<int> beatEvent;
    
private:
    
    void play(void);
    void redraw();
    
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    
    vector<ofxSequencerRowBase*> rows;
    
    float cursor;
    int column;
    int beatsPerMinute;
    int bpmInterval;
    int bpmTime;
    bool smooth;
    ofxSequencerBpm bpm;

    int cols;
    int x, y, width, height;
    float cellWidth, cellHeight;
    
    ofPoint mCell;
    bool draggingCell;
    int draggingFrames;
    
    ofFbo fbo;
};


template<class T>
void ofxSequencer::addRow(ofParameter<T> * parameter)
{
    ofxSequencerRow<T> *newRow = new ofxSequencerRow<T>(parameter, cols);
    rows.push_back(newRow);
    redraw();
}
