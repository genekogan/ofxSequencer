# ofxSequencer

addon for OpenFrameworks for creating an auto-updating sequencer. a couple features:

 - can be discrete (all cells 1 or 0) or continuously valued.
 - interacts with the mouse. drag the mouse over a cell to change the value, or click it to snap to 0/1
 - updates automatically, in its own thread, sends notifier to your app (based on ofxBpm)

requires [ofxBpm](https://github.com/mirrorboy714/ofxBpm).