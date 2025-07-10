#include "CameraManager.h"


CameraManager::CameraManager() {}

CameraManager::CameraManager(InputManager *i) {
	this->camYaw = .0f;
	this->camTilt = .0f
	this->camForward = { 0 };
	this->camRight = { 0 };
	this->rotSpeed = 120.0f;
	
	this->input = i;
	this->movement = { 0 };
	
	this->cam = { 0 }; // fast-init
    this->cam.position = (Vector3){ .0f, .7f, .0f };  // Eye position
    this->cam.target = (Vector3){ 0.0f, 0.5f, 0.0f };    // Look-at point
    this->cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };        // Up vector
    this->cam.fovy = 60.0f;
    this->cam.projection = CAMERA_PERSPECTIVE;
}


// [getters]

Camera CameraManager::getCam() { return this->cam; }
	/// Calculates the direction vector on the XZ plane, and then arctans to get the angle
float CameraManager::getFacingCamAngle(float x, float z) { return atan2f((this->cam.position.x - x), (this->cam.position.z - z)); }

Vector3 CameraManager::getMovement() { return this->movement; }

Vector3 CameraManager::getPosition() { return this->cam.position; }

float CameraManager::getPositionX() { return this->cam.position.x; }

float CameraManager::getPositionY() { return this->cam.position.y; }

float CameraManager::getPositionZ() { return this->cam.position.z; }

float CameraManager::getCamYaw() { return this->camYaw; }

Vector3 CameraManager::getCamForward() { return this->camForward; }

Vector3 CameraManager::getCamRight() { return this->camRight; }


// [setters]

void CameraManager::setPosition(Vector3 p) { this->cam.position = p; }

void CameraManager::setPositionX(float x) { this->cam.position.x = x; }

void CameraManager::setPositionY(float y) { this->cam.position.y = y; }

void CameraManager::setPositionZ(float z) { this->cam.position.z = z; }

void CameraManager::setCamYaw(float yaw) { this->camYaw = yaw; }

void CameraManager::increaseCamYaw(float dYaw) { this->camYaw += dYaw; }

void CameraManager::setCamForward(Vector3 cForward) { this->camForward = cForward; }

void CameraManager::setCamRight(Vector3 cRight) { this->camRight = cRight; }


// [methods]

void CameraManager::billboardModel(Model* m, Vector2 p) {
	m->transform = MatrixRotateY(getFacingCamAngle(p.x, p.y));
}

void CameraManager::updateCameraLookAt() {
	// Recalculate target (1 unit forward)
	this->cam.target = (Vector3) {
		this->cam.position.x + this->camForward.x,
		this->cam.position.y,
		this->cam.position.z + this->camForward.z
	};
}

void CameraManager::RenderSkybox(Model skybox) {
	rlDisableDepthMask();
	DrawModel(skybox, this->cam.position, 1.0f, WHITE); // Make sure it's centered on camera
	rlEnableDepthMask();
}