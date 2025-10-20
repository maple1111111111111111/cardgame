#include "MainScene.h"

using namespace cocos2d;

Scene* MainScene::createScene() {
    return MainScene::create();
}

bool MainScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // 初始化MVC组件
    _gameModel = new GameModel();
    _gameView = GameView::create(_gameModel);
    _gameController = new GameController(_gameModel, _gameView);

    addChild(_gameView);

    // 开始游戏
    _gameController->startGame();

    return true;
}