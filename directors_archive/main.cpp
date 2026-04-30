#include <iostream>
#include "celluloid_app.h"

using namespace std;

void runDirectorsArchive() {
    int difficulty = 2; 
    cout << "\nSelect Profile:\n1. Intern (Easy)\n2. Archivist (Medium)\n3. Master Curator (Hard)\n> ";
    cin >> difficulty;

    CelluloidApp* app2 = new CelluloidApp(difficulty);
    app2->run();
    delete app2;
}
