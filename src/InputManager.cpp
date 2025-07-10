#include "InputManager.h"

InputManager::InputManager() {
	this->gamepadConnected = IsGamepadAvailable(0); // Detect gamepad
	
    if (this->gamepadConnected)
		TraceLog(LOG_INFO, "Gamepad 0 connected: %s", GetGamepadName(0));
	else
		TraceLog(LOG_WARNING, "No gamepad detected.");
}

void InputManager::updateInputManager() {
    this->gamepadConnected = IsGamepadAvailable(0); // Detect gamepad

    // Left joystick
    this->mLeft = this->gamepadConnected ? 	(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) < -0.3f)	: IsKeyDown(KEY_A);
    this->mRight = this->gamepadConnected ? (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) > 0.3f) 	: IsKeyDown(KEY_D);
    this->mUp = this->gamepadConnected ? 	(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) < -0.3f) 	: IsKeyDown(KEY_W);
    this->mDown = this->gamepadConnected ? 	(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) > 0.3f) 	: IsKeyDown(KEY_S);

	// Right joystick
    this->vLeft = this->gamepadConnected ? 	(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X) < -0.3f)	: IsKeyDown(KEY_LEFT);
    this->vRight = this->gamepadConnected ? (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X) > 0.3f) 	: IsKeyDown(KEY_RIGHT);
    this->vUp = this->gamepadConnected ? 	(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y) < -0.3f) 	: IsKeyDown(KEY_UP);
    this->vDown = this->gamepadConnected ? 	(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y) > 0.3f) 	: IsKeyDown(KEY_DOWN);
	
    // Actions
	this->btnA = this->gamepadConnected ? 	IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) 	: IsKeyPressed(KEY_L);
	this->btnB = this->gamepadConnected ? 	IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) 	: IsKeyPressed(KEY_P);
    this->btnX = this->gamepadConnected ? 	IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) 	: IsKeyPressed(KEY_K);
	this->btnY = this->gamepadConnected ? 	IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP) 	: IsKeyPressed(KEY_O);
	
	// Triggers and back buttons
	this->btnL = this->gamepadConnected ? 	IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1) 		: IsKeyDown(KEY_Q);
	this->btnR = this->gamepadConnected ? 	IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) 		: IsKeyDown(KEY_E);
	this->tLeft = this->gamepadConnected ? 	IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2) 		: IsKeyDown(KEY_ONE);
	this->tRight = this->gamepadConnected ? IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) 		: IsKeyDown(KEY_THREE);
	
	// Main
	this->btnSEL = this->gamepadConnected ? IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_LEFT) 		: IsMouseButtonDown(KEY_BACKSPACE);
	this->btnSTR = this->gamepadConnected ? IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT) 		: IsMouseButtonDown(KEY_SPACE);
}

void InputManager::logInputState() {
	if (this->mUp || this->mRight || this->mDown || this->mLeft) DrawText("[move]", 10, 130, 20, GREEN);
	if (this->vUp || this->vRight || this->vDown || this->vLeft) DrawText("[view]", 10, 150, 20, BLUE);
	
	if (this->btnA) DrawText("[A]", 605, 110, 20, GREEN);
	if (this->btnB) DrawText("[B]", 605, 130, 20, RED);
	if (this->btnX) DrawText("[X]", 605, 150, 20, BLUE);
	if (this->btnY) DrawText("[Y]", 605, 170, 20, YELLOW);
	
	if (this->btnL) DrawText("[L]", 95, 15, 20, WHITE);
	if (this->btnR) DrawText("[R]", 480, 15, 20, WHITE);
	if (this->tLeft) DrawText("[L_TRIGGER]", 95, 35, 20, YELLOW);
	if (this->tRight) DrawText("[R_TRIGGER]", 480, 35, 20, YELLOW);
	
	if (this->btnSEL) DrawText("[select]", 100, 70, 20, WHITE);
	if (this->btnSTR) DrawText("[start]", 490, 70, 20, WHITE);
}