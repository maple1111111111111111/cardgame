#include "Card.h"
#include <string>

Card::Card(Suit suit, Rank rank) : _suit(suit), _rank(rank) {}

Suit Card::getSuit() const {
    return _suit;
}

Rank Card::getRank() const {
    return _rank;
}

int Card::getRankValue() const {
    return static_cast<int>(_rank) + 1;
}

std::string Card::getCardName() const {
    std::string suitStr;
    switch (_suit) {
    case Suit::HEART: suitStr = "heart"; break;
    case Suit::DIAMOND: suitStr = "diamond"; break;
    case Suit::CLUB: suitStr = "club"; break;
    case Suit::SPADE: suitStr = "spade"; break;
    }

    std::string rankStr;
    switch (_rank) {
    case Rank::ACE: rankStr = "a"; break;
    case Rank::TWO: rankStr = "2"; break;
    case Rank::THREE: rankStr = "3"; break;
    case Rank::FOUR: rankStr = "4"; break;
    case Rank::FIVE: rankStr = "5"; break;
    case Rank::SIX: rankStr = "6"; break;
    case Rank::SEVEN: rankStr = "7"; break;
    case Rank::EIGHT: rankStr = "8"; break;
    case Rank::NINE: rankStr = "9"; break;
    case Rank::TEN: rankStr = "10"; break;
    case Rank::JACK: rankStr = "j"; break;
    case Rank::QUEEN: rankStr = "q"; break;
    case Rank::KING: rankStr = "k"; break;
    }

    return suitStr + "_" + rankStr;
}

bool Card::canMatch(const Card& other) const {
    int diff = getRankValue() - other.getRankValue();
    return (diff == 1 || diff == -1);
}

// 在Card.h中添加
std::string Card::getSuitName() const {
    // 根据花色枚举返回对应字符串
        switch (_suit) {
        case Suit::DIAMOND: return "diamond";
        case Suit::HEART: return "heart";
        case Suit::CLUB: return "club"; 
        case Suit::SPADE: return "spade"; 
        }
    }


std::string Card::getNumberName() const {
    // 根据数字枚举返回对应字符串
    std::string rankStr;
    switch (_rank) {
    case Rank::ACE: rankStr = "a"; break;
    case Rank::TWO: rankStr = "2"; break;
    case Rank::THREE: rankStr = "3"; break;
    case Rank::FOUR: rankStr = "4"; break;
    case Rank::FIVE: rankStr = "5"; break;
    case Rank::SIX: rankStr = "6"; break;
    case Rank::SEVEN: rankStr = "7"; break;
    case Rank::EIGHT: rankStr = "8"; break;
    case Rank::NINE: rankStr = "9"; break;
    case Rank::TEN: rankStr = "10"; break;
    case Rank::JACK: rankStr = "j"; break;
    case Rank::QUEEN: rankStr = "q"; break;
    case Rank::KING: rankStr = "k"; break;
    }
    return rankStr;
}