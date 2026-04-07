#include <iostream>
using namespace std;

void start_game () {
  cout << "Welcome to the Crossword Puzzle!\n" << endl; }

// make grid for first level - easy, create a 2D array 

void make_grid_easy () {             
  char grid_easy [7][7];

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      grid [i][j] = '_'; 
    }
  }

// prints the _ with spaces bw them 
  
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      cout << grid[i][j] << " "; 
    }
    cout << endl; 
  }
}
        
        
