#include "MainScene.h"

using namespace cocos2d;

Scene* MainScene::createScene() {
    return MainScene::create();
}

bool MainScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // ��ʼ��MVC���
    _gameModel = new GameModel();
    _gameView = GameView::create(_gameModel);
    _gameController = new GameController(_gameModel, _gameView);

    addChild(_gameView);

    // ��ʼ��Ϸ
    _gameController->startGame();

    return true;
}