#pragma once

#include "ofMain.h"
#include "ncKinectv2Core.h"
#include "ncKinectUserManager.h"
#include "ncKinect2dScene.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

		void discreteTracked(NCGenericEventArg &arg);
		ncKinectv2Core kinect;
		ncKinectUserManager kinectusermanager;
		ncKinect2dScene kinect2dscene;

		void newUser(NCGenericEventArg &arg);
		void lostUser(NCGenericEventArg &arg);

		void drawGestureStateBox(int gesture_index, vector<float> & data, string name, int x, int y, int w, int h);

		int gesture_count;
		
		map<string, vector<float>> gesture_state_data; // store of gesture confidence values
		vector<string> gesture_names;

		// TODO: make this gesture_state_data extra depth to track data per user index ? id?

};
