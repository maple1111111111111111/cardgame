#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Model/GameModel.h"
#include "View/GameView.h"

class GameController {
public:
    GameController(GameModel* model, GameView* view);

    void startGame();

private:
    GameModel* _model;
    GameView* _view;

    // 处理手牌点击
    void onHandCardClicked(int index);

    // 处理桌面牌点击
    void onTableCardClicked(int index);

    // 处理回退按钮点击
    void onUndoClicked();
};

#endif // GAME_CONTROLLER_H