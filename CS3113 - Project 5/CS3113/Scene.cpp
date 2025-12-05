#include "Scene.h"

Scene::Scene() : mOrigin{{}} {}

Scene::Scene(Vector2 origin, const char *bgTextureFilepath) : mOrigin{origin}
{
    mBackgroundTexture = LoadTexture(bgTextureFilepath);
}