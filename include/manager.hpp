#pragma once
#include "student.hpp"
#include <functional>
#include <vector>

namespace Manager {
enum sortDirection { ASCEND, DESCEND };

class ScoreManager {
private:
  std::vector<Student *> *pStuInfoList;

private:
  ScoreManager();
  ~ScoreManager();
  ScoreManager(const ScoreManager &) = delete;

  static ScoreManager *_instance;

public:
  static ScoreManager *Instance();
  static void releaseInstance();
  bool addStu(const Student *stu);
  int addStu(const std::vector<Student *> *pStuInfoList);
  bool delStu(const Student *stu);
  int delStu(const std::vector<Student *> *pStuInfoListWaitForDel);
  bool modifyStu(Student *stu, const std::string &name, bool gender_is_male);
  bool modifyStu(Student *stu, const Score &score);
  std::vector<Student *> findStuByName(const std::string &name);
  Student *findStuByID(const std::string &stu_id);
  void sortStuByID(sortDirection t);
  void sortStuByScore(sortDirection t);
  void showStuInfoList() const;
  int showStuNum() const { return pStuInfoList->size(); }

  static std::function<bool(const std::string &id,
                            const std::vector<Student *> *pStuInfoList)>
      isStudentExist;

  static std::function<Student *(const std::string &id,
                                 const std::vector<Student *> *pStuInfoList)>
      pStudentExist;
};

} // namespace Manager