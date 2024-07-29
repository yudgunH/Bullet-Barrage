#ifndef SCORE_H
#define SCORE_H

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

class Score {
public:
    Score();
    void addScore(int newScore);
    const std::vector<int>& getTopScores() const;
    void saveScores(const std::string& filename) const;
    void loadScores(const std::string& filename);

private:
    std::vector<int> topScores;
    static constexpr int maxScores = 10;
};

#endif // SCORE_H
