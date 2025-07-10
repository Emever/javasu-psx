#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "InputManager.h"
#include "utils.h"


class CameraManager {
	private:
		InputManager* input;	// InputManager
		Camera cam; 			// alias for Camera3D
		float camYaw; 			// camera rotation according to 'Y' axis
		float camTilt;			// camera rotation according to 'X' axis
		Vector3 camForward, camRight; // [for faster camera calculation]
		Vector3 movement;		// next calculated movement according to input (to be checked with collisions)
		
	public:
		//float radius = .45f;
		//float collisionBounceBackStrength = .5f;
		float rotSpeed;
		
		CameraManager();
		CameraManager(InputManager *i);
		
		// [getters]
		Camera getCam();
		float getFacingCamAngle(float x, float z);
		Vector3 getMovement();
		Vector3 getPosition();
		float getPositionX();
		float getPositionY();
		float getPositionZ();
		float getCamYaw();
		float getCamTilt();
		Vector3 getCamForward();
		Vector3 getCamRight();
		
		// [setters]
		void setPosition(Vector3 p);
		void setPositionX(float x);
		void setPositionY(float y);
		void setPositionZ(float z);
		void setCamYaw(float yaw);
		void setCamTilt(float tilt);
		void increaseCamYaw(float dYaw);
		void increaseCamTilt(float dTilt);
		void setCamForward(Vector3 cForward);
		void setCamRight(Vector3 cRight);
		
		// [methods]
		void updateCameraLookAt();
		void RenderSkybox(Model skybox);
		void billboardModel(Model *m, Vector2 p);
};

#endif
