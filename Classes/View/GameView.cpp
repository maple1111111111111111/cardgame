#include "GameView.h"

using namespace cocos2d;

GameView* GameView::create(GameModel* model) {
    GameView* pRet = new(std::nothrow) GameView();
    if (pRet && pRet->init(model)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool GameView::init(GameModel* model) {
    if (!Layer::init()) {
        return false;
    }

    _model = model;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    float tableHeight = visibleSize.height * 0.7f;  
    float handHeight = visibleSize.height * 0.3f;  



    _tableArea = Node::create();
    _tableArea->setAnchorPoint(Vec2(0, 0));  // 锚点设为左下角
    _tableArea->setPosition(origin.x, origin.y + handHeight);
    _tableArea->setContentSize(Size(visibleSize.width, tableHeight));  // 明确区域尺寸
    addChild(_tableArea);
    // 桌牌区背景 锚点(0,0)
    auto tableBg = LayerColor::create(
        Color4B(169, 121, 68, 200),  // 棕色
        visibleSize.width,           
        tableHeight                   
    );
    tableBg->setAnchorPoint(Vec2(0, 0));  // 锚点左下角
    tableBg->setPosition(0, 0);           
    _tableArea->addChild(tableBg, -1);    // 背景在底层


    // 锚点(0,0)，左下角坐标(0, 0)
    _handArea = Node::create();
    _handArea->setAnchorPoint(Vec2(0, 0));  // 锚点设为左下角
    _handArea->setPosition(origin.x, origin.y);  
    _handArea->setContentSize(Size(visibleSize.width, handHeight)); 
    addChild(_handArea);

    // 手牌区背景 锚点(0,0)
    auto handBg = LayerColor::create(
        Color4B(154, 32, 134, 200),  // 紫色
        visibleSize.width,            
        handHeight                   
    );
    handBg->setAnchorPoint(Vec2(0, 0));  // 锚点左下角
    handBg->setPosition(0, 0);           
    _handArea->addChild(handBg, -1);     // 背景在底层


    //// 回退按钮
    //_undoButton = MenuItemImage::create(
    //    "undo_normal.png",
    //    "undo_selected.png",
    //    [this](Ref* sender) {
    //        if (_onUndoClicked) {
    //            _onUndoClicked();
    //        }
    //    }
    //);
   // —— 回退按钮（放在 handBg 添加之后） ——
    _undoButton = MenuItemImage::create(
        "undo_normal.png",
        "undo_selected.png",
        [this](Ref*) { if (_onUndoClicked) _onUndoClicked(); }
    );

    // 目标像素大小（改这里即可）
    const float kUndoTargetPx = 64.0f;                 // 48/56/64 都行
    const Size tex = _undoButton->getContentSize();
    const float scale = kUndoTargetPx / std::max(tex.width, tex.height);
    _undoButton->setScale(scale);

    // 把 Menu 加到“手牌区”坐标系，便于自适应
    auto menu = Menu::create(_undoButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    menu->setLocalZOrder(10000);
    _handArea->addChild(menu, 10000);

    // 右上角对齐（不与牌重叠）
    const Size handSz = _handArea->getContentSize();
    const float kMargin = 12.0f;
    _undoButton->setAnchorPoint(Vec2(1.f, 1.f));
    _undoButton->setPosition(handSz.width - kMargin, handSz.height - kMargin);



    //auto menu = Menu::create(_undoButton, nullptr);
    // —— 回退按钮（放在 handBg 添加之后） ——
    //_undoButton = MenuItemImage::create(
    //    "undo_normal.png",
    //    "undo_selected.png",
    //    [this](Ref*) { if (_onUndoClicked) _onUndoClicked(); }
    //);

    //// 1) 用“目标像素尺寸”控制按钮大小（调这里即可）
    //const float kUndoTargetPx = 56.0f;           // ← 想大用 64/72，想小用 48
    //const auto  tex = _undoButton->getContentSize();
    //const float scale = kUndoTargetPx / std::max(tex.width, tex.height);
    //_undoButton->setScale(scale);

    //// 2) 锚点与位置：挂在“手牌区”右侧中部（自适应分辨率）
    //_undoButton->setAnchorPoint(cocos2d::Vec2(1.f, 0.5f));
    //const float kMargin = 12.0f;                  // ← 右边距（像素）
    //const auto  handSz = _handArea->getContentSize();

    //// 用 Menu 承载，但把 Menu 放在 handArea 内部坐标系(0,0)
    //auto menu = Menu::create(_undoButton, nullptr);
    //menu->setPosition(cocos2d::Vec2::ZERO);
    //menu->setLocalZOrder(10000);
    //_handArea->addChild(menu, 10000);

    //// 设置按钮在 handArea 内的最终坐标（右侧中部）
    //_undoButton->setPosition(handSz.width - kMargin, handSz.height * 0.5f);

    /*menu->setPosition(origin.x + visibleSize.width - 50, origin.y + handHeight - 50);
    addChild(menu, 10); */

    return true;
}



void GameView::updateView() {
    // 记录当前所有牌的位置
    _cardPositions.clear();
    for (int i = 0; i < _tableCardViews.size(); ++i) {
        CardView* card = _tableCardViews[i];
        if (card) {
            _cardPositions[getCardUniqueId(card->getCard())] = card->getPosition();
        }
    }
    // 收集当前手中的牌及其位置
    for (int i = 0; i < _handCardViews.size(); ++i) {
        CardView* card = _handCardViews[i];
        if (card) {
            _cardPositions[getCardUniqueId(card->getCard())] = card->getPosition();
        }
    }

    // 获取新的牌数据
    const auto& newTableCards = _model->getTableCards();
    const auto& newHandCards = _model->getHandCards();

    // 创建新的牌视图并添加到场景
    std::vector<CardView*> newTableCardViews;
    std::vector<CardView*> newHandCardViews;
    //根据新model更新新views
    handleTableCardViews(newTableCardViews);
    handleHandCardViews(newHandCardViews);


    // 移除不再需要的牌
    auto removeUnusedCards = [](std::vector<CardView*>& cards) {
        for (auto card : cards) {
            if (card) {
                // 淡出动画后移除
                card->runAction(Sequence::create(
                    FadeOut::create(0.2f),
                    RemoveSelf::create(),
                    nullptr
                ));
            }
        }
        cards.clear();
    };

    removeUnusedCards(_tableCardViews);
    removeUnusedCards(_handCardViews);

    // 更新牌视图列表
    _tableCardViews = newTableCardViews;
    _handCardViews = newHandCardViews;
}

void GameView::layoutTableCards() {
    int count = _tableCardViews.size(); if (count == 0) return;
    Size tableSize = _tableArea->getContentSize();
    float centerX = tableSize.width / 2; // 桌牌区中心 X
    float bottomY = 250; 
    float splitOffset = 150; 
    float leftBottomX = centerX - splitOffset; 
    float rightBottomX = centerX + splitOffset; 
    float topLeftStartX = 200; 
    float topLeftStartY = tableSize.height - 200;
    float topRightStartX = tableSize.width - 200; 
    float topRightStartY = tableSize.height - 200; 
    int leftCount = (count + 1) / 2; 
    int rightCount = count / 2; 
    // 左堆
    for (int i = 0; i < leftCount; ++i) {
        CardView* card = _tableCardViews[i];
        if (!card) continue;
        float progress = (leftCount == 1) ? 0.5f : static_cast<float>(i) / (leftCount - 1);
        float x = topLeftStartX + (leftBottomX - topLeftStartX) * progress;
        float y = topLeftStartY - (topLeftStartY - bottomY) * progress;
        float rotation = 0 - 20 * progress;
        // 设置位置、旋转和层级
        card->setPosition(x, y);
        card->setRotation(rotation);
        card->setLocalZOrder(i);
    }
    // 右堆
    for (int i = 0; i < rightCount; ++i) {
        int cardIndex = leftCount + i; // 右堆卡片索引
        CardView* card = _tableCardViews[cardIndex];
        if (!card) continue;
        float progress = (rightCount == 1) ? 0.5f : static_cast<float>(i) / (rightCount - 1);
        float x = topRightStartX - (topRightStartX - rightBottomX) * progress;
        float y = topRightStartY - (topRightStartY - bottomY) * progress;
        float rotation = 0 + 20 * progress;
        // 设置位置、旋转和层级
        card->setPosition(x, y);
        card->setRotation(rotation);
        card->setLocalZOrder(i + leftCount); 
    }
}
void GameView::layoutHandCards() {
    int count = _handCardViews.size();
    if (count == 0) return;

    // 获取手牌区尺寸
    Size handSize = _handArea->getContentSize();

    float cardWidth = 100;
    float spacing = 20;
    float totalCardsWidth = (count - 1) * spacing + count * cardWidth;

    // 水平居中排列
    float startX = (handSize.width - totalCardsWidth) / 2;
    float centerY = handSize.height / 2;  // 垂直居中

    for (int i = 0; i < count-1; ++i) {
        CardView* card = _handCardViews[i];
        float x = startX + i * (cardWidth + spacing);
        card->setPosition(x, centerY);
    }
    //即将匹配的手牌
    CardView* lastCard = _handCardViews[count - 1];
    float x = startX + (count - 1 + 2) * (cardWidth + spacing);
    lastCard->setPosition(x, centerY);
}

std::string GameView::getCardUniqueId(const Card& card) {
    return card.getSuitName() + "_" + card.getNumberName();
}

Vec2 GameView::getTargetPositionForTableCard(int index) {
    Size tableSize = _tableArea->getContentSize();
    float centerX = tableSize.width / 2;
    float bottomY = 250;
    float splitOffset = 150;
    float leftBottomX = centerX - splitOffset;
    float rightBottomX = centerX + splitOffset;
    float topLeftStartX = 200;
    float topLeftStartY = tableSize.height - 200;
    float topRightStartX = tableSize.width - 200;
    float topRightStartY = tableSize.height - 200;
    int count = _model->getTableCards().size();
    int leftCount = (count + 1) / 2;

    if (index < leftCount) {
        float progress = (leftCount == 1) ? 0.5f : static_cast<float>(index) / (leftCount - 1);
        float x = topLeftStartX + (leftBottomX - topLeftStartX) * progress;
        float y = topLeftStartY - (topLeftStartY - bottomY) * progress;
        return Vec2(x, y);
    }
    else {
        int rightIndex = index - leftCount;
        int rightCount = count / 2;
        float progress = (rightCount == 1) ? 0.5f : static_cast<float>(rightIndex) / (rightCount - 1);
        float x = topRightStartX - (topRightStartX - rightBottomX) * progress;
        float y = topRightStartY - (topRightStartY - bottomY) * progress;
        return Vec2(x, y);
    }
}

Vec2 GameView::getTargetPositionForHandCard(int index) {
    Size handSize = _handArea->getContentSize();
    int count = _model->getHandCards().size();

    float cardWidth = 100;
    float spacing = 20;
    float totalCardsWidth = (count - 1) * spacing + count * cardWidth;
    float startX = (handSize.width - totalCardsWidth) / 2;
    float centerY = handSize.height / 2;

    if (index == count - 1) {
        return Vec2(startX + (count - 1 + 2) * (cardWidth + spacing), centerY);
    }
    else {
        return Vec2(startX + index * (cardWidth + spacing), centerY);
    }
}

// 执行牌的移动动画
void GameView::animateCardMovement(CardView* card, const Vec2& targetPos, std::function<void()> callback) {
    card->stopAllActions();
    auto moveTo = MoveTo::create(0.3f, targetPos);
    auto sequence = Sequence::create(
        moveTo,
        CallFunc::create([callback]() {
            if (callback) callback();
            }),
        nullptr
                );
    card->runAction(sequence);
}

void GameView::handleTableCardViews(std::vector<CardView*>& newTableCardViews){
    const auto& newTableCards = _model->getTableCards();
    // 处理桌面牌,修改_tablecardView（handview）,只剩下需要被操作的纸牌
    for (int i = 0; i < newTableCards.size(); ++i) {
        const Card& card = newTableCards[i];
        std::string cardId = getCardUniqueId(card);
        // 查找是否有现成的牌视图可以复用
        CardView* cardView = nullptr;
        for (auto it = _tableCardViews.begin(); it != _tableCardViews.end(); ) {
            if (getCardUniqueId((*it)->getCard()) == cardId) {
                cardView = *it;
                it = _tableCardViews.erase(it);
                break;
            }
            else {
                ++it;
            }
        }
        // 如果没有找到，则检查手牌中是否有
        if (!cardView) {
            for (auto it = _handCardViews.begin(); it != _handCardViews.end();++it ) {
                if (getCardUniqueId((*it)->getCard()) == cardId) {
                    cardView = *it;
                    it = _handCardViews.erase(it);
                    // 手动保留节点，防止被释放
                    cardView->retain();
                    cardView->removeFromParent();
                    _tableArea->addChild(cardView);
                    cardView->release();
                    break;
                }     
            }
        }
        // 如果仍然没有，则创建新的牌视图
        if (!cardView) {
            cardView = CardView::create(card);
            _tableArea->addChild(cardView);
            Vec2 targetPos = getTargetPositionForTableCard(i);
            cardView->setPosition(targetPos + Vec2(0, 200));
        }
        cardView->setOnClickCallback([this, i](CardView* view) {
            if (_onTableCardClicked) {
                _onTableCardClicked(i);
            }
            });
        // 记录目标位置并执行动画
        Vec2 targetPos = getTargetPositionForTableCard(i);
        animateCardMovement(cardView, targetPos);
        newTableCardViews.push_back(cardView);
    }
}
void GameView::handleHandCardViews(std::vector<CardView*>& newHandCardViews) {
    const auto& newHandCards = _model->getHandCards();
    for (int i = 0; i < newHandCards.size(); ++i) {
        const Card& card = newHandCards[i];
        std::string cardId = getCardUniqueId(card);
        // 查找是否有现成的牌视图可以复用
        CardView* cardView = nullptr;
        for (auto it = _handCardViews.begin(); it != _handCardViews.end(); it++) {
            if (getCardUniqueId((*it)->getCard()) == cardId) {
                cardView = *it;
                it = _handCardViews.erase(it);
                break;
            }      
        }
        if (!cardView) {
            for (auto it = _tableCardViews.begin(); it != _tableCardViews.end(); it++) {
                if (getCardUniqueId((*it)->getCard()) == cardId) {
                    cardView = *it;
                    it = _tableCardViews.erase(it);
                    // 将牌从桌面区移动到手牌区

                    // 手动保留节点，防止被释放
                    cardView->retain();
                    cardView->removeFromParent();
                    _handArea->addChild(cardView);
                    // 释放手动保留的引用
                    cardView->release();
                    break;
                }      
            }
        }
        // 如果仍然没有，则创建新的牌视图
        if (!cardView) {
            cardView = CardView::create(card);
            _handArea->addChild(cardView);
            // 设置初始位置
            Vec2 targetPos = getTargetPositionForHandCard(i);
            cardView->setPosition(targetPos + Vec2(0, -200));
        }
        // 设置点击回调
        cardView->setOnClickCallback([this, i](CardView* view) {
            if (_onHandCardClicked) {
                _onHandCardClicked(i);
            }
            });
        // 记录目标位置并执行动画
        Vec2 targetPos = getTargetPositionForHandCard(i);
        animateCardMovement(cardView, targetPos);
        newHandCardViews.push_back(cardView);
    }
}