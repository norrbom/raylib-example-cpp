#include <iostream>
#include "raylib.h"

Camera camera = { 0 };
Vector3 cubePosition = { 0 };

// Splash
Font font;
Vector2 titlePos = { 0 };
Vector2 subTitlePos1 = { 0 };
Vector2 subTitlePos2 = { 0 };
Vector2 titleSize = { 0.0f, 0.0f };
Vector2 subTitleSize1 = { 0.0f, 0.0f };
Vector2 subTitleSize2 = { 0.0f, 0.0f };
const float fontSizeXL = 128.0f;
const float fontSizeL = 64.0f;
const char title[14] = "Vendel Period";
const char subTitle1[34] = "Scandinavia, a few dacades after ";
const char subTitle2[32] = "the collaps of the roman empire";
unsigned int splashSpeed = 1;
static const int FIRST = 5;
static const int SECOND = 10;
static const int THIRD = 15;
static const int FOURTH = 20;
static const int END = 30;
Music music;

// Model
Model model;
ModelAnimation anim;

static int DrawSplash(void);

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Vendel");

    // Splash
    font = LoadFontEx("resources/fonts/KAISG.ttf", (int)fontSizeXL, 0, 250);

    titleSize     = MeasureTextEx(font, title, fontSizeXL, 0);
    subTitleSize1 = MeasureTextEx(font, subTitle1, fontSizeL, 0);
    subTitleSize2 = MeasureTextEx(font, subTitle2, fontSizeL, 0);
    titlePos      = (Vector2){GetScreenWidth()/2 - titleSize.x/2, GetScreenHeight()/2 - (2 * titleSize.y/2)};
    subTitlePos1  = (Vector2){GetScreenWidth()/2 - subTitleSize1.x/2, GetScreenHeight()/2 + (2 * subTitleSize2.y/2)};
    subTitlePos2  = (Vector2){GetScreenWidth()/2 - subTitleSize2.x/2, GetScreenHeight()/2 + (4 * subTitleSize2.y/2)};

    InitAudioDevice();
    music = LoadMusicStream("resources/sound/dark.mp3");
    PlayMusicStream(music);

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 5.0f, 5.0f, 5.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Load gltf model
    model = LoadModel("resources/models/character.glb");
    // Load gltf model animations
    unsigned int animsCount = 0;
    unsigned int animIndex = 0;
    unsigned int animCurrentFrame = 0;
    ModelAnimation *modelAnimations = LoadModelAnimations("resources/models/character.glb", &animsCount);
    Vector3 position = (Vector3){ 0.0f, 0.0f, 0.0f };    // Set model position
    Vector3 scale = (Vector3){ 0.01f, 0.01f, 0.01f };
    Vector3 rotationAxis = (Vector3){ 1.0f, 0.0f, 0.0f };
    float rotationAngle = 90.0f;

    DisableCursor();                             // Limit cursor to relative movement inside the window

    SetTargetFPS(60);

    int splashDone = 0;

    // Main game loop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);

        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        // Select current animation
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) animIndex = (animIndex + 1)%animsCount;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) animIndex = (animIndex + animsCount - 1)%animsCount;

        // Update model animation
        anim = modelAnimations[animIndex];
        animCurrentFrame = (animCurrentFrame + 1)%anim.frameCount;
        UpdateModelAnimation(model, anim, animCurrentFrame);

        BeginDrawing();
            splashDone = 1;
            if (splashDone == 0) {
                splashDone = DrawSplash();
            }
            else {
                ClearBackground(BLACK);
                BeginMode3D(camera);
                    DrawModelEx(model, position, rotationAxis, rotationAngle, scale, WHITE);
                    std::cout << position.x;
                    DrawGrid(10, 1.0f);
                EndMode3D();
                DrawText("Use the LEFT/RIGHT mouse buttons to switch animation", 10, 10, 20, GRAY);
                DrawText(TextFormat("Animation: %d", anim.boneCount), 10, GetScreenHeight() - 20, 10, DARKGRAY);
            }
        EndDrawing();
    }

    // De-Initialization
    UnloadMusicStream(music);
    CloseAudioDevice();
    UnloadModel(model);
    CloseWindow();
    return 0;
}

static int DrawSplash(void)
{
    if (IsKeyDown(KEY_SPACE))
        splashSpeed++;
    double time = GetTime() * splashSpeed;

    ClearBackground(BLACK);
    DrawTextEx(font, title, titlePos, fontSizeXL, 0, (Color){ 190, 33, 55, 255 });

    if(time > END) {
        StopMusicStream(music);
        return 1;
    }

    if(time > FIRST && time <= SECOND) {
        DrawTextEx(font, subTitle1, subTitlePos1, fontSizeL, 0, (Color){ 190, 33, 55, 255*((time-FIRST)/FIRST) });
        DrawTextEx(font, subTitle2, subTitlePos2, fontSizeL, 0, (Color){ 190, 33, 55, 255*((time-FIRST)/FIRST) });
    }
    else if(time >= SECOND && time <= THIRD) {
        DrawTextEx(font, subTitle1, subTitlePos1, fontSizeL, 0, (Color){ 190, 33, 55, 255 });
        DrawTextEx(font, subTitle2, subTitlePos2, fontSizeL, 0, (Color){ 190, 33, 55, 255 });
    }
    else if(time > THIRD && time < FOURTH) {
        float transp = 255 - (time-THIRD)*51;
        DrawTextEx(font, subTitle1, subTitlePos1, fontSizeL, 0, (Color){ 190, 33, 55, transp });
        DrawTextEx(font, subTitle2, subTitlePos2, fontSizeL, 0, (Color){ 190, 33, 55, transp });
    }
    return 0;
}
