#include "Scene.h"

class GameOverScreen : public Scene 
{
private:
    int mFinalP1Score = 0;
    int mFinalP2Score = 0;

public:
    GameOverScreen();
    GameOverScreen(Vector2 origin, const char *bgTexture);
    ~GameOverScreen();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

    void setFinalScores(int p1Score, int p2Score) 
    { 
        mFinalP1Score = p1Score; 
        mFinalP2Score = p2Score; 
    }
};