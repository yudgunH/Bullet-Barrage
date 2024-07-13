#pragma once
#ifndef SCORE_H
#define SCORE_H

#include <vector>
#include <string>
#include <algorithm>

class Score {
public:
    Score();
    void addScore(int newScore);
    const std::vector<int>& getTopScores() const;

private:
    std::vector<int> topScores;
    const int maxScores = 10;
};

#endif // SCORE_H
