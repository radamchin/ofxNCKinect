#pragma once

#include "ofMain.h"
#include "Kinect.h"
#include "Kinect.VisualGestureBuilder.h"
#include "ncKinectEventDispatcher.h"

class ncKinectv2GestureDetector {

public:
	~ncKinectv2GestureDetector();
	ncKinectv2GestureDetector(IKinectSensor* _sensor, int _id, string _database);

	void update();

	UINT64 getTrackingId();
	void setTrackingId(UINT64 _id);

	bool getPaused();
	void setPaused(bool _paused);

	int getGestureCount() {
		return numberofgestures;
	}

	string getGestureName(int index) {
		return gesture_names[index];
	}

private:
	IKinectSensor* sensor;
	IVisualGestureBuilderDatabase		*	dataBase;
	IVisualGestureBuilderFrameReader	*	frameReader;
	IVisualGestureBuilderFrameSource	*	frameSource;
	vector<IGesture *>  gestures;
	vector<std::string> gesture_names;
	UINT numberofgestures;
	int id;
};

// Safe release for interfaces
template<class Interface>
inline void SafeReleaseGDT(Interface *& pInterfaceToRelease) {
	if (pInterfaceToRelease != NULL) {
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}
