#include "Entity.h"

#ifndef SCENE_H
#define SCENE_H

struct GameState
{
    Entity *player1;
    Entity *player2;
    Entity *ball;
    Entity *ai[3];
    Map *map;

    Music bgm;
    Sound kickSound;
    Sound walkSound;
    Sound kickOffSound;
    Sound goalSound;
    Sound fullTimeSound;

    Camera2D camera;

    int player1Score;
    int player2Score;
    float matchTimer;

    int nextSceneID;
};

class Scene 
{
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode = "#000000";

    Texture2D mBackgroundTexture; 
    
public:
    Scene();
    Scene(Vector2 origin, const char *bgTextureFilepath);

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;
    
    GameState   getState()           const { return mGameState; }
    Vector2     getOrigin()          const { return mOrigin;    }
    Texture2D getBackgroundTexture()  const { return mBackgroundTexture; }
};

#endif