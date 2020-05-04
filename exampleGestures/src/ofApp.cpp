#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(0);
	
	ofLogToConsole();

	kinect.setup("gestures.gbd", true);
	kinectusermanager.setup(kinect.getUsers());
	kinect2dscene.setup(kinectusermanager.getUsers());

	ofAddListener(ncKinectEventDispatcher::NCEVENTDISPATCHER.DISCRETE_GESTURE, this, &ofApp::discreteTracked);
	ofAddListener(ncKinectEventDispatcher::NCEVENTDISPATCHER.NEW_USER, this, &ofApp::newUser);
	ofAddListener(ncKinectEventDispatcher::NCEVENTDISPATCHER.LOST_USER, this, &ofApp::lostUser);

	// Look up the gestures and setup data to store their values.
	ncKinectv2GestureDetector * gesture_detector = kinect.getGestureDetector();
	gesture_count = gesture_detector->getGestureCount();

	for (int i = 0; i < gesture_count; i++) {
		gesture_names.push_back(gesture_detector->getGestureName(i));
		vector<float> d;
		gesture_state_data[gesture_names.back()] = d;
		ofLogNotice("Add Gesture") << i << ":'" << gesture_names.back() << "'";
	}

}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
	kinectusermanager.update();
	
	// offset data store over 30 samples
	for (int i = 0; i < gesture_count; i++) {
		string & nm = gesture_names[i];
		if (gesture_state_data[nm].size() == 30) {
			gesture_state_data[nm].erase( gesture_state_data[nm].begin() ); // remove oldest value.
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	kinect2dscene.drawDepthMap2d(kinect.getDepthMap2D());

	int x = 540;
	int y = 0;
	int h = 240;
	int dim = 200;

	for (int i = 0; i < gesture_count; i++) {
		drawGestureStateBox(i, gesture_state_data[gesture_names[i]], gesture_names[i], x, y, dim, dim);
		y += h;
	}

}

void ofApp::drawGestureStateBox(int gesture_index, vector<float> & data, string name, int x, int y, int w, int h) {

	ofPushMatrix();
	ofPushStyle();

	ofSetLineWidth(2);
	ofNoFill();
	
	ofTranslate(x, y);

	ofDrawRectangle(0, 0, w, h);
	ofFill();
	// draw the vertical lines.
	for (size_t i = 0; i < data.size(); i++) {
		ofDrawLine(ofPoint((i * 7), h), ofPoint((i * 7), h - (data[i] * h)));
	}

	// draw centre line
	ofSetColor(ofColor::red);
	ofDrawLine(ofPoint(0, h - (0.5 * h)), ofPoint(w, h - (0.5 * h))); // mid point.
	
	// draw name
	ofSetColor(255);
	ofSetLineWidth(1);
	ofDrawBitmapString(name, 0, h + 20);

	ofPopMatrix();	 
	ofPopStyle();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::discreteTracked(NCGenericEventArg & arg) {

	// arg.message : name of gesture
	// arg.value : norm position?
	// arg.userid : int of user / skeleton.

	if ( gesture_state_data.find(arg.message) != gesture_state_data.end() ) { // gesture is valid name
		gesture_state_data[arg.message].push_back(arg.value);
	}

}

//--------------------------------------------------------------
void ofApp::newUser(NCGenericEventArg & arg) {
	ncKinectUser * user = kinectusermanager.getUserById(arg.userid);
	kinect.turnOffGestureDectors(); // Why? to stop tracking for said user? lostUser might be better?
	kinect.turnOnGestureDetectorForUser(user->id, user->kinectid);
}

//--------------------------------------------------------------
void ofApp::lostUser(NCGenericEventArg & arg) {
	kinect.turnOffGestureDectors();
}
