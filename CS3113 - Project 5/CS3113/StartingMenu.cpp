#include "StartingMenu.h"

StartingMenu::StartingMenu() : Scene{{0.0f}, nullptr} {}
StartingMenu::StartingMenu(Vector2 origin, const char *bgTexture) : Scene{origin, bgTexture} {}

StartingMenu::~StartingMenu() { shutdown(); }

void StartingMenu::initialise()
{
    mGameState.nextSceneID = 0;  
    
    mText = GetFontDefault();

    mGameState.bgm = LoadMusicStream("Assets/Justice - Neverender (instrumental).mp3");
    SetMusicVolume(mGameState.bgm, 0.4f);
    PlayMusicStream(mGameState.bgm);
    
    mGameState.player1 = nullptr;
    mGameState.player2 = nullptr;
    mGameState.ball = nullptr;
    mGameState.map = nullptr;

     for (int i = 0; i < 3; i++){
        mGameState.ai[i] = nullptr;
     }

}

void StartingMenu::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        mGameState.nextSceneID = 1;  // Go to the main game level
    }

    UpdateMusicStream(mGameState.bgm);
}

void StartingMenu::render() {

     DrawTexturePro(
        mBackgroundTexture,
        {0, 0, (float)mBackgroundTexture.width, (float)mBackgroundTexture.height},  
        {0, 0, 1000, 600},  
        {0, 0},             
        0.0f,               
        WHITE               
    );
    
    const char* title = "FARCICAL FOOTBALL";  
    int titleFontSize = 60;
    Vector2 titleSize = MeasureTextEx(mText, title, titleFontSize, 2);
    float titleX = (1000 - titleSize.x) / 2.0f; 
    float titleY = 150.0f;
    
    DrawTextEx(mText, title, {titleX + 3, titleY + 3}, titleFontSize, 2, BLACK);
    DrawTextEx(mText, title, {titleX, titleY}, titleFontSize, 2, GREEN);

    const char* p1Controls = "Player 1: W/A/S/D to move, F to lob ball";
    int controlsFontSize = 20;
    Vector2 p1Size = MeasureTextEx(mText, p1Controls, controlsFontSize, 2);
    float p1X = (1000 - p1Size.x) / 2.0f;
    DrawTextEx(mText, p1Controls, {p1X, 280.0f}, controlsFontSize, 2, LIGHTGRAY);

    const char* p2Controls = "Player 2: Arrow keys to move, / to lob ball";
    Vector2 p2Size = MeasureTextEx(mText, p2Controls, controlsFontSize, 2);
    float p2X = (1000 - p2Size.x) / 2.0f;
    DrawTextEx(mText, p2Controls, {p2X, 320.0f}, controlsFontSize, 2, LIGHTGRAY);

    const char* instruction = "PRESS ENTER TO START";
    int instructionFontSize = 25;
    Vector2 instructionSize = MeasureTextEx(mText, instruction, instructionFontSize, 2);
    float instructionX = (1000 - instructionSize.x) / 2.0f;
    DrawTextEx(mText, instruction, {instructionX, 420.0f}, instructionFontSize, 2, WHITE);
    
    const char* credits = "By: Karan Singh";
    int creditsFontSize = 20;
    Vector2 creditsSize = MeasureTextEx(mText, credits, creditsFontSize, 1);
    float creditsX = (1000 - creditsSize.x) / 2.0f;
    DrawTextEx(mText, credits, {creditsX, 550.0f}, creditsFontSize, 1, LIGHTGRAY);
}

void StartingMenu::shutdown()
{
  UnloadTexture(mBackgroundTexture);
  UnloadMusicStream(mGameState.bgm);
}