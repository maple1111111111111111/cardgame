#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "cocos2d.h"
#include "Controller/GameController.h"
#include "Model/GameModel.h"
#include "View/GameView.h"

class MainScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    CREATE_FUNC(MainScene);

private:
    GameModel* _gameModel;
    GameView* _gameView;
    GameController* _gameController;
};

#endif // MAIN_SCENE_H