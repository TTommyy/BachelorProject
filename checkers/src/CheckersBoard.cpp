#include "../inc/CheckersBoard.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include "../inc/Move.h"

namespace
{
 const auto directions = std::vector<std::pair<int8_t, int8_t>>
  {
    {1, -1},  // upper left
    {1, 1},   // upper right
    {-1, 1},  // lower right
    {-1, -1}  // lower left
  };
}


CheckersBoard::CheckersBoard()
{
  intializeBoard();
}

std::string CheckersBoard::toString() const
{
  std::stringstream ss;
  for (const auto& row: m_board)
  {
    for (const auto& cell : row)
    {
      ss << static_cast<int>(cell) << ",";
    }
    ss << "\n";
  }
  return ss.str();
}

std::string CheckersBoard::show() const
{
  std::stringstream ss;

  auto row = static_cast<int32_t>(BOARD_SIZE - 1);
  for (auto it = m_board.rbegin(); it < m_board.rend(); ++it)
  {
    ss << row-- << "| "; 
    for (const auto& cell : *it)
    {
      auto p = static_cast<int>(cell); 
      if (p == -1) ss << "8,";
      else if (p == 1) ss << "1,";
      else if (p == -3) ss << "9,";
      else if (p == 3) ss << "2,";
      else ss << static_cast<int>(cell) << ",";
    }
    ss << "\n";
  }

  ss << "  ";
  for (auto col = 0; col < BOARD_SIZE ; col++)
  {
    ss << "--";
  }
  ss << "\n   ";
  for (auto col = 0; col < BOARD_SIZE; col++)
  {
    ss << col << " ";
  }
  ss << "\n";

  std::cout << ss.str();
  return ss.str();
}

std::vector<Move> CheckersBoard::getPossibleMoves(const PlayerE& player) const
{
  std::vector<Move> moves;
  uint8_t mostCaptured = 0;
  if (player == PlayerE::White)
  {
    // check man can kill
    // only most numbers of kills are vaild moves
    // kiling four directions
    // check man move
    for (uint8_t y = 0; y < BOARD_SIZE; ++y)
    {
      for (uint8_t x = 0; x < BOARD_SIZE; ++x)
      {
        // check man kill
        if (m_board[y][x] == FigureTypeE::WhiteMan)
        {
          // [end_field, captures_by_the_way]
          auto cap_vec = checkIfManCanKill(y,x, player, std::vector<Move::Field>(), m_board);
          for (const auto &pair: cap_vec)
          {
            if (pair.second.size() > mostCaptured)
            {
              mostCaptured = pair.second.size();
            }
          }
          cap_vec.erase(std::remove_if(cap_vec.begin(), cap_vec.end(), 
            [mostCaptured](const auto& pair)
              {
                return pair.second.size() < mostCaptured;
              }),
            cap_vec.end());
          for(const auto &pair: cap_vec)
          {
            moves.emplace_back(std::make_pair(y, x), pair.first, pair.second);
          }
        }

        if (m_board[y][x] == FigureTypeE::WhiteKing)
        {
          // [end_field, captures_by_the_way]
          auto cap_vec = checkIfKingCanKill(y,x, player, std::vector<Move::Field>(), m_board);
          for (const auto &pair: cap_vec)
          {
            if (pair.second.size() > mostCaptured)
            {
              mostCaptured = pair.second.size();
            }
          }
          cap_vec.erase(std::remove_if(cap_vec.begin(), cap_vec.end(), 
            [mostCaptured](const auto& pair)
              {
                return pair.second.size() < mostCaptured;
              }),
            cap_vec.end());
          for(const auto &pair: cap_vec)
          {
            moves.emplace_back(std::make_pair(y, x), pair.first, pair.second);
          }
        }
      }
    }

    moves.erase(std::remove_if(moves.begin(), moves.end(), 
      [mostCaptured](const auto& move)
      {
        return move.getNumberOfCaptured() < mostCaptured;
      }), moves.end());
    
    if (mostCaptured != 0)
    {
      return moves;
    }


    // check man move
    for(uint8_t y = 0; y < BOARD_SIZE; ++y)
    {
      for(uint8_t x = 0; x < BOARD_SIZE; ++x)
      {
        // check man move
        if (m_board[y][x] == FigureTypeE::WhiteMan)
        {
          if (y + 1 >= BOARD_SIZE)
          {
            continue;
          }
          // left
          if (checkIfEmpty(y+1, x-1, m_board))
          {
            Move m({y,x}, {y+1, x-1}, {});
            moves.push_back(m);
          }
          // right
          if (checkIfEmpty(y+1, x+1, m_board))
          {
            Move m({y,x}, {y+1, x+1}, {});
            moves.push_back(m);
          }
        }

        else if (m_board[y][x] == FigureTypeE::WhiteKing)
        {
          auto kingMoves = checkIfKingCanMove(y, x);
          moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());
        }
      }
    }
  }

  else if (player == PlayerE::Black)
  {
    // check man can kill
    // only most numbers of kills are vail moves
    // kiling both directions
    // check man move
    for (uint8_t y = 0; y < BOARD_SIZE; ++y)
    {
      for (uint8_t x = 0; x < BOARD_SIZE; ++x)
      {
        // check man kill
        if (m_board[y][x] == FigureTypeE::BlackMan)
        {
          // [end_field, captures_by_the_way]
          auto cap_vec = checkIfManCanKill(y,x, player, std::vector<Move::Field>(), m_board);
          for (const auto &pair: cap_vec)
          {
            if (pair.second.size() > mostCaptured)
            {
              mostCaptured = pair.second.size();
            }
          }
          cap_vec.erase(std::remove_if(cap_vec.begin(), cap_vec.end(), 
            [mostCaptured](const auto& pair)
              {
                return pair.second.size() < mostCaptured;
              }),
            cap_vec.end());
          for(const auto &pair: cap_vec)
          {
            moves.emplace_back(std::make_pair(y, x), pair.first, pair.second);
          }
        }

        if (m_board[y][x] == FigureTypeE::BlackKing)
        {
          // [end_field, captures_by_the_way]
          auto cap_vec = checkIfKingCanKill(y,x, player, std::vector<Move::Field>(), m_board);
          for (const auto &pair: cap_vec)
          {
            if (pair.second.size() > mostCaptured)
            {
              mostCaptured = pair.second.size();
            }
          }
          cap_vec.erase(std::remove_if(cap_vec.begin(), cap_vec.end(), 
            [mostCaptured](const auto& pair)
              {
                return pair.second.size() < mostCaptured;
              }),
            cap_vec.end());
          for(const auto &pair: cap_vec)
          {
            moves.emplace_back(std::make_pair(y, x), pair.first, pair.second);
          }
        }
      }
    }

    moves.erase(std::remove_if(moves.begin(), moves.end(), 
      [mostCaptured](const auto& move)
      {
        return move.getNumberOfCaptured() < mostCaptured;
      }), moves.end());
    
    if (mostCaptured != 0)
    {
      return moves;
    }

    // check man move
    for(uint8_t y = 0; y < BOARD_SIZE; ++y)
    {
      for(uint8_t x = 0; x < BOARD_SIZE; ++x)
      {
        // check man move
        if (m_board[y][x] == FigureTypeE::BlackMan)
        {
          if (y - 1 < 0)
          {
            continue;
          }
          // left
          if (checkIfEmpty(y-1, x-1, m_board))
          {
            Move m({y,x}, {y-1, x-1}, {});
            moves.push_back(m);
          }
          // right
          if (checkIfEmpty(y-1, x+1, m_board))
          {
            Move m({y,x}, {y-1, x+1}, {});
            moves.push_back(m);
          }
        }

        else if (m_board[y][x] == FigureTypeE::BlackKing)
        {
          auto kingMoves = checkIfKingCanMove(y, x);
          moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());
        }
      }
    }
  }

  return moves;
}

CheckersBoardIf::Board CheckersBoard::getBoard() const
{
  return m_board;
}

void CheckersBoard::setBoard(CheckersBoardIf::Board board)
{
  m_board = board;
}

void CheckersBoard::intializeBoard()
{
  for(auto& row: m_board)
  {
    std::fill(row.begin(), row.end(), FigureTypeE::Empty);
  }

  // Initialize white pieces

  for (int x = 0; x < BOARD_SIZE; x += 2)
  {
    m_board[0][x] = FigureTypeE::WhiteMan;
  }

  for (int x = 1; x < BOARD_SIZE; x += 2)
  {
    m_board[1][x] = FigureTypeE::WhiteMan;
  }

  for (int x = 0; x < BOARD_SIZE; x += 2)
  {
    m_board[2][x] = FigureTypeE::WhiteMan;
  }


  // Initialize black pieces
  for(int x = 1; x < BOARD_SIZE; x += 2)
  { 
    m_board[5][x] = FigureTypeE::BlackMan;
  }

  for(int x = 0; x < BOARD_SIZE; x += 2)
  { 
    m_board[6][x] = FigureTypeE::BlackMan;
  }

  for (int x = 1; x < BOARD_SIZE; x += 2)
  {
    m_board[7][x] = FigureTypeE::BlackMan;
  }

}

CheckersBoard::CapcturesVector CheckersBoard::checkIfManCanKill(uint8_t y, uint8_t x, const PlayerE& player, std::vector<Move::Field> cap, Board board) const
{
  CapcturesVector res;
  const PlayerE enemy = player == PlayerE::White? PlayerE::Black : PlayerE::White;

  // upper left
  if (checkIfOnField(y+1, x-1, enemy, board) && checkIfEmpty(y+2, x-2, board))
  {
    auto new_cap = cap;
    new_cap.push_back({y+1, x-1});
    res.push_back(std::make_pair(std::make_pair(y+2, x-2), new_cap));

    auto new_board = board;
    new_board[y+1][x-1] = FigureTypeE::Empty;
    new_board[y][x] = FigureTypeE::Empty;
    new_board[y+2][x-2] = FigureTypeE::WhiteMan;

    auto recur = checkIfManCanKill(y+2, x-2, player, new_cap, new_board);
    res.insert(res.end(), recur.begin(), recur.end());
  }

  // upper right
  if (checkIfOnField(y+1, x+1, enemy, board) && checkIfEmpty(y+2, x+2, board))
  {
    auto new_cap = cap;
    new_cap.push_back({y+1, x+1});
    res.push_back(std::make_pair(std::make_pair(y+2, x+2), new_cap));

    auto new_board = board;
    new_board[y+1][x+1] = FigureTypeE::Empty;
    new_board[y][x] = FigureTypeE::Empty;
    new_board[y+2][x+2] = FigureTypeE::WhiteMan;

    auto recur = checkIfManCanKill(y+2, x+2, player, new_cap, new_board);
    res.insert(res.end(), recur.begin(), recur.end());
  }

  // lower right
  if (checkIfOnField(y-1, x+1, enemy, board) && checkIfEmpty(y-2, x+2, board))
  {
    auto new_cap = cap;
    new_cap.push_back({y-1, x+1});
    res.push_back(std::make_pair(std::make_pair(y-2, x+2), new_cap));

    auto new_board = board;
    new_board[y-1][x+1] = FigureTypeE::Empty;
    new_board[y][x] = FigureTypeE::Empty;
    new_board[y-2][x+2] = FigureTypeE::WhiteMan;

    auto recur = checkIfManCanKill(y-2, x+2, player, new_cap, new_board);
    res.insert(res.end(), recur.begin(), recur.end());
  }

  // lower left
  if (checkIfOnField(y-1, x-1, enemy, board) && checkIfEmpty(y-2, x-2, board))
  {
    auto new_cap = cap;
    new_cap.push_back({y-1, x-1});
    res.push_back(std::make_pair(std::make_pair(y-2, x-2), new_cap));

    auto new_board = board;
    new_board[y-1][x-1] = FigureTypeE::Empty;
    new_board[y][x] = FigureTypeE::Empty;
    new_board[y-2][x-2] = FigureTypeE::WhiteMan;

    auto recur = checkIfManCanKill(y-2, x-2, player, new_cap, new_board);
    res.insert(res.end(), recur.begin(), recur.end());
  }

  return res;
}

bool CheckersBoard::validField(uint8_t y,  uint8_t x) const
{
  return 0 <= y && y < BOARD_SIZE
      && 0 <= x && x < BOARD_SIZE;
}

bool CheckersBoard::checkIfOnField(uint8_t y, uint8_t x, PlayerE player, const Board& board) const
{

  if (!validField(y,x))
  {
    return false;
  }

  const auto field = board[y][x];
  if (player == PlayerE::White)
  {
    return field == FigureTypeE::WhiteMan || field == FigureTypeE::WhiteKing;
  }

  return field == FigureTypeE::BlackMan || field == FigureTypeE::BlackKing;
}

bool CheckersBoard::checkIfEmpty(uint8_t y, uint8_t x, const Board& board) const
{
  return validField(y, x) && board[y][x] == FigureTypeE::Empty;
}

// KingsLogic
CheckersBoard::CapcturesVector CheckersBoard::checkIfKingCanKill(uint8_t y, uint8_t x, const PlayerE& player, std::vector<Move::Field> cap, Board board) const
{
  CapcturesVector res;
  const PlayerE enemy = player == PlayerE::White ? PlayerE::Black : PlayerE::White;

  for (const auto& [dy, dx] : directions)
  {
    uint8_t newY = y;
    uint8_t newX = x;
    bool foundCapture = false;

    while (validField(newY + dy, newX + dx))
    {
      newY += dy;
      newX += dx;

      if (checkIfOnField(newY, newX, player, board))
      {
        break;
      }

      if (checkIfOnField(newY, newX, enemy, board))
      {
        foundCapture = true;
        break;
      }
    }

    if (foundCapture) 
    {
      uint8_t captureY = newY;
      uint8_t captureX = newX;
      newY += dy;
      newX += dx;
      while (checkIfEmpty(newY, newX, board))
      {
        auto new_cap = cap;
        new_cap.push_back({captureY, captureX});
        res.push_back(std::make_pair(std::make_pair(newY, newX), new_cap));

        auto new_board = board;
        new_board[captureY][captureX] = FigureTypeE::Empty;
        new_board[y][x] = FigureTypeE::Empty;
        new_board[newY][newX] = FigureTypeE::WhiteKing;

        auto recur = checkIfKingCanKill(newY, newX, player, new_cap, new_board);
        res.insert(res.end(), recur.begin(), recur.end());

        newY += dy;
        newX += dx;
      }
    }
  }

  return res;
}

std::vector<Move> CheckersBoard::checkIfKingCanMove(uint8_t y, uint8_t x) const
{
  std::vector<Move> res;

  for (const auto& [dy, dx] : directions)
  {
    auto newY = y;
    auto newX = x;
    while (checkIfEmpty(newY + dy, newX + dx, m_board))
    {
      newY += dy;
      newX += dx;
      Move m = Move({y, x}, {newY, newX}, {});
      res.push_back(m);
    }
  }

  return res;
}