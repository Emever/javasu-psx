#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "InputManager.h"
#include "CameraManager.h"

using V3 = Vector3;
using V2 = Vector2;

class Player {
	
	private:
		V3 p, v, a; 			// position, velocity and acceleration
		float height;			// offsets camera 'y' position
		CameraManager* cam;		// camera manager
		InputManager* input;		// player input
		
	public:
		float mSpeed;	// move speed (input press)
		float aDrag;	// automatic deacceleration (input release), multiples mSpeed
		float radius;	// radius for player collision
		float collisionBounceBackStrength; // bounce-back collision strength
		
		Player(InputManager* input, CameraManager* cam);
		
		// [getters]
		V3 getPosition(), getVelocity();;
		float getPX(), getPY(), getPZ(), getVX(), getVY(), getVZ();
		CameraManager* getCam();
		InputManager* getInput();
		
		// [methods]
		void inputVelocity();
		void updatePVA(int lvlCount, LevelObject lvlObj);
		void updateCam();
};

#endif
