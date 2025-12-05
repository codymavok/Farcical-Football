/**
* Author: Karan Singh 
* Assignment: Farcical Football
* Date due: December 5th 2025, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "raylib.h"
#include <math.h>
#include "CS3113/cs3113.h"
#include "CS3113/Entity.h"
#include "CS3113/MainLevel.h"
#include "CS3113/StartingMenu.h"
#include "CS3113/GameOverScreen.h" 
#include "CS3113/ShaderProgram.h"


// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr Vector2 ORIGIN           = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };


constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

int gFinalPlayer1Score = 0;
int gFinalPlayer2Score = 0;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

StartingMenu *gMenu = nullptr;
MainLevel *gMainLevel = nullptr;
GameOverScreen *gGameOver = nullptr;

ShaderProgram gShader;
Vector2 gLightPosition = { 0.0f, 0.0f };

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
void switchToScene(Scene *scene);


void switchToScene(Scene *scene)
{   
    gCurrentScene = scene;
    gCurrentScene->initialise();
}


void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 5");
    InitAudioDevice();

    gMenu = new StartingMenu(ORIGIN, "Assets/8bit-pixel-art-space-landscape-600nw-2637792813.jpeg");
    gMainLevel = new MainLevel(ORIGIN, "Assets/8bit-pixel-art-space-landscape-600nw-2637792813.jpeg");
    gGameOver = new GameOverScreen(ORIGIN, "Assets/8bit-pixel-art-space-landscape-600nw-2637792813.jpeg");

    gLevels.push_back(gMenu);
    gLevels.push_back(gMainLevel);
    gLevels.push_back(gGameOver);
    switchToScene(gLevels[0]);
    gShader.load("shaders/vertex.glsl", "shaders/fragment.glsl");

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (gCurrentScene->getState().player1 != nullptr && 
        gCurrentScene->getState().player2 != nullptr)
    {
        gCurrentScene->getState().player1->resetMovement();
        gCurrentScene->getState().player2->resetMovement();

        if (IsKeyDown(KEY_A)) gCurrentScene->getState().player1->moveLeft();
        if (IsKeyDown(KEY_D)) gCurrentScene->getState().player1->moveRight();
        
        if (IsKeyPressed(KEY_W) && gCurrentScene->getState().player1->isCollidingBottom())
        {
            gCurrentScene->getState().player1->jump();
            // PlaySound(gCurrentScene->getState().jumpSound);
        }

        if (IsKeyDown(KEY_LEFT))  gCurrentScene->getState().player2->moveLeft();
        if (IsKeyDown(KEY_RIGHT)) gCurrentScene->getState().player2->moveRight();
        
        if (IsKeyPressed(KEY_UP) && gCurrentScene->getState().player2->isCollidingBottom())
        {
            gCurrentScene->getState().player2->jump();
            // PlaySound(gCurrentScene->getState().jumpSound);
        }

        if (GetLength(gCurrentScene->getState().player1->getMovement()) > 1.0f) 
            gCurrentScene->getState().player1->normaliseMovement();
            
        if (GetLength(gCurrentScene->getState().player2->getMovement()) > 1.0f) 
            gCurrentScene->getState().player2->normaliseMovement();
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) 
        gAppStatus = TERMINATED;
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
        if (gCurrentScene->getState().ball != nullptr)
        {
            gLightPosition = gCurrentScene->getState().ball->getPosition();
        }
    }
    
    gTimeAccumulator = deltaTime;
}

void render()
{
    BeginDrawing();
    ClearBackground(BLACK);

   
    if (gCurrentScene->getState().player1 != nullptr && 
        gCurrentScene->getState().player2 != nullptr)
    {
        gShader.begin();
        gShader.setVector2("lightPosition", gLightPosition);
        gCurrentScene->render();
        gShader.end();
    }
    else
    {
        gCurrentScene->render();
    }

    EndDrawing();
}

void shutdown() 
{
    delete gMenu;
    delete gMainLevel;
    delete gGameOver;
    gShader.unload();
    for (size_t i = 0; i < gLevels.size(); i++) 
        gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        
        if (gCurrentScene->getState().nextSceneID != 0)  
        {
            int id = gCurrentScene->getState().nextSceneID;

             if (id == 2)  {
                gFinalPlayer1Score = gCurrentScene->getState().player1Score;
                gFinalPlayer2Score = gCurrentScene->getState().player2Score;
                gGameOver->setFinalScores(gFinalPlayer1Score, gFinalPlayer2Score);
            }
    
            
            if (id > 0 && id < (int)gLevels.size())  
            {
                switchToScene(gLevels[id]);
            }
            else if (id == -1 || id == -2)  // Return to menu
            {
                switchToScene(gLevels[0]);
            }
        }
        
        render();
    }

    shutdown();
    return 0;
}