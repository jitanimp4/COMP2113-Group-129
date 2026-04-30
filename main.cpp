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
