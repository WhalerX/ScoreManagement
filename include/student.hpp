#pragma once
#include <string>

#define CN_FULL_SCORE 150
#define MA_FULL_SCORE 200
#define EN_FULL_SCORE 120

struct Score {
  float chinese;
  float math;
  float english;

  Score() = default;
  Score(float cn, float ma, float en) : chinese(cn), math(ma), english(en) {}
  float total() const { return chinese + math + english; }
};

class Student {
private:
  std::string name;
  std::string stu_id;
  bool gender_is_male;

  Score stu_scores;

private:
  Student() = default;
  Student(const std::string &name, const std::string &stu_id,
          bool gender_is_male, const Score &stu_scores);
  Student(Student &stu) = default;

public:
  ~Student() = default;
  static Student *createStu();
  static Student *createStu(const std::string &name, const std::string &stu_id,
                     bool gender_is_male, const Score &stu_scores);
  void setBaseInfo(const std::string &name, const std::string &stu_id,
                   bool gender_is_male);
  bool setScores(float cn, float ma, float en);
  const Score &getScore() const { return stu_scores; }
  const std::string &getName() const { return this->name; }
  const std::string &getStuId() const { return this->stu_id; }
  bool getGender() const { return gender_is_male; }

  void showInfo() const;
};