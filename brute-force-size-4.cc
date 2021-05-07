// Brute force solution for a hexagon of size 4 (diameter 7)

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

const int num_threads = 12;
const int min_score = 110;

static vector<Cell> cells;
static int bits;

namespace {

bool valid(int row, int col) {
  return row >= 0 && row < 7 && col >= 0 && col < 7 && abs((row - 3) - (col - 3)) <= 3;
}

bool centre(int row, int col) { return row == 3 && col == 3; }

int score(const Cell& cell, uint64 red_mask) {
  if (red_mask & cell.bit) return 0;
  int res = 2;
  for (const Cell* n : cell.neighbours) if (red_mask & n->bit) ++res;
  return res;
}

int score(uint64 red_mask) {
  int res = 0;
  for (const Cell& cell : cells) res += score(cell, red_mask);
  return res;
}

void plot(uint64 red_mask) {
  char grid[8*7 + 1];
  for (int r = 0; r < 7; ++r) {
    for (int c = 0; c < 7; ++c) grid[8*r + c] = ' ';
    grid[8*r + 7] = '\n';
  }
  grid[8*7] = '\0';
  for (const Cell& cell : cells) {
    grid[8*cell.row + cell.col] = '0' + score(cell, red_mask);
  }
  cerr << "Score: " << score(red_mask) << '\n' << grid << endl;
}

void *search_thread(void *arg) {
  for (uint64 red_mask = *(int*)arg, limit = uint64(1) << bits;
       red_mask < limit; red_mask += num_threads) {
    if (score(red_mask) >= min_score) plot(red_mask);
    if ((red_mask & 0xfffffff) == 0) {
      std::cerr << 100.0 * red_mask / (uint64(1) << bits) << '%' << endl;;
    }
  }
  return nullptr;
}

}  // namespace

int main() {
  for (int r = 0; r < 7; ++r) {
    for (int c = 0; c < 7; ++c) {
      if (valid(r, c)) {
        Cell cell = { r, c, 0 };
        if (!centre(r, c)) cell.bit = uint64(1) << bits++;
        cells.push_back(cell);
      }
    }
  }

  for (int j = 0; j < cells.size(); ++j) {
    for (int i = 0; i < j; ++i) {
      int dr = cells[i].row - cells[j].row;
      int dc = cells[i].col - cells[j].col;
      if (abs(dr) <= 1 && abs(dc) <= 1 && abs(dr - dc) <= 1) {
        cells[i].neighbours.push_back(&cells[j]);
        cells[j].neighbours.push_back(&cells[i]);
      }
    }
  }

  vector<int> offsets;
  for (int i = 0; i < num_threads; ++i) offsets.push_back(i);
  vector<pthread_t> threads;
  for (int i = 0; i < num_threads; ++i) {
    pthread_t thread;
    int res = pthread_create(&thread, nullptr, search_thread, &offsets[i]);
    assert(res == 0);
    threads.push_back(thread);
  }
  for (pthread_t thread : threads) pthread_join(thread, nullptr);
}
