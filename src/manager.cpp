#include "manager.hpp"
#include "student.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace Manager {

ScoreManager *ScoreManager::_instance = nullptr;

ScoreManager *ScoreManager::Instance() {
  if (_instance == nullptr) {
    _instance = new ScoreManager;
  }
  return _instance;
}

void ScoreManager::releaseInstance() {
  if (_instance != nullptr) {
    delete _instance;
    _instance = nullptr;
  }
}

std::function<bool(const std::string &, const std::vector<Student *> *)>
    ScoreManager::isStudentExist =
        [](const std::string &id,
           const std::vector<Student *> *pStuInfoList) -> bool {
  auto it = std::find_if(
      pStuInfoList->begin(), pStuInfoList->end(),
      [&id](const Student *stuptr) { return id == stuptr->getStuId(); });

  return it != pStuInfoList->end();
};

std::function<Student *(const std::string &, const std::vector<Student *> *)>
    ScoreManager::pStudentExist =
        [](const std::string &id,
           const std::vector<Student *> *pStuInfoList) -> Student * {
  auto it = std::find_if(
      pStuInfoList->begin(), pStuInfoList->end(),
      [&id](const Student *stuptr) { return id == stuptr->getStuId(); });
  if (it != pStuInfoList->end())
    return *it;
  else
    return nullptr;
};

ScoreManager::ScoreManager() { pStuInfoList = new std::vector<Student *>; }

ScoreManager::~ScoreManager() {
  for (auto pStu : *pStuInfoList) {
    delete pStu;
  }
  delete pStuInfoList;
}

bool ScoreManager::addStu(const Student *stu) {
  if (isStudentExist(stu->getStuId(), this->pStuInfoList)) {
    std::cout << "Student " << stu->getStuId()
              << " already existed! Cannot add again.\n";
    return false;
  }
  this->pStuInfoList->push_back((Student *)stu);
  return true;
}

int ScoreManager::addStu(const std::vector<Student *> *pStuInfoList) {
  int success_add = 0;
  for (auto pStu : *pStuInfoList) {
    if (isStudentExist(pStu->getStuId(), this->pStuInfoList)) {
      std::cout << "Student " << pStu->getStuId()
                << " already existed! Cannot add again.\n";
      continue;
    }
    this->pStuInfoList->push_back(pStu);
    success_add++;
  }
  return success_add;
}

bool ScoreManager::delStu(const Student *stu) {
  if (pStuInfoList->size() == 0 ||
      !isStudentExist(stu->getStuId(), this->pStuInfoList)) {
    std::cout << "Student " << stu->getStuId()
              << " do not exist! Cannot delete.\n";
    return false;
  }
  pStuInfoList->erase(std::remove_if(pStuInfoList->begin(), pStuInfoList->end(),
                                     [stu](const Student *stuptr) {
                                       return stu->getStuId() ==
                                              stuptr->getStuId();
                                     }),
                      pStuInfoList->end());
  return true;
}

int ScoreManager::delStu(const std::vector<Student *> *pStuInfoListWaitForDel) {
  int success_delete = 0;
  for (auto pStu : *pStuInfoListWaitForDel) {
    if (pStuInfoList->size() == 0 ||
        !isStudentExist(pStu->getStuId(), this->pStuInfoList)) {
      std::cout << "Student " << pStu->getStuId()
                << " do not exist! Cannot delete.\n";
      continue;
    }
    pStuInfoList->erase(
        std::remove_if(pStuInfoList->begin(), pStuInfoList->end(),
                       [pStu](const Student *stuptr) {
                         return pStu->getStuId() == stuptr->getStuId();
                       }),
        pStuInfoList->end());
    success_delete++;
  }
  return success_delete;
}

bool ScoreManager::modifyStu(Student *stu, const std::string &name,
                             bool gender_is_male) {
  if (pStuInfoList->size() == 0 ||
      !isStudentExist(stu->getStuId(), this->pStuInfoList)) {
    std::cout << "Student " << stu->getStuId()
              << " do not exist! Cannot modify.\n";
    return false;
  }
  stu->setBaseInfo(name, stu->getStuId(), gender_is_male);
  return true;
}

bool ScoreManager::modifyStu(Student *stu, const Score &score) {
  if (pStuInfoList->size() == 0 ||
      !isStudentExist(stu->getStuId(), this->pStuInfoList)) {
    std::cout << "Student " << stu->getStuId()
              << " do not exist! Cannot modify.\n";
    return false;
  }
  return stu->setScores(score.chinese, score.math, score.english);
}

std::vector<Student *> ScoreManager::findStuByName(const std::string &name) {
  std::vector<Student *> res;
  for (auto pStu : *pStuInfoList) {
    if (pStu->getName() == name) {
      res.push_back(pStu);
    }
  }
  return res;
}

Student *ScoreManager::findStuByID(const std::string &stu_id) {
  for (auto pStu : *pStuInfoList) {
    if (pStu->getStuId() == stu_id)
      return pStu;
  }
  return nullptr;
}

void ScoreManager::sortStuByID(sortDirection t) {
  auto sortAscend = [](const Student *s1, const Student *s2) {
    return s1->getStuId() < s2->getStuId();
  };

  auto sortDescend = [](const Student *s1, const Student *s2) {
    return s1->getStuId() > s2->getStuId();
  };

  switch (t) {
  case ASCEND:
    std::sort(pStuInfoList->begin(), pStuInfoList->end(), sortAscend);
    break;
  case DESCEND:
    std::sort(pStuInfoList->begin(), pStuInfoList->end(), sortDescend);
    break;
  default:
    std::sort(pStuInfoList->begin(), pStuInfoList->end(), sortAscend);
  }
}

void ScoreManager::sortStuByScore(sortDirection t) {
  auto sortAscend = [](const Student *s1, const Student *s2) {
    return s1->getScore().total() < s2->getScore().total();
  };

  auto sortDescend = [](const Student *s1, const Student *s2) {
    return s1->getScore().total() > s2->getScore().total();
  };
  switch (t) {
  case ASCEND:
    std::sort(pStuInfoList->begin(), pStuInfoList->end(), sortAscend);
    break;
  case DESCEND:
    std::sort(pStuInfoList->begin(), pStuInfoList->end(), sortDescend);
    break;
  default:
    std::sort(pStuInfoList->begin(), pStuInfoList->end(), sortAscend);
  }
}

void ScoreManager::showStuInfoList() const {
  for (auto pStu : *pStuInfoList) {
    pStu->showInfo();
  }
}
} // namespace Manager
