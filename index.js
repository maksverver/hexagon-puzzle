const scoreElem = document.getElementById('score');
const gridElem = document.getElementById('grid');

class ScoreTracker {
  constructor(elem) {
    this.elem = elem;
    this.score = 0;
  }

  update(diff) {
    if (diff) {
      this.score += diff;
      this.elem.textContent = this.score;
    }
  }
}

class Tile {
  constructor(elem, scoreTracker) {
    this.elem = elem;
    this.scoreTracker = scoreTracker;
    this.neighbors = [];
    this.red = false;
    this.score = 0;
    elem.classList.add('tile', 'blue');
  }

  toggle() {
    this.elem.classList.remove(this.red ? 'red' : 'blue');
    this.red = !this.red;
    this.elem.classList.add(this.red ? 'red' : 'blue');
    this.updateScore();
    for (let neighbor of this.neighbors) {
      neighbor.updateScore();
    }
  }

  updateScore() {
    let score = 0;
    if (!this.red) {
      score = 2;
      for (let neighbor of this.neighbors) {
        if (neighbor.red) ++score;
      }
    }
    this.scoreTracker.update(score - this.score);
    this.elem.textContent = score;
    this.score = score;
  }
}

function encodeState(tiles) {
  // Can't use bitwise operations here, because we need more than 32 bits!
  let mask = 0;
  let bit = 1;
  for (let tile of tiles) {
    if (tile.red) mask += bit;
    bit += bit;

  }
  return mask;
}

function decodeState(tiles, mask) {
  // Can't use bitwise operations here, because we need more than 32 bits!
  for (let tile of tiles) {
    if (tile.red != mask % 2) tile.toggle();
    mask = (mask - mask % 2) / 2;
  }
}

function initialize() {
  function valid(row, col) {
    return row >= 0 && row < 7 && col >= 0 && col < 7 && Math.abs((row - 3) - (col - 3)) <= 3;
  }
  const scoreTracker = new ScoreTracker(scoreElem);
  const grid = [];
  const tiles = [];

  // Create all tiles
  for (let row = 0; row < 7; ++row) {
    grid[row] = [];
    for (let col = 0; col < 7; ++col) {
      if (valid(row, col)) {
        const tileElem = document.createElement('div');
        tileElem.style.gridRow = (1 + 2*row + (3 - col)) + ' / span 2';
        tileElem.style.gridColumn =  (1 + 2*col) + ' / span 3';
        let tile = new Tile(tileElem, scoreTracker);
        grid[row][col] = tile;
        tiles.push(tile);
        tileElem.onclick = () => {
          tile.toggle();
          window.location.hash = '#tiles=' + encodeState(tiles);
        }
        gridElem.appendChild(tileElem);
      }
    }
  }

  // Decode tile state encoded in the URL.
  let match = window.location.hash.match(/[#&]tiles=(\d+)/);
  if (match) decodeState(tiles, parseInt(match[1], 10));

  // Find tile neighbors
  for (let row = 0; row < 7; ++row) {
    for (let col = 0; col < 7; ++col) {
      if (valid(row, col)) {
        for (let dr = -1; dr <= 1; ++dr) {
          for (let dc = -1; dc <= 1; ++dc) {
            if ((dr != 0 || dc != 0) && Math.abs(dr - dc) <= 1 && valid(row + dr, col + dc)) {
              grid[row][col].neighbors.push(grid[row + dr][col + dc]);
            }
          }
        }
        grid[row][col].updateScore();
      }
    }
  }
}
