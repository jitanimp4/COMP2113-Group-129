// ============================================================
// THE FORGER'S TABLE
// ODYSSEY OS v2.7 | Interpol Heritage Authentication Unit
// ============================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <map>
#include <chrono>
#include <random>
#include <cctype>
#include <thread>
#include <atomic>
#include <cstdlib>
#include <ctime>
#include <functional>

#ifdef _WIN32
  #include <windows.h>
  void sleepMs(int ms) { Sleep(ms); }
  void clearScreen()   { system("cls"); }
#else
  #include <unistd.h>
  void sleepMs(int ms) { usleep(static_cast<useconds_t>(ms) * 1000); }
  void clearScreen()   { system("clear"); }
#endif

// ── Constants ──────────────────────────────────────────────────
const int  FW              = 30;   // field column display width
const int  QUESTIONS_EASY  = 5;
const int  QUESTIONS_MED   = 5;
const int  QUESTIONS_HARD  = 7;
const int  TIMER_SECONDS   = 30;
const int  SPEED_THRESHOLD = 15;   // seconds for speed bonus

// ── Enums ──────────────────────────────────────────────────────
enum class Difficulty { INTERN, ARCHIVIST, MASTER };

// ── Structs ────────────────────────────────────────────────────
struct ForgedField { std::string name, value; };

struct Artifact {
    std::string name;
    std::vector<std::pair<std::string,std::string>> fields; // ordered (name, value)
    std::string fact, difficulty;
    std::vector<ForgedField> easyForge, mediumForge, hardForge;
};

struct QResult {
    std::string artifact;
    bool correct = false;
    std::vector<std::string> correctFields;
    std::string playerInput;
};

// ── Timer globals ──────────────────────────────────────────────
std::atomic<bool> g_timerActive{false};
std::atomic<int>  g_timeLeft{TIMER_SECONDS};

// ── RNG ────────────────────────────────────────────────────────
std::mt19937 g_rng{std::random_device{}()};

// ══════════════════════════════════════════════════════════════
// UTILITY FUNCTIONS
// ══════════════════════════════════════════════════════════════

std::string toUp(std::string s) {
    for (char& c : s) c = (char)toupper((unsigned char)c);
    return s;
}

std::string trimStr(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    return a == std::string::npos ? "" : s.substr(a, b - a + 1);
}

std::string padR(const std::string& s, size_t w) {
    if (s.size() >= w) return s.substr(0, w);
    return s + std::string(w - s.size(), ' ');
}

// Typewriter effect
void tw(const std::string& s, int ms = 18) {
    for (char c : s) { std::cout << c << std::flush; sleepMs(ms); }
    std::cout << '\n';
}

void divider(char c = '-', int w = 70) {
    std::cout << "  " << std::string(w, c) << '\n';
}

std::vector<ForgedField> parseForge(const std::string& v) {
    std::vector<ForgedField> r;
    std::stringstream ss(v);
    std::string tok;
    while (std::getline(ss, tok, '|')) {
        tok = trimStr(tok);
        size_t eq = tok.find('=');
        if (eq != std::string::npos)
            r.push_back({trimStr(tok.substr(0, eq)), trimStr(tok.substr(eq+1))});
    }
    return r;
}

// ══════════════════════════════════════════════════════════════
// FILE LOADING
// ══════════════════════════════════════════════════════════════

std::vector<Artifact> loadArtifacts(const std::string& fn) {
    std::vector<Artifact> arts;
    std::ifstream f(fn);
    if (!f) {
        std::cout << "\n  [ERROR: DATABASE CORRUPTED — CONTACT SYSTEM ADMIN]\n";
        return arts;
    }
    Artifact cur;
    bool had = false;
    std::string line;

    while (std::getline(f, line)) {
        line = trimStr(line);
        if (line == "===" || line.empty()) {
            if (had && !cur.name.empty()) { arts.push_back(cur); cur = {}; had = false; }
            continue;
        }
        if (line[0] == '#') continue;
        size_t cp = line.find(':');
        if (cp == std::string::npos) continue;
        std::string k = trimStr(line.substr(0, cp));
        std::string v = trimStr(line.substr(cp + 1));
        had = true;
        if      (k == "NAME")         cur.name = v;
        else if (k == "DATE")         cur.fields.push_back({"DATE",     v});
        else if (k == "LOCATION")     cur.fields.push_back({"LOCATION", v});
        else if (k == "BUILDER")      cur.fields.push_back({"BUILDER",  v});
        else if (k == "MATERIAL")     cur.fields.push_back({"MATERIAL", v});
        else if (k == "STYLE")        cur.fields.push_back({"STYLE",    v});
        else if (k == "FACT")         cur.fact = v;
        else if (k == "DIFFICULTY")   cur.difficulty = v;
        else if (k == "EASY_FORGE")   cur.easyForge   = parseForge(v);
        else if (k == "MEDIUM_FORGE") cur.mediumForge = parseForge(v);
        else if (k == "HARD_FORGE")   cur.hardForge   = parseForge(v);
    }
    if (!cur.name.empty()) arts.push_back(cur);
    return arts;
}

// ══════════════════════════════════════════════════════════════
// TIMER THREAD
// ══════════════════════════════════════════════════════════════

void timerFn() {
    while (g_timerActive.load() && g_timeLeft.load() > 0) {
        sleepMs(1000);
        if (g_timerActive.load()) g_timeLeft.fetch_sub(1);
    }
    if (g_timerActive.load()) {
        // Timer expired, print message to stderr
        std::fprintf(stderr, "\n  [TIME: 00s — TIME HAS EXPIRED]\n");
        std::fflush(stderr);
    }
}

// ══════════════════════════════════════════════════════════════
// DISPLAY: ASCII ART & FEEDBACK
// ══════════════════════════════════════════════════════════════

void showForgeryAlert() {
    std::string banner =
        "  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "  !      F O R G E R Y   A L E R T               !\n"
        "  !           SUSPECT  IDENTIFIED                  !\n"
        "  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";

    for (int i = 0; i < 2; i++) {
        std::cout << '\n' << banner;
        std::cout.flush();
        sleepMs(350);
        // Erase lines (ANSI escape)
        for (int j = 0; j < 5; j++) std::cout << "\033[1A\033[2K";
        sleepMs(250);
    }
    std::cout << '\n' << banner << '\n';
}

void showAuthentic() {
    const std::string lines[] = {
        "  ########################################\n",
        "  #    [ A U T H E N T I C A T E D ]    #\n",
        "  #        CASE FILE SECURED             #\n",
        "  ########################################\n"
    };
    for (auto& l : lines) {
        for (char c : l) { std::cout << c << std::flush; sleepMs(8); }
    }
    std::cout << '\n';
}

void showTimeout() {
    divider('!');
    std::cout << "  [TIME EXPIRED] — CASE FILE AUTO-REJECTED\n";
    divider('!');
    std::cout << '\n';
}

void showCaseClosed(int score) {
    std::cout << '\n';
    divider('=');
    tw("  [CASE CLOSED — INSUFFICIENT EVIDENCE]", 20);
    std::cout << "  Partial score recorded: " << score << " pts\n";
    divider('=');
}

// ══════════════════════════════════════════════════════════════
// DISPLAY: ARTIFACT SIDE-BY-SIDE
// ══════════════════════════════════════════════════════════════

void displayArtifacts(const Artifact& art,
                       const std::vector<ForgedField>& forges,
                       bool showLabels)
{
    // Build forged field lookup
    std::map<std::string,std::string> fm;
    for (auto& ff : forges) fm[ff.name] = ff.value;

    size_t w = FW;

    auto cell = [&](const std::string& s) -> std::string {
        return " " + padR(s, w) + " ";
    };

    std::cout << '\n';
    std::cout << "  +" << std::string(w+2,'=') << "+" << std::string(w+2,'=') << "+\n";
    std::cout << "  |" << cell("[FILE A — CLASSIFIED]")
              << "|" << cell("[FILE B — CLASSIFIED]") << "|\n";
    std::cout << "  +" << std::string(w+2,'-') << "+" << std::string(w+2,'-') << "+\n";
    std::cout << "  |" << cell(art.name) << "|" << cell(art.name) << "|\n";
    std::cout << "  +" << std::string(w+2,'-') << "+" << std::string(w+2,'-') << "+\n";

    for (auto& [fn, fv] : art.fields) {
        std::string label = showLabels ? fn + ": " : "";
        std::string authVal  = label + fv;
        std::string forgedVal = label + (fm.count(fn) ? fm[fn] : fv);
        std::cout << "  |" << cell(authVal) << "|" << cell(forgedVal) << "|\n";
    }

    std::cout << "  +" << std::string(w+2,'=') << "+" << std::string(w+2,'=') << "+\n\n";
}

// ══════════════════════════════════════════════════════════════
// SAVE SCORE
// ══════════════════════════════════════════════════════════════

void saveScore(int score, Difficulty diff, int maxStreak) {
    std::ofstream f("save_data.txt", std::ios::app);
    if (!f) return;

    std::time_t now = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    std::string dn = diff == Difficulty::INTERN   ? "Intern" :
                     diff == Difficulty::ARCHIVIST ? "Archivist" : "Master Curator";
    f << "[" << buf << "]  Level: " << std::left << std::setw(14) << dn
      << "  Score: " << std::setw(6) << score
      << "  Best Streak: " << maxStreak << "\n";
}

// ══════════════════════════════════════════════════════════════
// CORE: INPUT HANDLING
// ══════════════════════════════════════════════════════════════

// Get a single valid field name from the player (Intern / Archivist)
std::string getSingleFieldInput() {
    const std::vector<std::string> valid = {"DATE","LOCATION","BUILDER","MATERIAL","STYLE"};
    std::string input;
    while (true) {
        std::cout << "  > " << std::flush;
        std::getline(std::cin, input);
        input = trimStr(input);
        std::string up = toUp(input);
        if (std::find(valid.begin(), valid.end(), up) != valid.end())
            return up;
        std::cout << "  [INVALID FIELD — TRY AGAIN]  "
                     "(DATE / LOCATION / BUILDER / MATERIAL / STYLE)\n";
    }
}

// Parse comma-separated field names for Hard mode
std::vector<std::string> parseMultiInput(const std::string& raw) {
    std::vector<std::string> result;
    std::stringstream ss(raw);
    std::string tok;
    while (std::getline(ss, tok, ',')) {
        tok = trimStr(toUp(tok));
        if (!tok.empty()) result.push_back(tok);
    }
    std::sort(result.begin(), result.end());
    return result;
}

// ══════════════════════════════════════════════════════════════
// CORE: PLAY ROUND
// ══════════════════════════════════════════════════════════════

void playRound(Difficulty diff, std::vector<Artifact>& arts) {

    int lives     = diff == Difficulty::INTERN   ? 5 :
                    diff == Difficulty::ARCHIVIST ? 3 : 2;
    int numQ      = diff == Difficulty::MASTER   ? QUESTIONS_HARD : QUESTIONS_EASY;
    bool labels   = (diff == Difficulty::INTERN);
    bool hasTimer = (diff == Difficulty::MASTER);

    int  score = 0, streak = 0, maxStreak = 0;
    std::vector<QResult> results;

    // ── Build candidate pool filtered by difficulty ──────────
    std::vector<int> pool;
    for (int i = 0; i < (int)arts.size(); i++) {
        const std::string& ad = arts[i].difficulty;
        if (diff == Difficulty::INTERN    && ad == "easy")              pool.push_back(i);
        if (diff == Difficulty::ARCHIVIST && (ad=="easy"||ad=="medium")) pool.push_back(i);
        if (diff == Difficulty::MASTER)                                  pool.push_back(i);
    }
    if (pool.empty()) {
        std::cout << "  [ERROR: No artifacts match this difficulty level.]\n";
        return;
    }

    std::shuffle(pool.begin(), pool.end(), g_rng);
    // Extend pool if needed by repeating
    while ((int)pool.size() < numQ) {
        std::vector<int> extra = pool;
        std::shuffle(extra.begin(), extra.end(), g_rng);
        pool.insert(pool.end(), extra.begin(), extra.end());
    }

    // ── Briefing screen ──────────────────────────────────────
    clearScreen();
    std::cout << '\n';
    divider('=');
    std::string dn = diff==Difficulty::INTERN ? "INTERN" :
                     diff==Difficulty::ARCHIVIST ? "ARCHIVIST" : "MASTER CURATOR";
    std::cout << "  [ODYSSEY OS]: Case files loaded. Clearance level: " << dn << "\n";
    std::cout << "  Lives: " << lives << "  |  Questions: " << numQ << "\n";

    if (diff == Difficulty::INTERN)
        tw("  Tip: Fields are labelled. Type the name of the forged field.", 14);
    else if (diff == Difficulty::ARCHIVIST)
        tw("  Tip: No labels. Fields appear in order: DATE, LOCATION, BUILDER, MATERIAL, STYLE", 14);
    else {
        tw("  Tip: Up to 3 fields may be forged. List them all (comma-separated).", 14);
        tw("  Tip: Some file pairs are AUTHENTIC — if nothing is forged, type: AUTHENTIC", 14);
        tw("  Tip: You have 30 seconds per question.", 14);
    }
    divider('=');
    std::cout << '\n';
    sleepMs(1000);

    // ── Question loop ─────────────────────────────────────────
    for (int q = 0; q < numQ && lives > 0; q++) {

        const Artifact& art = arts[pool[q]];

        // Determine forgeries
        std::vector<ForgedField> forges;
        bool isAuthentic = false;

        if (diff == Difficulty::INTERN) {
            forges = art.easyForge;
        } else if (diff == Difficulty::ARCHIVIST) {
            forges = art.mediumForge;
        } else {
            // Hard: ~20% chance of authentic pair
            std::uniform_int_distribution<int> coin(0,4);
            if (coin(g_rng) == 0) { isAuthentic = true; }
            else                  { forges = art.hardForge; }
        }

        // Correct answer field list
        std::vector<std::string> correctFields;
        if (isAuthentic) correctFields.push_back("AUTHENTIC");
        else for (auto& ff : forges) correctFields.push_back(ff.name);
        std::sort(correctFields.begin(), correctFields.end());

        // ── Draw question ─────────────────────────────────────
        clearScreen();
        std::cout << '\n';
        divider('=');
        std::cout << "  QUESTION " << (q+1) << "/" << numQ
                  << "   |   Lives: " << lives
                  << "   |   Score: " << score;
        if (streak >= 3)
            std::cout << "   |   ★ FORENSICS STREAK x2 [" << streak << "]";
        std::cout << '\n';
        if (hasTimer)
            std::cout << "  [TIMER: " << TIMER_SECONDS << " seconds — answer quickly for a speed bonus]\n";
        divider('=');

        displayArtifacts(art, forges, labels);

        // Prompt
        if (diff == Difficulty::MASTER) {
            std::cout << "  Which fields are forged? (comma-separated: DATE, BUILDER...)\n";
            std::cout << "  Or type AUTHENTIC if both files are identical.\n";
        } else {
            std::cout << "  Which field has been forged?\n";
            std::cout << "  Options: DATE  LOCATION  BUILDER  MATERIAL  STYLE\n";
        }
        std::cout << '\n';

        // ── Start timer ──────────────────────────────────────
        auto startTime = std::chrono::steady_clock::now();
        if (hasTimer) {
            g_timeLeft.store(TIMER_SECONDS);
            g_timerActive.store(true);
            std::thread thr(timerFn);
            thr.detach();
        }

        // ── Get input ─────────────────────────────────────────
        std::string raw;
        if (diff == Difficulty::MASTER) {
            std::cout << "  > " << std::flush;
            std::getline(std::cin, raw);
            raw = trimStr(raw);
        } else {
            raw = getSingleFieldInput();
        }

        // ── Stop timer, check elapsed ─────────────────────────
        bool timedOut = false;
        int  elapsed  = 0;
        if (hasTimer) {
            g_timerActive.store(false);
            elapsed = (int)std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - startTime).count();
            timedOut = (elapsed > TIMER_SECONDS || g_timeLeft.load() <= 0);
        }

        // ── Evaluate answer ───────────────────────────────────
        bool correct = false;

        if (!timedOut) {
            std::string up = toUp(raw);
            if (diff == Difficulty::MASTER) {
                if (isAuthentic) {
                    correct = (up == "AUTHENTIC");
                } else {
                    auto given = parseMultiInput(raw);
                    correct = (given == correctFields);
                }
            } else {
                correct = (!correctFields.empty() && up == correctFields[0]);
            }
        }

        // ── Build result record ───────────────────────────────
        QResult qr;
        qr.artifact     = art.name;
        qr.correct      = correct;
        qr.correctFields= correctFields;
        qr.playerInput  = raw;

        // ── Feedback ──────────────────────────────────────────
        if (timedOut) {
            showTimeout();
            lives--;
            streak = 0;
        } else if (correct) {
            showAuthentic();
            int pts = 100;
            if (streak >= 2) pts *= 2; // streak multiplier kicks in after 3 in a row
            if (hasTimer && elapsed < SPEED_THRESHOLD) {
                pts += 50;
                std::cout << "  [SPEED BONUS: +50 pts  (" << elapsed << "s)]\n";
            }
            score += pts;
            streak++;
            if (streak > maxStreak) maxStreak = streak;
            if (streak == 3) tw("  [★ FORENSICS STREAK x2 ACTIVATED — keep going!]", 12);
            std::cout << "  [+" << pts << " pts | Total: " << score << " pts]\n";
        } else {
            showForgeryAlert();
            std::cout << "  [INCORRECT] The forged field(s) were: ";
            for (int i = 0; i < (int)correctFields.size(); i++) {
                if (i) std::cout << ", ";
                std::cout << correctFields[i];
            }
            std::cout << '\n';
            lives--;
            streak = 0;
        }

        results.push_back(qr);

        // ── Historical fact ───────────────────────────────────
        if (!art.fact.empty()) {
            sleepMs(600);
            std::cout << '\n';
            divider('~');
            std::cout << "  [DEBRIEFING NOTE]\n  ";
            tw(art.fact, 11);
            divider('~');
        }

        if (lives <= 0) {
            showCaseClosed(score);
            results.push_back(qr); // already added above — don't duplicate
            // break handled by loop condition
        } else if (q < numQ - 1) {
            std::cout << "\n  Lives: " << lives
                      << "  |  Score: " << score
                      << "  |  [Press ENTER to continue...]\n";
            std::string pause;
            std::getline(std::cin, pause);
        }
    }

    // ── Life bonus ────────────────────────────────────────────
    int lifeBonus = lives * 50;
    if (lives > 0) score += lifeBonus;

    // ══════════════════════════════════════════════════════════
    // CASE FILE SUMMARY
    // ══════════════════════════════════════════════════════════
    clearScreen();
    std::cout << '\n';
    std::cout << "  ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║                     CASE FILE SUMMARY                           ║\n";
    std::cout << "  ╚══════════════════════════════════════════════════════════════════╝\n\n";

    int numCorrect = 0;
    for (auto& r : results) {
        std::string status = r.correct ? "[✓ AUTHENTICATED]" : "[✗ FAILED       ]";
        std::string ans;
        for (int i = 0; i < (int)r.correctFields.size(); i++) {
            if (i) ans += ", ";
            ans += r.correctFields[i];
        }
        std::cout << "  " << status << "  " << padR(r.artifact, 28) << " | " << ans << '\n';
        if (r.correct) numCorrect++;
    }

    std::cout << '\n';
    divider('-');
    if (lives > 0)
        std::cout << "  Life Bonus:    +" << lifeBonus << " pts  (" << lives << " lives remaining)\n";
    std::cout << "  Best Streak:   " << maxStreak << "\n";
    std::cout << "  Final Score:   " << score << " pts\n";
    divider('=');

    // ── Narrative ending ─────────────────────────────────────
    std::cout << '\n';
    tw("  [ODYSSEY OS]: Forgery ring assessment complete.", 15);
    tw("  [ODYSSEY OS]: " + std::to_string(numCorrect) +
       " artifact(s) returned to the heritage database.", 15);

    int pct = diff==Difficulty::INTERN ? 40 : diff==Difficulty::ARCHIVIST ? 65 : 80;
    std::cout << "  [ODYSSEY OS]: Decryption progress: " << pct << "% complete.\n";

    saveScore(score, diff, maxStreak);
    std::cout << "  [Score appended to save_data.txt]\n";
    divider('=');
}

// ══════════════════════════════════════════════════════════════
// INTRO SEQUENCE
// ══════════════════════════════════════════════════════════════

void showIntro() {
    clearScreen();
    std::cout << "\n\n";
    std::cout << "  ╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║        INTERPOL ARTIFACT AUTHENTICATION UNIT                ║\n";
    std::cout << "  ║               ─── ODYSSEY OS v2.7 ───                       ║\n";
    std::cout << "  ╚══════════════════════════════════════════════════════════════╝\n\n";

    auto loadStep = [](const std::string& msg, int baseMs) {
        std::cout << "  " << msg << std::flush;
        for (int i = 0; i < 6; i++) { sleepMs(baseMs); std::cout << '.' << std::flush; }
        std::cout << " [OK]\n";
    };

    loadStep("Accessing Interpol Database  ", 300);
    loadStep("Decrypting case files        ", 250);
    loadStep("Loading artifact database    ", 200);
    std::cout << '\n';
    sleepMs(400);

    divider('=');
    tw("  THE  FORGER'S  TABLE", 50);
    divider('=');
    std::cout << '\n';
    tw("  A forgery ring has infiltrated the global heritage archive.", 14);
    tw("  Two case files will appear — one authentic, one tampered.", 14);
    tw("  Your mission: identify which field the forger changed.", 14);
    std::cout << '\n';
}

// ══════════════════════════════════════════════════════════════
// DIFFICULTY SELECTION
// ══════════════════════════════════════════════════════════════

Difficulty selectDifficulty() {
    std::cout << "  ┌───────────────────────────────────────────┐\n";
    std::cout << "  │          SELECT CLEARANCE LEVEL           │\n";
    std::cout << "  ├───────────────────────────────────────────┤\n";
    std::cout << "  │  [1]  INTERN          (Easy)              │\n";
    std::cout << "  │       5 lives · labelled fields           │\n";
    std::cout << "  │                                           │\n";
    std::cout << "  │  [2]  ARCHIVIST       (Medium)            │\n";
    std::cout << "  │       3 lives · no labels · subtle fakes  │\n";
    std::cout << "  │                                           │\n";
    std::cout << "  │  [3]  MASTER CURATOR  (Hard)              │\n";
    std::cout << "  │       2 lives · 3 forgeries · 30s timer   │\n";
    std::cout << "  │       Some pairs may be AUTHENTIC          │\n";
    std::cout << "  └───────────────────────────────────────────┘\n\n";
    std::cout << "  Enter clearance level (1 / 2 / 3): ";

    std::string inp;
    while (true) {
        std::getline(std::cin, inp);
        inp = trimStr(inp);
        if (inp == "1") return Difficulty::INTERN;
        if (inp == "2") return Difficulty::ARCHIVIST;
        if (inp == "3") return Difficulty::MASTER;
        std::cout << "  [Invalid — enter 1, 2, or 3]: ";
    }
}

// ══════════════════════════════════════════════════════════════
// MAIN
// ══════════════════════════════════════════════════════════════

// ══════════════════════════════════════════════════════════════
// MAIN
// ══════════════════════════════════════════════════════════════

void runForgersTable() { // <--- CHANGED FROM int main()
    showIntro();

    // Locate forger_bank.txt next to the binary
    std::vector<Artifact> arts = loadArtifacts("forger_bank.txt");
    if (arts.empty()) {
        std::cout << "\n  [CRITICAL: No artifacts loaded. Ensure forger_bank.txt is present.]\n";
        return; // <--- CHANGED FROM return 1;
    }
    
    // ... (keep the rest of the game loop exactly the same) ...

    std::cout << '\n';
    tw("  [ODYSSEY OS]: Agent dismissed. Heritage database stabilising.", 16);
    std::cout << "  [ODYSSEY OS]: Cryptex decryption queued for next session.\n\n";
    
    return; // <--- CHANGED FROM return 0;
}
