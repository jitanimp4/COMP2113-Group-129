# The Forger's Table
### ODYSSEY OS v2.7 | Interpol Heritage Authentication Unit

A terminal-based artifact authentication game. Two case files appear side by side — one authentic, one tampered. Identify the forged field before the forgery ring escapes.

---

## Build & Run

```bash
make
./forger_table
```

Or compile manually:
```bash
g++ -std=c++17 -O2 -o forger_table main.cpp -lpthread
./forger_table
```

Requires: `forger_bank.txt` in the same directory as the binary.

---

## Difficulty Levels

| Level | Lives | Labels | Forgeries | Timer |
|---|---|---|---|---|
| **Intern** | 5 | ✓ Labelled | 1 (obvious) | None |
| **Archivist** | 3 | ✗ None | 1 (subtle) | None |
| **Master Curator** | 2 | ✗ None | Up to 3 | 30 seconds |

On **Master Curator**, some artifact pairs are completely authentic — type `AUTHENTIC` if you spot no differences.

---

## Scoring

- **+100 pts** per correct answer
- **×2 multiplier** after a 3-answer streak (FORENSICS STREAK)
- **+50 speed bonus** on Master Curator if answered under 15 seconds
- **+50 per life remaining** at end of round
- Scores saved with timestamp to `save_data.txt`

---

## Fields (always in this order)
`DATE` · `LOCATION` · `BUILDER` · `MATERIAL` · `STYLE`

On Intern mode these are labelled. On Archivist and higher you must remember the order.
