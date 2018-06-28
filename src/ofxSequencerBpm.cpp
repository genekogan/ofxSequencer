//  Based on ofxBpm by mirrorboy


#include "ofxSequencerBpm.h"
#include "ofMain.h"

ofxSequencerBpm::ofxSequencerBpm(float bpm,int beatPerBar):_beatPerBar(beatPerBar)
{
    _isPlaying = false;
    _isTick = false;
    setBpm(bpm);
};


void ofxSequencerBpm::start()
{
    stop();
    _isPlaying = true;
    reset();
    //startThread(true, false);   // blocking, verbose
    startThread();
}

void ofxSequencerBpm::stop()
{
    waitForThread(true);
    _isPlaying = false;
}

void ofxSequencerBpm::reset()
{
    if( lock() )
    {
        _remainderOfTick = 0;
        _countOfTick = -1;
        _preTime = ofGetElapsedTimeMicros();
        _totalTime = 0;
        unlock();
    }
}


void ofxSequencerBpm::threadedFunction()
{
    while(isThreadRunning() != 0 )
    {
        if(lock())
        {
            if(_isPlaying == true)
            {
                long nowTime = ofGetElapsedTimeMicros();
                long deltaTime = nowTime - _preTime;
                _preTime = nowTime;
                
                if(_totalTime + _remainderOfTick >= _durationOfTick)
                {
                    if((_countOfTick % (OFX_BPM_TICK / _beatPerBar)) == 0) {
                        ofNotifyEvent(beatEvent);
                    }
                    _remainderOfTick = (_totalTime + _remainderOfTick) % _durationOfTick;
                    _totalTime = 0.;
                    _isTick = true;
                    
                    _countOfTick++;
                    _countOfTick %= OFX_BPM_TICK;
                }
                else{
                    _isTick = false;
                }
                _totalTime += deltaTime;
                
                unlock();
                yield();
            }
        }
    }
}

void ofxSequencerBpm::setBeatPerBar(int beatPerBar)
{
    if(lock())
    {
        _beatPerBar = beatPerBar;
        unlock();
    }
}

void ofxSequencerBpm::setBpm(float bpm)
{
    if(lock())
    {
        if(bpm < OFX_BPM_MIN){
            _bpm = OFX_BPM_MIN;
        }
        else if(bpm >= OFX_BPM_MAX){
            _bpm = OFX_BPM_MAX;
        }
        else {
            _bpm = bpm;
        }
        
        _durationOfTick = 60. * 1000. * 1000. / (bpm * (OFX_BPM_TICK >> 2));
        unlock();
    }
}

float ofxSequencerBpm::getBpm() const
{
    return _bpm;
}

bool ofxSequencerBpm::isPlaying() const
{
    return _isPlaying;
}
