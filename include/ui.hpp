#pragma once
#include "manager.hpp"
#include <functional>
#include <iostream>
#include <limits>
#include <map>

class Ui {
private:
  Ui();
  Ui(const Ui &ui) = delete;

  static Ui *_instance;

  Manager::ScoreManager *pScoreManager;
  bool isExit;

  void AddStudent();
  void DeleteStudent();
  void ModifyStudent();
  void FindStudent();
  void ShowListOfStudents();
  void SortStudent();
  void SaveToExcel();
  void Exit();

public:
  ~Ui();
  std::map<int, std::function<void()>> opMap;
  static Ui *Instance();
  void showMenu() const;

  template <typename T> T getUsersKey(const std::string &prompt) {
    T val;
    std::cout << prompt;
    while (!(std::cin >> val)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Please enter valid elements!\n" << prompt;
    }
    return val;
  }
  void Scheduling();
};