#include "Player.h"

Player::Player(InputManager* input, CameraManager* cam) {
	this->input = input;
	this->cam = cam;
	
	this->p = { 0 };
	this->v = { 0 };
	this->a = { 0 };
	
	this->height = .7f;
	this->mSpeed = 3.0f;
	this->aDrag = 9.0f; // suggested drag: between (5, 10)
	this->radius = .35f;
	this->collisionBounceBackStrength = .5f;
}


// [getters]

V3 Player::getPosition() { return this->p; }
float Player::getPX() { return this->p.x; }
float Player::getPY() { return this->p.y; }
float Player::getPZ() { return this->p.z; }

V3 Player::getVelocity() { return this->v; }
float Player::getVX() { return this->v.x; }
float Player::getVY() { return this->v.y; }
float Player::getVZ() { return this->v.z; }

CameraManager* Player::getCam() { return this->cam; }
InputManager* Player::getInput() { return this->input; }


// [methods]

void Player::inputVelocity() {
	
	// Inputted rotation
	if (this->input->vLeft)	this->cam->increaseCamYaw(-this->cam->rotSpeed * GetFrameTime());
	if (this->input->vRight) this->cam->increaseCamYaw(this->cam->rotSpeed * GetFrameTime());
	
	
	// Calculate forward and right camera vectors
	this->cam->setCamForward( (V3) { sinf(DEG2RAD * this->cam->getCamYaw()), 0.0f, -cosf(DEG2RAD * this->cam->getCamYaw()) });
	this->cam->setCamRight( (V3) { cosf(DEG2RAD * this->cam->getCamYaw()), 0.0f, sinf(DEG2RAD * this->cam->getCamYaw()) });
	
	// Inputted velocity (use 'a' for delta 'v')
	this->a = { 0 };
	if (input->mUp)  	this->a = Vector3Add(this->a, this->cam->getCamForward());
	if (input->mDown) 	this->a = Vector3Subtract(this->a, this->cam->getCamForward());
	if (input->mLeft)   this->a = Vector3Subtract(this->a, this->cam->getCamRight());
	if (input->mRight)  this->a = Vector3Add(this->a, this->cam->getCamRight());
	
	// Normalize and scale velocity
	if (Vector3Length(this->a) > 0) {
		this->a = Vector3Normalize(this->a);
		this->v = Vector3Scale(this->a, this->mSpeed);
		this->a = { 0 };
	}
	
}

void Player::updatePVA(int lvlCount, LevelObject lvlObj) {
	
	// concat acceleration -> velocity -> position
	this->a = Vector3Scale(this->v, -this->aDrag); // Use 'a' negative for friction
	this->v = Vector3Add(this->v, Vector3Scale(this->a, GetFrameTime()));
	
	if (Vector3Length(this->v) <=.25f)
		this->v = { 0 }; // force velocity to stop if undervalues this threshold
	
	// save the next position before assigning it (we need to check for collisions, beforehand)
	V3 newP = Vector3Add(this->p, Vector3Scale(this->v, GetFrameTime()));
	
	// Check for collision
	if (!CheckSphereCollision(newP, this->radius, &lvlObj, lvlCount)) {
		this->p = newP; // No collision, apply movement
		
	} else { // Collision (partial or total)
		Vector3 bounceBack = { 0 };
		
		// Blocked — try X only
		Vector3 testX = { newP.x, this->p.y, this->p.z };
		if (!CheckSphereCollision(testX, this->radius, &lvlObj, lvlCount)) { this->p.x = testX.x; }
		else { bounceBack.x = -(this->v.x) * this->collisionBounceBackStrength; }
		
		// Then try Z only
		Vector3 testZ = { this->p.x, this->p.y, newP.z };
		if (!CheckSphereCollision(testZ, this->radius, &lvlObj, lvlCount)) { this->p.z = testZ.z; }
		else { bounceBack.z = -(this->v.z) * this->collisionBounceBackStrength; }

		// Apply the bounce effect if movement was blocked
		this->p =(Vector3Add(this->p, Vector3Scale(bounceBack, GetFrameTime())));
	}
	
	// update the cam position and direction with the new player position
	this->updateCam();
}


void Player::updateCam() {
	this->getCam()->setPosition( { this->p.x, this->p.y + this->height, this->p.z } );
	this->getCam()->updateCameraLookAt();
}