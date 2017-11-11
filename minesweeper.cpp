#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include "tile.h"
#include "minesweeper.h"

// Constructors
// constructor with variables
MineSweeper::MineSweeper(int col, int row, int mine_num) {
  this->mine_num_ = mine_num;
  this->col_ = col;
  this->row_ = row;

  this->minefield_ = new Tile*[row];

  for (int r = 0; r < row; ++r) {
    this->minefield_[r] = new Tile[col];
  }

  for (int r = 0; r < row; ++r) {
    for (int c = 0; c < col; ++c) {
      this->minefield_[r][c] = Tile(c, r);
    }
  }

  this->setup_ = true;
}

// copy constructor
MineSweeper::MineSweeper(MineSweeper& m) {
  this->mine_num_ = m.getMineNum();
  this->col_ = m.getCol();
  this->row_ = m.getRow();

  this->minefield_ = new Tile*[this->row_];
  for (int r = 0; r < this->row_; ++r) {
    this->minefield_[r] = new Tile[this->col_];
  }

  for (int r = 0; r < this->row_; ++r) {
    for (int c = 0; c < this->col_; ++c) {
      this->minefield_[r][c] = Tile(m.getTile(c, r));
    }
  }

  this->setup_ = true;
}

// deconstructor
MineSweeper::~MineSweeper()
{
  for (int r = 0; r < this->row_; ++r) {
    delete[] this->minefield_[r];
  }
  delete[] this->minefield_;
}


// Game functions 
// create the minefield!
void MineSweeper::CreateMineField(int col, int row) {
  vector<int> tile_nums;

  for (int r = 0; r < this->row_; ++r) {
    for (int c = 0; c < this->col_; ++c) {
      // change here to change the mine exclusion of the first click
      if (!(r >= row-1 && r <= row+1
            && c >= col-1 && c<= col+1)) {
        tile_nums.push_back(r * this->col_ + c);
      }

      /*  only one tile version
      if (!(r == row && c == col)) {
        tile_nums.push_back(r * this->col_ + c);
      }
      */
    }
  }

  for (int i = 0; i < this->mine_num_; ++i) {
    if (tile_nums.size() > 0) {
      srand(time(NULL));
      int k = (rand() % tile_nums.size());
      auto pair1 = tile_nums[k];

      this->minefield_[pair1 / this->col_][pair1 % this->col_].setMined(true);
      tile_nums.erase(tile_nums.begin() + k);
    }
  }

  for (int r = 0; r < this->row_; ++r) {
    for (int c = 0; c < this->col_; ++c) {
      this->CountMineTile(c, r);
    }
  }

  this->RevealSingleClick(col, row);
}

// print the minefield
void MineSweeper::PrintMineField() {
  for (int c = 0; c < this->col_ * 2 + 1; ++c) {
    cout << "-";
  }
  cout << endl;
  for (int r = 0; r < this->row_; ++r) {
    for (int c = 0; c < this->col_; ++c) {
      this->minefield_[r][c].PrintTile();
    }
    cout << "|" << endl;
    for (int c = 0; c < this->col_ * 2 + 1; ++c) {
      cout << "-";
    }
    cout << endl;
  }
}

// end the game!
// @TODO: add statistics of the game
void MineSweeper::EndGame(bool win) {
  for (int r = 0; r < this->row_; ++r) {
    for (int c = 0; c < this->col_; ++c) {
      this->setRevealedTile(c, r, true);
    }
  }

  this->PrintMineField();

  if (win) {
    cout << "Won!" << endl;
  }
  else {
    cout << "Lost..." << endl;
  }
  this->setGameEnd(true);
}

// check if the game is finished and/or won
void MineSweeper::CheckWin() {
  bool win = true;
  for (int r = 0; r < this->row_; ++r) {
    for (int c = 0; c < this->col_; ++c) {
      if (!this->isRevealedTile(c, r)
          && !this->isMinedTile(c, r)) {
        win = false;
      }
    }
  }
  if (win) {
    this->setGameEnd(true);
    this->setWin(true);
  }
}

// double click emulator
void MineSweeper::RevealDoubleClick(int col, int row) {
  // not revealed -> do nothing
  if (!this->isRevealedTile(col, row)) return;
  if (this->CountFlagTile(col, row) 
      != this->getNeighborCountTile(col, row)) return;

  for (int yoff = -1; yoff <= 1; ++yoff) {
    for (int xoff = -1; xoff <= 1; ++xoff) {
      if (xoff != 0 || yoff != 0) {
        int c = col + xoff;
        int r = row + yoff;

        // within range 
        if (c > -1 && c < this->col_
            && r > -1 && r < this->row_) {
          if (!this->isFlaggedTile(c, r)
              && !this->isDoneTile(c, r)) {
            this->setDoneTile(c, r, true);
            this->RevealSingleClick(c, r);
          }
        }
      }
    }
  }
}

// single click emulator
void MineSweeper::RevealSingleClick(int col, int row) {
  this->setRevealedTile(col, row, true);
  
  if (this->isMinedTile(col, row)) {
    this->setGameEnd(true);
    this->setWin(false);
    return;
  }

  if (this->getNeighborCountTile(col, row) == 0) {
    for (int yoff = -1; yoff <= 1; ++yoff) {
      for (int xoff = -1; xoff <= 1; ++xoff) {
        if (xoff != 0 || yoff != 0) {
          int c = col + xoff;
          int r = row + yoff;
          // within range 
          if (c > -1 && c < this->col_
              && r > -1 && r < this->row_
              && !this->isDoneTile(c, r)) {
            this->setDoneTile(c, r, true);  
            this->RevealSingleClick(c, r);
          }
        }
      }
    }
  }
}

// minesweeper functions

// is series
// is Game Ended?
bool MineSweeper::isGameEnd() {
  return this->game_end_;
}

// is the game won?
bool MineSweeper::isWin() { 
  return this->win_;
}

bool MineSweeper::isSetup() {
  return this->setup_;
}

// get series
// get column
int MineSweeper::getCol() {
  return this->col_;
}

// get row
int MineSweeper::getRow() {
  return this->row_;
}

// get mine_num_
int MineSweeper::getMineNum() {
  return this->mine_num_;
}

string MineSweeper::getMineState() {
  stringstream s;
  for (int r = 0; r < this->row_; ++r) {
    for (int c = 0; c < this->col_; ++c) {
      s << this->getTileState(c, r);
    }
  }
  return s.str();
}

vector<int> MineSweeper::getUntouchedTiles() {
  vector<int> empty_tiles;
  for (int r = 0; r < this->row_; ++r) {
    for (int c = 0; c < this->col_; ++c) {
      if (!this->isRevealedTile(c, r) && !this->isFlaggedTile(c, r)) {
        empty_tiles.push_back(r * this->col_ + c);
      }
    }
  }

  return empty_tiles;
}

vector<int> MineSweeper::getRevealedNeighborsTile(int col, int row) {
  vector<int> revealed_neighbors_list;
  for (int yoff = -1; yoff <= 1; ++yoff) {
    for (int xoff = -1; xoff <= 1; ++xoff) {
      int t_col = col + xoff, t_row = row + yoff;
      if (t_col > -1 && t_col < this->getCol()
          && t_row > -1 && t_row < this->getRow()) {
        if (this->isRevealedTile(t_col, t_row)) {
          revealed_neighbors_list.push_back(t_row * this->getCol() + t_col);
        }
      }
    }
  }
  return revealed_neighbors_list;
}

double MineSweeper::getRandomGuessProb() {
  return double(this->getMineNum() - this->CountAllFlagged()) 
    / double(this->CountAllCovered());
}


// set series
// set GameEnd
void MineSweeper::setGameEnd(bool finished) {
  this->game_end_ = finished;
}

// set the game is won or lost
void MineSweeper::setWin(bool win) {
  this->win_ = win;
}


// count series
// count all covered tiles 
int MineSweeper::CountAllCovered() {
  int total = 0;
  for (int r = 0; r < this->row_; ++r) {
    for (int c = 0; c < this->col_; ++c) {
      if (!this->isRevealedTile(c, r)) ++total;
    }
  }
  return total;
}

// count all flagged tiles 
int MineSweeper::CountAllFlagged() {
  int total = 0;
  for (int r = 0; r < this->row_; ++r) {
    for (int c = 0; c < this->col_; ++c) {
      if (this->isFlaggedTile(c, r)) ++total;
    }
  }
  return total;
}


// for tiles
// get reference of a tile
Tile& MineSweeper::getTile(int col, int row) {
  return this->minefield_[row][col];
}


// is series
// is tile done?
bool MineSweeper::isDoneTile(int col, int row) {
  return this->minefield_[row][col].isDone();
}

bool MineSweeper::isRevealedTile(int col, int row) {
  return this->minefield_[row][col].isRevealed();
}

bool MineSweeper::isFlaggedTile(int col, int row) {
  return this->minefield_[row][col].isFlagged();
}

bool MineSweeper::isMinedTile(int col, int row) {
  return this->minefield_[row][col].isMined();
}

// get series
// get neighborcount of a tile
int MineSweeper::getNeighborCountTile(int col, int row) {
  return this->minefield_[row][col].getNeighborCount();
}

string MineSweeper::getTileState(int col, int row) {
  return this->minefield_[row][col].getTileState();
}


// set series 
// set done for the tile
void MineSweeper::setDoneTile(int col, int row, bool done) {
  this->minefield_[row][col].setDone(done);
}

void MineSweeper::setFlagTile(int col, int row, bool val) {
  this->minefield_[row][col].setFlagged(val);
}

void MineSweeper::setRevealedTile(int col, int row, bool val) {
  this->minefield_[row][col].setRevealed(val);
}

void MineSweeper::setMineTile(int col, int row, bool val) {
  this->minefield_[row][col].setMined(val);
}

void MineSweeper::setNeighborCountTile(int col, int row, int val) {
  this->minefield_[row][col].setNeighborCount(val);
}


// count series
// count nearing mines for a tile
void MineSweeper::CountMineTile(int col, int row) {
  // if the tile is mine 
  if (this->isMinedTile(col, row)) {
    this->setNeighborCountTile(col, row, -1);
    return;
  }

  // not mine
  int total = 0;
  for (int yoff = -1; yoff <= 1; ++yoff) {
    for (int xoff = -1; xoff <= 1; ++xoff) {
      int c = col + xoff;
      int r = row + yoff;

      if (c > -1 && c < this->col_
          && r > -1 && r < this->row_) {
        if (this->isMinedTile(c, r)) ++total;
      }
    }
  }
  this->setNeighborCountTile(col, row, total);
}

// count nearing flags for a tile 
int MineSweeper::CountFlagTile(int col, int row) {
  int total = 0;
  for (int yoff = -1; yoff <= 1; ++yoff) {
    for (int xoff = -1; xoff <= 1; ++xoff) {
      if (xoff != 0 || yoff != 0) {
        int c = col + xoff;
        int r = row + yoff;

        if (c > -1 && c < this->col_
            && r > -1 && r < this->row_) {
          if (this->isFlaggedTile(c, r)) ++total;    
        }
      }
    }
  }
  return total;
}

// count nearing covered tiles 
int MineSweeper::CountCoveredTile(int col, int row) {
  int total = 0;
  for (int yoff = -1; yoff <= 1; ++yoff) {
    for (int xoff = -1; xoff <= 1; ++xoff) {
      if (xoff != 0 || yoff != 0) {
        int c = col + xoff;
        int r = row + yoff;

        if (c > -1 && c < this->col_
            && r > -1 && r < this->row_) {
          if (!this->isRevealedTile(c, r)) ++total;
        }
      }
    }
  }
  return total;
}
