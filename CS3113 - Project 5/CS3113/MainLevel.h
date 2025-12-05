#include "Scene.h"

class MainLevel : public Scene 
{
private:
    Entity *mPlayer1;  
    Entity *mPlayer2; 
    Entity *mBall;    
    Entity *mGround; 
    
    Entity *mLeftGoal;   
    Entity *mRightGoal;  
    
    Entity *mSpaceShip;
    Entity *mGlowingStar;
    Entity *mFallingStar;
    
    float mMatchTimer;
    int mPlayer1Score;
    int mPlayer2Score;

    float mKickCooldownP1;
    float mKickCooldownP2;
    Entity *mPlayersAndBall[3];
    
    void checkGoalScored();
    void updateBallPhysics();
    void resetBallPosition();
    void resetPlayerPosition();
    void ballKicked(Entity *player);
    void ballLobbed(Entity *player);
    bool mGoalsShrunk = false;


    void checkBallOutOfBounds();

public:
    static constexpr float MATCH_DURATION = 120.0f;  
    static constexpr float KICK_FORCE = 200.0f; // Need to test to see how hard we want the player to kick 
    static constexpr float KICK_COOLDOWN_TIME = 0.05f; // Added this and mKickCooldown because the ball was being collided with too many times
    
    MainLevel();
    MainLevel(Vector2 origin, const char *bgTexture);
    ~MainLevel();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};