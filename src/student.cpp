#include "student.hpp"
#include <iostream>

Student::Student(const std::string &name, const std::string &stu_id,
                 bool gender_is_male, const Score &stu_scores) {
  this->name = name;
  this->stu_id = stu_id;
  this->gender_is_male = gender_is_male;
  this->stu_scores = stu_scores;
}

Student *Student::createStu() { return new Student(); }

Student *Student::createStu(const std::string &name, const std::string &stu_id,
                            bool gender_is_male, const Score &stu_scores) {
  return new Student(name, stu_id, gender_is_male, stu_scores);
}

void Student::setBaseInfo(const std::string &name, const std::string &stu_id,
                          bool gender_is_male) {
  this->name = name;
  this->stu_id = stu_id;
  this->gender_is_male = gender_is_male;
}

bool Student::setScores(float cn, float ma, float en) {
  bool ret = true;
  auto IsScoreValid = [](float score, int limit) {
    return score <= limit ? true : false;
  };

  if (IsScoreValid(cn, CN_FULL_SCORE))
    this->stu_scores.chinese = cn;
  else {
    std::cout << "Invalid Chinese Score.\n";
    ret = false;
  }

  if (IsScoreValid(ma, MA_FULL_SCORE))
    this->stu_scores.math = ma;
  else {
    std::cout << "Invalid Math Score.\n";
    ret = false;
  }

  if (IsScoreValid(en, EN_FULL_SCORE))
    this->stu_scores.english = en;
  else {
    std::cout << "Invalid English Score.\n";
    ret = false;
  }

  return ret;
}

void Student::showInfo() const {
  std::cout << "======================= Student ID =======================\n";
  std::cout << "  Name: \t" << name << "\n";
  std::cout << "  ID: \t\t" << stu_id << "\n";
  std::cout << "  Gender: \t" << (gender_is_male ? "Male" : "Female") << "\n";

  std::cout << "========================= Scores =========================\n";
  std::cout << "  Chinese: \t" << stu_scores.chinese << "\n";
  std::cout << "  Math: \t" << stu_scores.math << "\n";
  std::cout << "  English: \t" << stu_scores.english << "\n";
  std::cout << "==========================================================\n";
}