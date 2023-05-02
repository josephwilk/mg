#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker2.h"


class ofApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);


  ofVideoGrabber grabber;

  ofxFaceTracker2 tracker;

  std::deque<double> oldTracker;
  std::deque<double> newTracker;


  //let video;
  //let poseNet;
  int poseDistance = 0;
  
  int noseX = 0;
  int noseY = 0;

  int eyelX = 0;
  int eyelY = 0;
  int rawX=0;
  int rawY=0;
  //var gui;
  float maxEyeRange=8.1;
  float minEyeRange=0.5;
  bool videoOn = true;

  //Saccades jitter: The eyes are never completely at rest

  vector<ofImage> eyes;
  vector<ofImage> pupils;
  vector<glm::vec2> pupilCenters;

  vector<float> jitter;
  vector<float> jitterGradient;


  ofImage backgroundEye1;
  ofImage backgroundEye2;

  //Tracking motion:
  //1. Saccades
  //2. Saccades to correct
  //3. Smooth follow.

  bool guiOn = true;

  int num = 100;
  float range = 0.1;

  vector<float> ax;
  vector<float> ay;

};
