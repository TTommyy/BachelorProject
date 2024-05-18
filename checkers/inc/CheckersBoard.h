#pragma once

#include "CheckersBoardIf.h"

class CheckersBoard : public CheckersBoardIf
{
public:
using CapcturesVector = std::vector<std::pair<Move::Field ,std::vector<Move::Field>>>;
  CheckersBoard();
  std::string toString() const override;
  std::vector<Move> getPossibleMoves(const PlayerE&) const override;
  std::string show() const override;
  Board getBoard() const override;
  void setBoard(Board) override;

private:
  void intializeBoard();
  CapcturesVector checkIfManCanKill(uint8_t, uint8_t, const PlayerE&, std::vector<Move::Field>, Board) const;
  bool validField(uint8_t, uint8_t) const;
  bool checkIfOnField(uint8_t, uint8_t, const PlayerE, const Board&) const;
  bool checkIfEmpty(uint8_t, uint8_t, const Board&) const;
  Board m_board;
};