#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  grabber.setDeviceID(1);
  grabber.setup(1280,720);
  tracker.setup();

  gui.setup();
  gui.add(maxEyeRange.set("maxEyeRange", 23.6, 0, 100));


  ofEnableAlphaBlending();
  for ( int i = 0; i < num; i++ ) {
    ax.push_back(0.0);
    ay.push_back(0.0);
  }

  ofSetDataPathRoot(ofFile(__BASE_FILE__).getEnclosingDirectory()+"../../model/");


  for(int i=1; i < 14; i++){
    string eye_file =  "/Users/josephwilk/Workspace/c++/of_v0.11.2_osx_release/apps/myApps/medicalGaze/bin/data/"+ofToString(i)+"_eye.png";
    string pupil_file =  "/Users/josephwilk/Workspace/c++/of_v0.11.2_osx_release/apps/myApps/medicalGaze/bin/data/"+ofToString(i)+"_pupil.png";

    if(i!=11){

      ofImage eyeTemp;
      eyeTemp.load(eye_file);
      // temp.resize(43*3,16*3);
      eyes.push_back(eyeTemp);

      ofImage pupilTemp;
      pupilTemp.load(pupil_file);
      pupils.push_back(pupilTemp);
    }

  }

  pupilCenters.push_back((glm::vec2(40,36)));
  pupilCenters.push_back((glm::vec2(41,36)));
  pupilCenters.push_back((glm::vec2(38,37)));
  pupilCenters.push_back((glm::vec2(40,36)));
  pupilCenters.push_back((glm::vec2(39,36)));
  pupilCenters.push_back((glm::vec2(37,34)));
  pupilCenters.push_back((glm::vec2(36,35)));


  backgroundEye1.load("/Users/josephwilk/Workspace/c++/of_v0.11.2_osx_release/apps/myApps/medicalGaze/bin/data/noise_background1.png");
  backgroundEye2.load("/Users/josephwilk/Workspace/c++/of_v0.11.2_osx_release/apps/myApps/medicalGaze/bin/data/noise_background2.png");

  int eyeWidth =75;
  int eyeHeight=75;

  int i =0;
  for (int y = 0; y < ofGetHeight() / eyeHeight + 0.0; y++) {
    for (int x = 0; x < ofGetWidth() / eyeWidth + 0.0; x++) {
      jitter.push_back(ofRandom(-1.1, 1.1));
      jitterGradient.push_back(ofRandom(5, 35.0));
    }
  }
}

//--------------------------------------------------------------
void ofApp::update(){
  grabber.update();

  if(grabber.isFrameNew()){
    tracker.update(grabber);
  }
  ofSeedRandom();
  for (int i = 1; i < num; i++ ) {
    ax[i - 1] = ax[i];
    ay[i - 1] = ay[i];
  }
  float r1 = ofRandomf() * range;
  float r2 = ofRandomf() * range;
  // Put a new value at the end of the array
  ax[num - 1] += r1 ;
  ay[num - 1] += r2 ;

  // Constrain all points
  ax[num - 1] = ofClamp(ax[num - 1], -5,5);
  ay[num - 1] = ofClamp(ay[num - 1], -5,5);

}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(0);
  ofPushView();

  float factor = 0.5;

  ofScale(factor);

  int eyeWidth =75;
  int eyeHeight=75;
  glm::vec2 focusTarget = glm::vec2(mouseX, mouseY);
  //focusTarget = glm::vec2(noseX, noseY);

  for (int y = 0; y < 23*2; y++) {
    for (int x = 0; x < 36*2; x++) {
      int eyeX = eyeWidth * x;
      int eyeY = eyeHeight * y;
      int selectedEye = ofNoise(x,y) * eyes.size();

      glm::vec2 pupilTarget = pupilCenters[selectedEye % pupilCenters.size()];
      pupilTarget.x += eyeX;
      pupilTarget.y += eyeY;

      float distance = glm::distance(focusTarget,pupilTarget);

//      if(distance > 6359){
//        cout << "distance:" << distance << " ";
//      }
      glm::vec2 focus = focusTarget - pupilTarget;
      focus = glm::normalize(focus);

//      float maxScreenDistance = 6359.0;
      float maxScreenDistance = 5000.0;
      float mappedDistance = ofMap(distance, 0.0, maxScreenDistance, 0.0, maxEyeRange - (jitter[selectedEye]));
      //focus = focus*mappedDistance;
      focus.x = (mappedDistance * 1.01) * focus.x;
      focus.y = mappedDistance * focus.y;

      glm::vec2 pupilPos = pupilTarget + focus;
      pupilPos.x += ax[num - (x+y % num)];
      pupilPos.y += ay[num - (x+y % num) ];

      int offWhite = jitterGradient[selectedEye] * 2;
      ofSetColor(242 - offWhite, 242 - offWhite, 240 - offWhite);
      ofFill();

      ofDrawRectangle(eyeX, eyeY, 75, 76);

      if(ofNoise(x,y) > 0.5){
        backgroundEye2.draw(eyeX, eyeY);
      }
      else{
        backgroundEye1.draw(eyeX, eyeY);
      }

      pupils[selectedEye].draw( pupilPos.x - (eyeWidth / 2),  pupilPos.y - (eyeHeight / 2));
      eyes[selectedEye].draw(eyeX,eyeY);
    }
  }

  //grabber.draw(0, 0);

  ofPopView();

  for(auto face : tracker.getInstances()){
    //ofPolyline leftEye = face.getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE);
    //ofPolyline rightEye = face.getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE);
    ofPolyline nose = face.getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::NOSE_BASE);
    glm::vec2 p = nose.getVertices()[0];

    noseX=p.x;
    noseY=p.y;

    noseX = (p.x/1280) * ofGetWidth();
    noseY = (p.y/720)  * ofGetHeight();
    //ofDrawCircle(noseX, noseY, 10);

  }
  if( !bHide ){
    gui.draw();
  }
  //ofDrawBitmapStringHighlight("Tracker fps: "+ofToString(tracker.getThreadFps()), 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key == 'g'){
    bHide=!bHide;
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
