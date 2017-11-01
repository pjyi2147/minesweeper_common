#pragma once

class Tile 
{
private:
  // universal
  bool mined_ = false;
  bool revealed_ = false;
  bool flagged_ = false;

  int neighbor_count_ = 0;
  int col_;
  int row_;

  // for Client (AI)
  bool done_ = false;

public:
  // universal
  Tile();
  Tile(int col, int row);
  Tile(Tile& t);

  void PrintTile();
  
  bool isMined();
  bool isRevealed();
  bool isFlagged();
  bool isDone();
  
  int getRow();
  int getCol();
  int getNeighborCount();
  std::string getTileState();
  
  void setMined(bool);
  void setFlagged(bool);
  void setNeighborCount(int count);
  void setRevealed(bool);
  void setDone(bool done);
};