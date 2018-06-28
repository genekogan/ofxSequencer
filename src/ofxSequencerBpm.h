//  Based on ofxBpm by mirrorboy

#pragma once
#include "ofMain.h"
#include "ofThread.h"

class ofxSequencerBpm : private ofThread
{
public:
    const float OFX_BPM_MAX = 300.0;
    const float  OFX_BPM_DEFAULT = 120.0;
    const float OFX_BPM_MIN = 1.;
    const int OFX_BPM_TICK = 960;
    
    explicit ofxSequencerBpm(float bpm = 120.0, int beatPerBar = 4);
    
    void start();
    void stop();
    void reset();
    
    void setBpm(float bpm);
    float getBpm() const;
    
    void setBeatPerBar(int beatPerBar);
    
    bool isPlaying() const;
    
    ofEvent<void> beatEvent;
    
private:
    void threadedFunction();
    
    float _bpm;
    bool  _isTick;
    bool  _isPlaying;
    int _countOfTick;
    long _totalTime;
    long _durationOfTick;
    long _remainderOfTick;
    
    float _preTime;
    int _beatPerBar;
    
    inline int getCountOfTick() const;
};

//init
//const float ofxSequencerBpm::OFX_BPM_MAX = 300. ;
//const float ofxSequencerBpm::OFX_BPM_DEFAULT = 120.;
//const float ofxSequencerBpm::OFX_BPM_MIN = 1.;
//const int ofxSequencerBpm::OFX_BPM_TICK = 960;

