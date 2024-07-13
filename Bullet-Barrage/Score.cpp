#include "Score.h"

Score::Score() {
}

void Score::addScore(int newScore) {
    topScores.push_back(newScore);
    std::sort(topScores.begin(), topScores.end(), std::greater<int>());
    if (topScores.size() > maxScores) {
        topScores.pop_back();
    }
}

const std::vector<int>& Score::getTopScores() const {
    return topScores;
}
