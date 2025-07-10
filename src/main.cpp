#define _GNU_SOURCE

#include "raylib.h"
#include "raymath.h"
#include "cJSON.h"
#include "Player.h"
#include "InputManager.h"
#include "CameraManager.h"
#include <stdio.h>
#include <cstring>

Player* player;

Model LoadTexturedPrimitive(Mesh mesh, const char *textureName) {
    Model model = LoadModelFromMesh(mesh);       		// Create model from mesh
	
	char* pathFile;
	asprintf(&pathFile, "%s%s%s", "assets/3d-text/", textureName, ".png");
	Image image = LoadImage(pathFile);        			// Load image from file
	
	ImageFlipVertical(&image);                   		// Flip vertically to match OpenGL UVs
	Texture2D texture = LoadTextureFromImage(image);	// Generate the texture from the inverted Image
    UnloadImage(image);                          		// Unload the image, it's no longer needed after texture is initialized
    
	SetTextureFilter(texture, TEXTURE_FILTER_POINT);    // Keep pixel perfect style
	SetTextureWrap(texture, TEXTURE_WRAP_REPEAT);		// Wrap the texture (tiles)

	// Additional logic for transparency:
	SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE; // Includes alpha
	
    return model;
}

Model CreateTexturedCube(Vector3 size, const char* textureName) {
	return LoadTexturedPrimitive(GenMeshCube(size.x, size.y, size.z), textureName);
}

Model CreateTexturedPlane(Vector2 size, bool tiled, const char* textureName) {
	// if marked as so, we will automatically tile our plane into 1x1 sized units
	Mesh mesh = GenMeshPlane(size.x, size.y, 1, 1);
	if (tiled) {
		for (int i = 0; i < mesh.vertexCount; i++) {
			mesh.texcoords[i * 2]     *= size.x; // U
			mesh.texcoords[i * 2 + 1] *= size.y; // V
		}
		UploadMesh(&mesh, false);
	}
	
	return LoadTexturedPrimitive(mesh, textureName);
}

Model GenerateObjWithTexture(const char* objName, const char* textureName) {
	char* objFile; char* pngFileName;
	
	asprintf(&objFile, "%s%s%s", "assets/3d-model/", objName, ".obj");
	asprintf(&pngFileName, "%s%s%s", "assets/3d-text/", textureName, ".png");
	
	Model model = LoadModel(objFile);
	
	// Manually assign the texture (just in case the .mtl is ignored)
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture(pngFileName);
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
	
	return model;
}

Model GenerateSkyboxWithTexture(const char* textureName) {
	return GenerateObjWithTexture("skybox", textureName);
}

void Preload() {
	
    TraceLog(LOG_INFO, "Preloading game settings...");
	
	// Init models
	InputManager* input = new InputManager();
	CameraManager* cameraManager = new CameraManager(input);
	player = new Player(input, cameraManager);
	
	// Rendering common behavior configuration
	rlDisableBackfaceCulling();		// Disable culling, globally
	rlEnableColorBlend();          	// Enable blending
	rlSetBlendMode(BLEND_ALPHA);    // Use alpha blend mode
	
}

int main(void) {
	
    // Initialize window
    InitWindow(640, 576, "Dark Souls Demake");
    SetTargetFPS(60);
	
    Preload();  // Run before main loop
	
	// Generate level
	Model skybox = GenerateSkyboxWithTexture("skybox");
	Vector3 cubePos = (Vector3) { 1.0f, .5f, 1.0f };
    Model cubeModel = CreateTexturedCube({ 1.0f, 1.0f, 1.0f }, "wall-test");
	Model planeModel = CreateTexturedPlane({ 1.0f, 1.0f }, true, "floor-test");
	
	Vector3 enemyDefaultPos = (Vector3) { 1, 0, -3 };
	
	Model enemyModel = GenerateObjWithTexture("enemy-plane", "wip/enemy");
	//enemyModel.transform =  MatrixRotateX(-PI / 2.0f); // rotate it 90 degrees, to make it stand vertically

	// Collisions init data
	int levelCount = 1;
	LevelObject levelObj = {
		.position = cubePos,
		.size = Vector3One()
	};
	
    // Main game loop
    while (!WindowShouldClose()) {
		player->getInput()->updateInputManager();
		
        //UpdateCamera(&cam, CAMERA_ORBITAL); // Orbiting mode
		player->inputVelocity();
		player->updatePVA(levelCount, levelObj);
		
		// "Billboard" models that need it
		player->getCam()->billboardModel(&enemyModel, { enemyDefaultPos.x, enemyDefaultPos.z });
		
		BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(player->getCam()->getCam());
			
			// (#1) Skybox
			player->getCam()->RenderSkybox(skybox);
			
			// (#2) Level building
			DrawModel(cubeModel, cubePos, 1.0f, WHITE);  // Draw cube
			for (float i=0.0f; i<10.0f; i++) {
				for (float j=0.0f; j<10.0f; j++) {
					DrawModel(planeModel, (Vector3) { i - 5.0f, 0.0f, j - 5.0f }, 1.0f, WHITE);	// Draw Plane (floor)
				}
			}
			
			// (#3) Transparent meshes (because we need to disable depth mask for them)
			
			rlDisableDepthMask(); // mandatory for transparent textures
				DrawModel(enemyModel, enemyDefaultPos, 1.0f, WHITE);
			rlEnableDepthMask();
			
			
			// DEBUGGING RENDER
            //DrawGrid(10, 1.0f);  // Optional ground grid			
			//DrawLine3D((Vector3){0, 0, 0}, (Vector3){1, 0, 0}, RED);    // X (right)
			//DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 1, 0}, GREEN);  // Y (up)
			//DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 0, 1}, BLUE);   // Z (backward)
			
			//DrawSphereWires(cameraManager.getCam().position, cameraManager.radius, 8, 8, RED);
			
        EndMode3D();
		//logInputState(input);
		
        EndDrawing();
    }
	
    // Cleanup
    UnloadModel(cubeModel);
	UnloadModel(skybox);
	UnloadModel(enemyModel);
	UnloadModel(planeModel);
	
    CloseWindow();
	
    return 0;
}
