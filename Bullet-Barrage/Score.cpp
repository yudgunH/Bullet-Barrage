#include "Score.h"
#include <iostream>

Score::Score() {
    // Constructor body can be empty as no initialization is required.
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

void Score::saveScores(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }
    for (int score : topScores) {
        outFile << score << std::endl;
    }
}

void Score::loadScores(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return;
    }
    int score;
    topScores.clear();
    while (inFile >> score) {
        topScores.push_back(score);
    }
    std::sort(topScores.begin(), topScores.end(), std::greater<int>());
    if (topScores.size() > maxScores) {
        topScores.resize(maxScores);
    }
}