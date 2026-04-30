#include "puzzle.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <random>
#include <tuple>

namespace {
std::mt19937 &globalRng() {
    static std::mt19937 rng(
        static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));
    return rng;
}

bool inBounds(int r, int c, int n) { return r >= 0 && c >= 0 && r < n && c < n; }

std::string normalizeWord(const std::string &s) {
    std::string out;
    for (char c : s) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            out.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
        }
    }
    return out;
}

bool placeWord(std::vector<std::string> &grid, int r, int c, Direction d, const std::string &word) {
    int n = static_cast<int>(grid.size());
    int dr = (d == Direction::Across ? 0 : 1);
    int dc = (d == Direction::Across ? 1 : 0);

    for (int i = 0; i < static_cast<int>(word.size()); i++) {
        int rr = r + dr * i;
        int cc = c + dc * i;
        if (!inBounds(rr, cc, n)) return false;
        char existing = grid[rr][cc];
        if (existing != '#' && existing != word[i]) return false;
    }
    for (int i = 0; i < static_cast<int>(word.size()); i++) {
        int rr = r + dr * i;
        int cc = c + dc * i;
        grid[rr][cc] = word[i];
    }
    return true;
}

std::vector<RawItem> easyRaw() {
    return {
        {"PYRAMID", "Ancient wonder in Giza"},
        {"ROME", "The Colosseum's city"},
        {"PETRA", "Jordan's rose-red ancient city"},
        {"ANGKOR", "Cambodian temple complex"},
        {"EIFFEL", "Famous iron tower in Paris"},
        {"TAJ", "Mahal shorthand monument"},
        {"WALL", "Great one in China"},
        {"MACHU", "Peruvian mountain citadel (short form)"}};
}

std::vector<RawItem> mediumRaw() {
    return {
        {"DAVINCI", "Man who painted the Mona Lisa"},
        {"MONET", "Painter tied to Impression, Sunrise"},
        {"HAMLET", "Shakespeare tragedy with 'To be, or not to be'"},
        {"DRAMA", "Stage art category"},
        {"BAROQUE", "Dramatic and ornate European art style"},
        {"ODYSSEY", "Epic journey poem linked to Homer"},
        {"FRESCO", "Painting done on wet plaster"},
        {"MUSEUM", "Place that exhibits art"},
        {"CANVAS", "Painter's working surface"},
        {"OPERA", "Dramatic art with music"}};
}

std::vector<RawItem> hardRaw() {
    return {
        {"PICASSO", "Cubist co-founder who painted Guernica"},
        {"CAESAR", "Roman ruler tied to the Ides of March"},
        {"CLEO", "Egyptian queen (short form)"},
        {"PLATO", "Student of Socrates"},
        {"SOCRATES", "Philosopher known by questioning method"},
        {"NAPOLEON", "French military leader crowned himself emperor"},
        {"GALILEO", "Scientist linked with heliocentrism trial"},
        {"GANDHI", "Indian independence leader of nonviolence"},
        {"VERMEER", "Dutch painter of Girl with a Pearl Earring"},
        {"ARCHIMED", "Ancient inventor linked to Eureka (short form)"},
        {"SPARTA", "Greek city-state famed for warriors"},
        {"NEWTON", "Scientist of gravity fame"},
        {"CURIE", "Pioneer of radioactivity"},
        {"DARWIN", "Natural selection theorist"},
        {"TESLA", "Inventor tied to AC systems"}};
}

std::vector<RawItem> sanitizeRaw(const std::vector<RawItem> &raw) {
    std::vector<RawItem> cleaned;
    for (auto item : raw) {
        std::string nw = normalizeWord(item.answer);
        if (nw.size() >= 3) {
            item.answer = nw;
            cleaned.push_back(item);
        }
    }
    return cleaned;
}
} // namespace

DifficultyConfig getConfig(int choice) {
    if (choice == 1) {
        return {"Easy", "Wonders & Landmarks", 7, 0, 0, 8, 100, 0, true, false, 0, 0, false, 0, 0, 0};
    }
    if (choice == 2) {
        return {
            "Medium", "Media & Masterpieces", 9, 600, 3, 10, 150, 10, false, true, 7, 0, false, 0, 0, 2};
    }
    return {"Hard", "Iconoclasts & Architects", 11, 420, 1, 15, 220, 25, false, false, 150, 1, true, 3, 120, 3};
}

std::vector<RawItem> getRawByDifficulty(int choice) {
    if (choice == 1) return sanitizeRaw(easyRaw());
    if (choice == 2) return sanitizeRaw(mediumRaw());
    return sanitizeRaw(hardRaw());
}

bool buildCrossword(
    int n, const std::vector<RawItem> &items, std::vector<std::string> &solutionGrid,
    std::vector<Clue> &placedClues) {
    std::vector<RawItem> words = items;
    std::sort(words.begin(), words.end(), [](const RawItem &a, const RawItem &b) {
        return a.answer.size() > b.answer.size();
    });

    for (int attempt = 0; attempt < 250; attempt++) {
        std::vector<std::string> grid(n, std::string(n, '#'));
        std::vector<Clue> clues;

        const std::string &first = words[0].answer;
        if (static_cast<int>(first.size()) > n) continue;
        int startCol = (n - static_cast<int>(first.size())) / 2;
        int startRow = n / 2;
        if (!placeWord(grid, startRow, startCol, Direction::Across, first)) continue;
        clues.push_back({0, Direction::Across, first, words[0].clue, startRow, startCol});

        bool allPlaced = true;
        for (std::size_t wi = 1; wi < words.size(); wi++) {
            const std::string &w = words[wi].answer;
            std::vector<std::tuple<int, int, Direction, int>> candidates;
            for (int i = 0; i < static_cast<int>(w.size()); i++) {
                char ch = w[i];
                for (int r = 0; r < n; r++) {
                    for (int c = 0; c < n; c++) {
                        if (grid[r][c] != ch) continue;
                        int ar = r, ac = c - i;
                        if (placeWord(grid, ar, ac, Direction::Across, w)) {
                            candidates.emplace_back(ar, ac, Direction::Across, 1);
                            std::vector<std::string> temp(n, std::string(n, '#'));
                            for (const auto &cl : clues) placeWord(temp, cl.row, cl.col, cl.direction, cl.answer);
                            grid = temp;
                        }
                        int dr = r - i, dc = c;
                        if (placeWord(grid, dr, dc, Direction::Down, w)) {
                            candidates.emplace_back(dr, dc, Direction::Down, 1);
                            std::vector<std::string> temp(n, std::string(n, '#'));
                            for (const auto &cl : clues) placeWord(temp, cl.row, cl.col, cl.direction, cl.answer);
                            grid = temp;
                        }
                    }
                }
            }

            if (candidates.empty()) {
                for (int rr = 0; rr < n; rr++) {
                    for (int cc = 0; cc < n; cc++) {
                        if (placeWord(grid, rr, cc, Direction::Across, w)) {
                            candidates.emplace_back(rr, cc, Direction::Across, 0);
                            std::vector<std::string> temp(n, std::string(n, '#'));
                            for (const auto &cl : clues) placeWord(temp, cl.row, cl.col, cl.direction, cl.answer);
                            grid = temp;
                        }
                        if (placeWord(grid, rr, cc, Direction::Down, w)) {
                            candidates.emplace_back(rr, cc, Direction::Down, 0);
                            std::vector<std::string> temp(n, std::string(n, '#'));
                            for (const auto &cl : clues) placeWord(temp, cl.row, cl.col, cl.direction, cl.answer);
                            grid = temp;
                        }
                    }
                }
                if (candidates.empty()) {
                    allPlaced = false;
                    break;
                }
            }

            std::shuffle(candidates.begin(), candidates.end(), globalRng());
            bool placed = false;
            for (const auto &cand : candidates) {
                int rr = std::get<0>(cand), cc = std::get<1>(cand);
                Direction dd = std::get<2>(cand);
                if (placeWord(grid, rr, cc, dd, w)) {
                    clues.push_back({0, dd, w, words[wi].clue, rr, cc});
                    placed = true;
                    break;
                }
            }
            if (!placed) {
                allPlaced = false;
                break;
            }
        }
        if (!allPlaced) continue;

        auto num = buildNumbering(grid);
        for (auto &cl : clues) cl.number = num[cl.row][cl.col];
        solutionGrid = grid;
        placedClues = clues;
        return true;
    }
    return false;
}

std::vector<std::vector<int>> buildNumbering(const std::vector<std::string> &grid) {
    int n = static_cast<int>(grid.size());
    std::vector<std::vector<int>> num(n, std::vector<int>(n, 0));
    int counter = 1;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == '#') continue;
            bool startsAcross = (c == 0 || grid[r][c - 1] == '#');
            bool startsDown = (r == 0 || grid[r - 1][c] == '#');
            if (startsAcross || startsDown) num[r][c] = counter++;
        }
    }
    return num;
}
