#include "Scene.h"

class StartingMenu : public Scene 
{
private:
    Font mText;
    bool mShowText = true;

public:
    StartingMenu();
    StartingMenu(Vector2 origin, const char *bgTextureFilepath);
    ~StartingMenu();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};