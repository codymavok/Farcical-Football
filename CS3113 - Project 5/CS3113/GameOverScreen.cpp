#include "GameOverScreen.h"

GameOverScreen::GameOverScreen() : Scene{{0.0f, 0.0f}, nullptr} {}
GameOverScreen::GameOverScreen(Vector2 origin, const char *bgTexture) 
    : Scene{origin, bgTexture} {}

GameOverScreen::~GameOverScreen() { shutdown(); }

void GameOverScreen::initialise()
{
    mGameState.nextSceneID = 0;
    
    mGameState.player1 = nullptr;
    mGameState.player2 = nullptr;
    mGameState.ball = nullptr;
    mGameState.map = nullptr;
    
    for (int i = 0; i < 3; i++)
        mGameState.ai[i] = nullptr;

    mGameState.bgm = LoadMusicStream("Assets/Justice - Neverender (instrumental).mp3");
    SetMusicVolume(mGameState.bgm, 0.4f);
    PlayMusicStream(mGameState.bgm);
}

void GameOverScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        mGameState.nextSceneID = -2;  // Return to menu
    }

    UpdateMusicStream(mGameState.bgm);
}

void GameOverScreen::render()
{
      DrawTexturePro(
        mBackgroundTexture,
        {0, 0, (float)mBackgroundTexture.width, (float)mBackgroundTexture.height},  
        {0, 0, 1000, 600},  
        {0, 0},             
        0.0f,               
        WHITE               
    );
    
    
    if (mFinalP1Score > mFinalP2Score)
    {
        const char* result = "Player 1 Wins!";
        int textWidth = MeasureText(result, 50);
        DrawText(result, (1000 - textWidth) / 2, 250, 50, GREEN);
    }
    else if (mFinalP2Score > mFinalP1Score)
    {
        const char* result = "Player 2 Wins!";
        int textWidth = MeasureText(result, 50);
        DrawText(result, (1000 - textWidth) / 2, 250, 50, BLUE);
    }
    else
    {
        const char* result = "Match Drawn.";
        int textWidth = MeasureText(result, 50);
        DrawText(result, (1000 - textWidth) / 2, 250, 50, YELLOW);
    }

    const char* instruction = "Press ENTER to return to menu";
    int instrWidth = MeasureText(instruction, 20);
    DrawText(instruction, (1000 - instrWidth) / 2, 500, 20, LIGHTGRAY);
}

void GameOverScreen::shutdown()
{
    UnloadTexture(mBackgroundTexture);
    UnloadMusicStream(mGameState.bgm);
}