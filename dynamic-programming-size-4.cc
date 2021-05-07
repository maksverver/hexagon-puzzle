// Calculates the maximum score for a hexagon of size 4 (diameter 7).
//
// Uses Dynamic Programming to solve the problem in time proportional
// to O((2^n) * (n^2)) where n is the size of the hexagon.
//
// Does not output any actual solutions!

#include <algorithm>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
using namespace std;
  
typedef unsigned long long uint64;

struct Cell {
  int row, col;
  uint64 bit;
  vector<const Cell*> neighbours;
};

const int max_cells = 37;

static vector<Cell> cells;
static int bits;

namespace {

bool valid(int row, int col) {
  return row >= 0 && row < 7 && col >= 0 && col < 7 && abs((row - 3) - (col - 3)) <= 3;
}

int search(int i, uint64 red_mask);

int search_red(int i, uint64 red_mask) {
  int res = 0;
  for (const Cell* cell : cells[i].neighbours) {
    if ((cell->bit & red_mask) == 0) ++res;
  }
  return res + search(i + 1, red_mask | cells[i].bit);
}

int search_blue(int i, uint64 red_mask) {
  int res = 2;
  for (const Cell*cell : cells[i].neighbours) {
    if ((cell->bit & red_mask) != 0) ++res;
  }
  return res + search(i + 1, red_mask);
}

int search(int i, uint64 red_mask) {
  if (i == cells.size()) return 0;
  static int memo[1 << 7][max_cells];
  assert(i < max_cells);
  int &m = memo[red_mask >> max(0, i - 7)][i];
  if (m > 0) return m;
  int res = search_blue(i, red_mask);
  if (cells[i].bit) res = max(res, search_red(i, red_mask));
  return m = res;
}

}  // namespace

int main() {
  for (int r = 0; r < 7; ++r) {
    for (int c = 0; c < 7; ++c) {
      if (valid(r, c)) {
        Cell cell = { r, c, 0 };
        cell.bit = uint64(1) << bits++;
        cells.push_back(cell);
      }
    }
  }

  for (int j = 0; j < cells.size(); ++j) {
    for (int i = 0; i < j; ++i) {
      int dr = cells[i].row - cells[j].row;
      int dc = cells[i].col - cells[j].col;
      if (abs(dr) <= 1 && abs(dc) <= 1 && abs(dr - dc) <= 1) {
        cells[j].neighbours.push_back(&cells[i]);
      }
    }
  }

  cout << search(0, 0) << endl;
}
