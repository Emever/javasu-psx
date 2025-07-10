#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "raylib.h"
#include <stdbool.h>

class InputManager {
	
	private:
		bool gamepadConnected;
	
	public:

		bool mUp, mLeft, mDown, mRight; // left axis - move managing
		bool vUp, vLeft, vDown, vRight; // right axis - view managing
		bool btnL, btnR, tLeft, tRight;	// triggers
		bool btnX, btnY, btnA, btnB;	// buttons
		bool btnSEL, btnSTR;	// "select" and "start" middle buttons

		InputManager();
		void updateInputManager();
		void logInputState();
	
};

#endif
