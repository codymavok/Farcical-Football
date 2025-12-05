#include "MainLevel.h"

MainLevel::MainLevel() : Scene{{0.0f, 0.0f}, nullptr} {}
MainLevel::MainLevel(Vector2 origin, const char *bgTexture) 
    : Scene{origin, bgTexture} {}

MainLevel::~MainLevel() { shutdown(); }

void MainLevel::initialise()
{
    mGameState.nextSceneID = 0;
    
    mMatchTimer = MATCH_DURATION;
    mPlayer1Score = 0;
    mPlayer2Score = 0;
    mKickCooldownP1 = 0.0f;
    mKickCooldownP2 = 0.0f;


    // Sound 
    mGameState.bgm = LoadMusicStream("Assets/Justice - Neverender (instrumental).mp3");
    SetMusicVolume(mGameState.bgm, 0.13f);
    PlayMusicStream(mGameState.bgm);
    mGameState.kickOffSound = LoadSound("Assets/Whistle Sound Effect!.mp3");
    mGameState.goalSound = LoadSound("Assets/Winning Sound Effect.mp3");
    mGameState.fullTimeSound = LoadSound("Assets/Coup de sifflet fin de match --- End of game whistle.mp3");
    mGameState.kickSound = LoadSound("Assets/Soccer ball kick [Sound Effects].mp3");

    // Player 1 Animation Sheet 
    std::map<Direction, std::vector<int>> player1AnimationAtlas = {
        {LEFT,  {5, 4, 3, 2, 1, 0}},   
        {RIGHT, {0, 1, 2, 3, 4, 5}},  
    };
    
    mPlayer1 = new Entity(
        {200.0f, 450.0f},                 
        {80.0f, 80.0f},                      // Scale
        "Assets/Pink_Monster_Run_6.png",    // Sprite sheet texture
        ATLAS,                               // Texture type 
        {1,6},                              // Sprite sheet dimensions 
        player1AnimationAtlas,               // Animation map
        PLAYER                               // Entity type
    );
    mPlayer1->setSpeed(250);
    mPlayer1->setJumpingPower(400.0f);
    mPlayer1->setAcceleration({0.0f, 800.0f});
    mPlayer1->setFrameSpeed(14); 
    mPlayer1->setColliderDimensions({60.0f, 65.0f});

    // Player 2 Animation Sheet 
    std::map<Direction, std::vector<int>> player2AnimationAtlas = {
        {LEFT,  {0, 1, 2, 3, 4, 5}},   
        {RIGHT, {5, 4, 3, 2, 1, 0}},  
    };
    
    mPlayer2 = new Entity(
        {800.0f, 450.0f},                 
        {80.0f, 80.0f},                      // Scale
        "Assets/Dude_Monster_Run_6.png",    // Sprite sheet texture
        ATLAS,                               // Texture type 
        {1,6},                              // Sprite sheet dimensions 
        player2AnimationAtlas,               // Animation map
        PLAYER                               // Entity type
    );
    mPlayer2->setSpeed(250);
    mPlayer2->setJumpingPower(400.0f);
    mPlayer2->setAcceleration({0.0f, 800.0f});
    mPlayer2->setFrameSpeed(14);  
    mPlayer2->setColliderDimensions({60.0f, 65.0f});
    
    // Ball 
    mBall = new Entity(
        {500.0f, 200.0f},
        {50.0f, 50.0f},
        "Assets/ball.png",
        PLATFORM  // can be changed later maybe 
    );
    mBall->setAcceleration({0.0f, 980.0f});
    mBall->setColliderDimensions({30.0f, 30.0f});

    mGround = new Entity(
        {500.0f, 590.0f},                    
        {1000.0f, 20.0f},                    
        "Assets/ball.png",                   
        PLATFORM
    );
    mGround->setAcceleration({0.0f, 0.0f});  
    // mGround->deactivate();                   
    
    // The first goal 
    mLeftGoal = new Entity(
        {50.0f, 500.0f},        
        {100.0f, 200.0f},       
        "Assets/goal1.png", 
        PLATFORM               
    );
    mLeftGoal->setAcceleration({0.0f, 0.0f});  
    // mLeftGoal->setColliderDimensions({20.0f, 145.0f});
    
    // The second goal 
    mRightGoal = new Entity(
        {950.0f, 500.0f},       
        {100.0f, 200.0f},       
        "Assets/goal2.png",
        PLATFORM
    );
    mRightGoal->setAcceleration({0.0f, 0.0f}); 
    
    mGameState.player1 = mPlayer1;
    mGameState.player2 = mPlayer2;
    mGameState.ball = mBall;
    mGameState.player1Score = mPlayer1Score;
    mGameState.player2Score = mPlayer2Score;
    mGameState.matchTimer = mMatchTimer;

    mPlayersAndBall[0] = mPlayer1;
    mPlayersAndBall[1] = mPlayer2;
    mPlayersAndBall[2] = mBall;

    // Star Animation Atlas 
    std::map<Direction, std::vector<int>> starAnimationAtlas = {
        {LEFT,  {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}},   
        {RIGHT, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},  
    };
    
  
    mGlowingStar = new Entity(
        {799.0f, 60.0f},                 
        {80.0f, 80.0f},                      // Scale
        "Assets/Star.png",                  // Sprite sheet texture
        ATLAS,                               // Texture type 
        {1,13},                              // Sprite sheet dimensions 
        starAnimationAtlas,               // Animation map
        NPC                               // Entity type
    );
    mGlowingStar->setAcceleration({0.0f, 0.0f});  // No gravity
    mGlowingStar->setAIType(WANDERER);
    mGlowingStar->setSpeed(10.0f);


    // Falling star Animation Atlas 
    std::map<Direction, std::vector<int>> fallingStarAnimationAtlas = {   
        {LEFT, {0, 1, 2, 3, 4, 5, 6, 7, 8}} ,
        {RIGHT, {0, 1, 2, 3, 4, 5, 6, 7, 8}} 
    };
    
  
    mFallingStar = new Entity(
        {500.0f, 100.0f},                 
        {80.0f, 80.0f},                      // Scale
        "Assets/FallingStar_Sprites.png",    // Sprite sheet texture
        ATLAS,                               // Texture type 
        {1,9},                              // Sprite sheet dimensions 
        fallingStarAnimationAtlas,               // Animation map
        NPC                               // Entity type
    );
    mFallingStar->setAcceleration({-1000.0f, 0.0f});  // No gravity
    mFallingStar->setAIType(WANDERER);
    mFallingStar->setSpeed(10.0f);


    mSpaceShip = new Entity(
        {500.0f, 150.0f},                 
        {80.0f, 80.0f},                      // Scale
        "Assets/spaceship-in-pixel-art-style-vector Background Removed.png",  // Sprite sheet texture
        NPC                               // Entity type
    );
    mSpaceShip->setAcceleration({0.0f, 0.0f});  // No gravity
    mSpaceShip->setAIType(FOLLOWER);
    mSpaceShip->setSpeed(10.0f);
    
    PlaySound(mGameState.kickOffSound);
    mGoalsShrunk = false;
}

void MainLevel::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);
    
    // Update timer
    mMatchTimer -= deltaTime;
    mGameState.matchTimer = mMatchTimer;
    

    // I added this to prevent jittering since without a cooldown it looks like the ball is being kicked multiple times
    if (mKickCooldownP1 > 0.0f)
        mKickCooldownP1 -= deltaTime;

    if (mKickCooldownP2 > 0.0f)
        mKickCooldownP2 -= deltaTime;

    if (mMatchTimer <= 30.0f && !mGoalsShrunk) {
        mGoalsShrunk = true;
        mLeftGoal->setColliderDimensions({20.0f, 100.0f});
        mRightGoal->setColliderDimensions({20.0f, 100.0f});
        mLeftGoal->setScale({100.0f, 140.0f});  
        mRightGoal->setScale({100.0f, 140.0f});
    }
    
    if (mMatchTimer <= 0.0f || IsKeyDown(KEY_P))
    {
        mGameState.nextSceneID = 2;
        PlaySound(mGameState.fullTimeSound);
        return;
    }
    
    updateBallPhysics();

    Entity *player1Collidables[5] = {mPlayer2, mGround, mLeftGoal, mRightGoal};
    mPlayer1->update(deltaTime, nullptr, nullptr, player1Collidables, 4);

    Entity *player2Collidables[5] = {mPlayer1, mGround, mLeftGoal, mRightGoal};
    mPlayer2->update(deltaTime, nullptr, nullptr, player2Collidables, 4);

    Entity *ballCollidables[3] = {mGround};
    mBall->update(deltaTime, nullptr, nullptr, ballCollidables, 1);

    if (mKickCooldownP1 <= 0.0f){
        if (mPlayer1->isColliding(mBall) && IsKeyDown(KEY_F)){
            ballLobbed(mPlayer1);
            mKickCooldownP1 = KICK_COOLDOWN_TIME;
            }
            else if (mPlayer1->isColliding(mBall))
            {
                ballKicked(mPlayer1); 
                mKickCooldownP1 = KICK_COOLDOWN_TIME;  
            }
    }

    if (mKickCooldownP2 <= 0.0f){
        if (mPlayer2->isColliding(mBall) && IsKeyDown(KEY_SLASH)){
            ballLobbed(mPlayer2);
            mKickCooldownP2 = KICK_COOLDOWN_TIME;
            }
            else if (mPlayer2->isColliding(mBall))
            {
                ballKicked(mPlayer2); 
                mKickCooldownP2 = KICK_COOLDOWN_TIME;  
            }
    }

    mGlowingStar->update(deltaTime, nullptr, nullptr, nullptr, 0);
    mFallingStar->update(deltaTime, nullptr, nullptr, nullptr, 0);
    mSpaceShip->update(deltaTime, mGameState.player1, nullptr, 
        nullptr, 0);
    checkGoalScored();
    checkBallOutOfBounds();
}

void MainLevel::render()
{
      DrawTexturePro(
        mBackgroundTexture,
        {0, 0, (float)mBackgroundTexture.width, (float)mBackgroundTexture.height},  
        {0, 0, 1000, 600},  
        {0, 0},             
        0.0f,               
        WHITE               
    );
    
    mLeftGoal->render();
    mRightGoal->render();
    
    mPlayer1->render();
    mPlayer2->render();
    mBall->render();
    mGlowingStar->render();
    mFallingStar->render();
    mSpaceShip->render();
    
    // Draw scores and timer
    char scoreText[64];
    snprintf(scoreText, sizeof(scoreText), "P1: %d  TIME: %.0f  P2: %d", 
             mPlayer1Score, mMatchTimer, mPlayer2Score);
    DrawText(scoreText, 300, 20, 30, WHITE);

}

void MainLevel::checkGoalScored()
{
      Vector2 ballPos = mBall->getPosition();

    if (mBall->isColliding(mLeftGoal) && ballPos.x > mLeftGoal->getPosition().x)
    {
        mPlayer2Score++;  
        mGameState.player2Score = mPlayer2Score;
        PlaySound(mGameState.goalSound);
        resetBallPosition();
        resetPlayerPosition();
    }
    
    if (mBall->isColliding(mRightGoal) && ballPos.x < mRightGoal->getPosition().x)
    {
        mPlayer1Score++;  
        mGameState.player1Score = mPlayer1Score;
        PlaySound(mGameState.goalSound);
        resetBallPosition();
        resetPlayerPosition();
    }
}

void MainLevel::ballKicked(Entity *player)
{
    Vector2 playerVel = player->getVelocity();
    Vector2 playerPos = player->getPosition();
    Vector2 ballPos = mBall->getPosition();
    
    Vector2 kickDirection = {
        ballPos.x - playerPos.x,
        ballPos.y - playerPos.y
    };
    
    float length = sqrt(kickDirection.x * kickDirection.x + 
                       kickDirection.y * kickDirection.y);
    if (length > 0)
    {
        kickDirection.x /= length;
        kickDirection.y /= length;
    }

    float kickStrength = sqrt(playerVel.x * playerVel.x + playerVel.y * playerVel.y);
    
    kickStrength = fmaxf(kickStrength, 200.0f);  
    
    Vector2 playerMovement = player->getMovement();
    if (GetLength(playerMovement) > 0)
    {
        kickDirection.x = kickDirection.x * 0.6f + playerMovement.x * 0.4f;
        kickDirection.y = kickDirection.y * 0.6f + playerMovement.y * 0.4f;
        
        float newLength = sqrt(kickDirection.x * kickDirection.x + 
                              kickDirection.y * kickDirection.y);
        if (newLength > 0)
        {
            kickDirection.x /= newLength;
            kickDirection.y /= newLength;
        }
    }

    Vector2 newBallVelocity = {
        kickDirection.x * kickStrength * 1.2f,  
        kickDirection.y * kickStrength * 1.2f
    };
    
    mBall->setVelocity(newBallVelocity);
    PlaySound(mGameState.kickSound);
}



void MainLevel::ballLobbed(Entity *player){
    Vector2 currVelocity = mBall->getVelocity();
    Vector2 playerPos = player->getPosition();
    Vector2 ballPos = mBall->getPosition();

    float xDirection = 0.0f;
    if (ballPos.x > playerPos.x)
            xDirection = 1.0f;   
    else
        xDirection = -1.0f;  
        
    Vector2 newBallVelocity = {
        xDirection * 150.0f,  
        -250.0f               
        };
mBall->setVelocity(newBallVelocity);

}


void MainLevel::updateBallPhysics()
{
    if (mMatchTimer > 90.0f)
    {
        mBall->setAcceleration({0.0f, 980.0f});  
    }
    else if (mMatchTimer > 40.0f)
    {
        mBall->setAcceleration({0.0f, 490.0f}); 
    }
    else
    {
        mBall->setAcceleration({0.0f, 200.0f});  
    }
}

void MainLevel::resetBallPosition()
{
    mBall->setPosition({500.0f, 200.0f});
    mBall->setVelocity({0.0f, 0.0f});
}

void MainLevel::resetPlayerPosition()
{
    mPlayer1->setPosition({200.0f, 450.0f});
    mPlayer2->setPosition({800.0f, 450.0f});
}


void MainLevel::checkBallOutOfBounds()
{
    Vector2 ballPos = mBall->getPosition();
    

    if (ballPos.x < -50.0f ||   
        ballPos.x > 1050.0f || 
        ballPos.y < -50.0f ||   
        ballPos.y > 650.0f)    
    {
        resetBallPosition();
    }
}

void MainLevel::shutdown()
{
    delete mPlayer1;
    delete mPlayer2;
    delete mBall;
    delete mLeftGoal;   
    delete mRightGoal;  
    delete mGround;
    delete mGlowingStar;
    delete mFallingStar;
    delete mSpaceShip;

    UnloadTexture(mBackgroundTexture);
    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.kickOffSound);
    UnloadSound(mGameState.goalSound);
    UnloadSound(mGameState.fullTimeSound);
    UnloadSound(mGameState.kickSound);
    
    // UnloadSound(mGameState.kickSound);
    // UnloadSound(mGameState.goalSound);
    // UnloadMusicStream(mGameState.bgm);
    // UnloadTexture(mBackgroundTexture);
}

   
