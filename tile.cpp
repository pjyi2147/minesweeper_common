#include <iostream>
#include <vector>
#include <string>

#include "tile.h"

using namespace std;

// A default constructor
Tile::Tile() {
  this->col_ = 0;
  this->row_ = 0;
}

Tile::Tile(int col, int row) {
  this->col_ = col;
  this->row_ = row;
}

// copy constructor 
Tile::Tile(Tile& t)
{
  this->mined_ = t.isMined();
  this->revealed_ = t.isRevealed();
  this->flagged_ = t.isFlagged();

  this->neighbor_count_ = t.getNeighborCount();
  this->done_ = t.isDone();

  this->col_ = t.getCol();
  this->row_ = t.getRow();
}


// print functions
// print the tile status
void Tile::PrintTile() {
  cout << "|";
  if (this->revealed_ && this->mined_) cout << "M";
  else if (this->revealed_) cout << this->neighbor_count_;
  else if (this->flagged_) cout << "F";
  else cout << "C";
}


// is series
// Is the tile mined?
bool Tile::isMined() {
  return this->mined_;
}

// Is the tile revealed?
bool Tile::isRevealed() {
  return this->revealed_;
}

// Is the tile flagged?
bool Tile::isFlagged() {
  return this->flagged_;
}

bool Tile::isDone() {
  return this->done_;
}


// get series
// get tile's row
int Tile::getRow() {
  return this->row_;
}

// get tile's column
int Tile::getCol() {
  return this->col_;
}

// get tile's neighborcount
int Tile::getNeighborCount() {
  return this->neighbor_count_;
}

// return the status as string
std::string Tile::getTileState()
{
  if (this->revealed_ && this->mined_) return "M";
  else if (this->revealed_)
    return to_string(this->neighbor_count_);
  else if (this->flagged_) return "F";			// Flag
  else return "C";								// Covered
}


// set series
// set mine
void Tile::setMined(bool val) {
  this->mined_ = val;
}

// set flag
void Tile::setFlagged(bool val) {
  this->flagged_ = val;
}

void Tile::setNeighborCount(int count) {
  this->neighbor_count_ = count;
}

void Tile::setRevealed(bool val) {
  this->revealed_ = val;
}

void Tile::setDone(bool done) {
  this->done_ = done;
}