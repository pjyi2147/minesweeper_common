#pragma once

#include "tile.h"

using namespace std;

class MineSweeper
{
private:
  // universal
  int col_;
  int row_;
  int mine_num_;
  Tile** minefield_;

  bool game_end_ = false;
  bool win_ = false;

  // client only
  bool done_ = false;
  bool setup_ = false;

public:
  // universal 
  MineSweeper(int col, int row, int mine_num);       
  MineSweeper(MineSweeper& m);                      
  ~MineSweeper();                                   

  // game functions
  void CreateMineField(int col, int row);           
  void PrintMineField();                            
  void EndGame(bool win);                           
  void CheckWin();                                  

  void RevealDoubleClick(int col, int row);         
  
  // for minesweeper
  bool isGameEnd();                                 
  bool isWin();                                     
  bool isSetup();
  
  int getCol();                                     
  int getRow();                                     
  int getMineNum();                                 
  string getMineState();

  void setGameEnd(bool finished);
  void setWin(bool win);

  int CountAllCovered();
  int CountAllFlagged();                            

  // for tiles in minesweeper
  Tile& getTile(int col, int row);
  
  bool isDoneTile(int col, int row);
  bool isRevealedTile(int col, int row);
  bool isFlaggedTile(int col, int row);
  bool isMinedTile(int col, int row);

  int getNeighborCountTile(int col, int row);
  string getTileState(int c, int r);
  
  void setDoneTile(int col, int row, bool done);
  void setFlagTile(int col, int row, bool val);
  void setRevealedTile(int col, int row, bool val);
  void setMineTile(int col, int row, bool val);
  void setNeighborCountTile(int col, int row, int val);

  void CountMineTile(int col, int row);   // used to set neighborcount  
  int CountFlagTile(int col, int row);    
  int CountCoveredTile(int col, int row);
};