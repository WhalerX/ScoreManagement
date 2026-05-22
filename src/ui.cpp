#include "ui.hpp"
#include "manager.hpp"
#include "student.hpp"
#include "OpenXLSX.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

auto strlower = [](std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s;
};

Ui *Ui::_instance = nullptr;

Ui *Ui::Instance() {
  if (_instance == nullptr) {
    _instance = new Ui;
  }
  return _instance;
}

Ui::Ui() {
  isExit = false;
  pScoreManager = Manager::ScoreManager::Instance();
  opMap[1] = std::bind(&Ui::AddStudent, this);
  opMap[2] = std::bind(&Ui::DeleteStudent, this);
  opMap[3] = std::bind(&Ui::ModifyStudent, this);
  opMap[4] = std::bind(&Ui::FindStudent, this);
  opMap[5] = std::bind(&Ui::ShowListOfStudents, this);
  opMap[6] = std::bind(&Ui::SortStudent, this);
  opMap[7] = std::bind(&Ui::SaveToExcel, this);
  opMap[8] = std::bind(&Ui::Exit, this);
}

Ui::~Ui() { Manager::ScoreManager::releaseInstance(); }

void Ui::showMenu() const {
  system("cls");

  std::cout << "================ Student Management System ===============\n";
  std::cout << "(1) Add Student                  (2) Delete Student       \n";
  std::cout << "(3) Modify Student               (4) Find Student         \n";
  std::cout << "(5) Show List of Students        (6) Sort Student         \n";
  std::cout << "(7) Save to Excel                (8) Exit                 \n";
  std::cout << "==========================================================\n";
}

void Ui::AddStudent() {
  std::vector<Student *> stuInfoList;
  std::cout << ">>>>>> Enter students' information <<<<<<\n";
  while (true) {
    Student *pStu = Student::createStu();
    std::string name = getUsersKey<decltype(name)>("Name: ");
    std::string stu_id = getUsersKey<decltype(stu_id)>("Student ID: ");
    std::string gender_is_male =
        getUsersKey<decltype(gender_is_male)>("Gender(y or n): ");
    float chinese = getUsersKey<float>("Chinese Scores: ");
    float math = getUsersKey<float>("Math Scores: ");
    float english = getUsersKey<float>("English Scores: ");
    pStu->setBaseInfo(name, stu_id,
                      (strlower(gender_is_male) == "y") ? true : false);
    if (!pStu->setScores(chinese, math, english))
      pStu->setScores(0, 0, 0);
    stuInfoList.push_back(pStu);
    std::cout << "=========================================\n";

    std::string is_continue =
        getUsersKey<decltype(is_continue)>("Continue?(y or n): ");
    if (strlower(is_continue) != "y")
      break;
  }
  std::cout << "============= ";
  std::cout << pScoreManager->addStu(&stuInfoList)
            << " student(s) be added successfully!";
  std::cout << " =============\n";
}

void Ui::DeleteStudent() {
  enum delMethod { DeleteByName = 1, DeleteByID = 2, ExitDelete = 3 };
  std::vector<Student *> stuWaitForDelList;

  auto fillWaitForDelListByID = [this, &stuWaitForDelList]() {
    std::string stu_id = getUsersKey<decltype(stu_id)>("ID: ");
    Student *pStu = pScoreManager->findStuByID(stu_id);
    if (pStu != nullptr) {
      stuWaitForDelList.push_back(pStu);
    } else
      std::cout << "Cannot find a student with an ID: " << stu_id << "\n";
  };

  auto fillWaitForDelListByName = [this, &stuWaitForDelList]() {
    std::vector<Student *> res;
    std::string name = getUsersKey<decltype(name)>("name: ");
    res = pScoreManager->findStuByName(name);
    std::cout << "Find " << res.size() << " student(s)\n";
    while (res.size() != 0) {
      for (auto pStu : res)
        pStu->showInfo();
      std::string stu_id = getUsersKey<decltype(stu_id)>(
          "Which student do you want to choose?(Enter ID): ");
      Student *pStu = pScoreManager->findStuByID(stu_id);
      if (pStu != nullptr) {
        stuWaitForDelList.push_back(pStu);
        res.erase(remove(res.begin(), res.end(), pStu), res.end());
      } else
        std::cout << "Cannot find a student with an ID: " << stu_id << "\n";

      std::string is_continue =
          getUsersKey<decltype(is_continue)>("Continue?(y or n): ");
      if (strlower(is_continue) != "y")
        break;
    }
  };

  while (true) {
    std::cout << "===================== How to delete? =====================\n";
    std::cout << "(1) Delete By Name             (2) Delete By ID           \n";
    std::cout << "(3) Exit Delete                                           \n";
    std::cout << "==========================================================\n";
    int key = getUsersKey<int>("Enter your choice: ");

    switch (key) {
    case DeleteByName:
      fillWaitForDelListByName();
      pScoreManager->delStu(&stuWaitForDelList);
      break;
    case DeleteByID:
      fillWaitForDelListByID();
      pScoreManager->delStu(&stuWaitForDelList);
      break;
    case ExitDelete:
      break;

    default:
      std::cout << "Enter a valid choice!\n";
    }
    stuWaitForDelList.clear();
    if (key == ExitDelete)
      break;
  }
  std::cout << "============= " << "Delete Complete" << " =============\n";
}

void Ui::ModifyStudent() {
  enum modifyMethod { ModifyBaseInfo = 1, ModifyScores = 2, ExitModify = 3 };
  Student *pStu = nullptr;
  auto hitStuAndModBsInfo = [this, &pStu]() {
    std::string stu_id =
        getUsersKey<decltype(stu_id)>("Choose just one student(ID): ");
    pStu = pScoreManager->findStuByID(stu_id);
    if (pStu != nullptr) {
      std::string name = getUsersKey<decltype(name)>("New Name: ");
      std::string gender_is_male =
          getUsersKey<decltype(gender_is_male)>("New Gender(y or n): ");
      if (pScoreManager->modifyStu(
              pStu, name, (strlower(gender_is_male) == "y") ? true : false))
        std::cout << "Modify student " << pStu->getStuId()
                  << " successfully!\n";
    } else
      std::cout << "Cannot find a student with an ID: " << stu_id << "\n";
  };

  auto hitStuAndModScore = [this, &pStu]() {
    std::string stu_id =
        getUsersKey<decltype(stu_id)>("Choose just one student(ID): ");
    pStu = pScoreManager->findStuByID(stu_id);
    if (pStu != nullptr) {
      float chinese = getUsersKey<float>("Chinese: ");
      float math = getUsersKey<float>("Math: ");
      float english = getUsersKey<float>("English: ");
      if (pScoreManager->modifyStu(pStu, Score(chinese, math, english)))
        std::cout << "Modify student " << pStu->getStuId()
                  << " successfully!\n";
    } else
      std::cout << "Cannot find a student with an ID: " << stu_id << "\n";
  };

  while (true) {
    std::cout << "===================== How to modify? =====================\n";
    std::cout << "(1) Modify BaseInfo             (2) Modify Scores         \n";
    std::cout << "(3) Exit Modify                                           \n";
    std::cout << "==========================================================\n";
    int key = getUsersKey<int>("Enter your choice: ");

    switch (key) {
    case ModifyBaseInfo:
      hitStuAndModBsInfo();
      break;
    case ModifyScores:
      hitStuAndModScore();
      break;
    case ExitModify:
      break;

    default:
      std::cout << "Enter a valid choice!\n";
    }
    if (key == ExitModify)
      break;
  }
  std::cout << "============= " << "Modify Complete" << " =============\n";
}

void Ui::FindStudent() {
  enum findMethod { FindByName = 1, FindByID = 2, ExitFind = 3 };
  std::vector<Student *> stuHittedList;
  auto hitStuByname = [this, &stuHittedList]() {
    std::string name = getUsersKey<decltype(name)>("Name: ");
    stuHittedList = pScoreManager->findStuByName(name);
    std::cout << "Find " << stuHittedList.size() << " student(s)\n";

    if (stuHittedList.size() != 0) {
      for (auto pStu : stuHittedList)
        pStu->showInfo();
    }
  };
  auto hitStuByID = [this, &stuHittedList] {
    std::string stu_id = getUsersKey<decltype(stu_id)>("ID: ");
    Student *pStu = pScoreManager->findStuByID(stu_id);
    if (pStu != nullptr)
      stuHittedList.push_back(pStu);
    std::cout << "Find " << stuHittedList.size() << " student(s)\n";

    if (stuHittedList.size() != 0) {
      for (auto pStu : stuHittedList)
        pStu->showInfo();
    }
  };

  while (true) {
    std::cout << "====================== How to find? ======================\n";
    std::cout << "(1) Find By Name             (2) Find By ID               \n";
    std::cout << "(3) Exit Find                                             \n";
    std::cout << "==========================================================\n";
    int key = getUsersKey<int>("Enter your choice: ");

    switch (key) {
    case FindByName:
      hitStuByname();
      break;
    case FindByID:
      hitStuByID();
      break;
    case ExitFind:
      break;

    default:
      std::cout << "Enter a valid choice!\n";
    }
    stuHittedList.clear();
    if (key == ExitFind)
      break;
  }
  std::cout << "============= " << "Find Complete" << " =============\n";
}

void Ui::ShowListOfStudents() { pScoreManager->showStuInfoList(); }

void Ui::SortStudent() {
  enum sortMethod { SortByID = 1, SortByScores = 2, ExitSort = 3 };
  auto sortflowByID = [this]() {
    std::cout << "==================== Sort Direction? =====================\n";
    std::cout << "(1) ASCEND                        (2) DESCEND             \n";
    std::cout << "==========================================================\n";
    int subKey = getUsersKey<int>("Enter your choice: ");
    switch (subKey) {
    case 1:
      pScoreManager->sortStuByID(Manager::ASCEND);
      break;
    case 2:
      pScoreManager->sortStuByID(Manager::DESCEND);
      break;
    default:
      std::cout << "Invalid sort direction!\n";
      break;
    }
  };

  auto sortflowByScore = [this]() {
    std::cout << "==================== Sort Direction? =====================\n";
    std::cout << "(1) ASCEND                        (2) DESCEND             \n";
    std::cout << "==========================================================\n";
    int subKey = getUsersKey<int>("Enter your choice: ");
    switch (subKey) {
    case 1:
      pScoreManager->sortStuByScore(Manager::ASCEND);
      break;
    case 2:
      pScoreManager->sortStuByScore(Manager::DESCEND);
      break;
    default:
      std::cout << "Invalid sort direction!\n";
      break;
    }
  };

  while (true) {
    std::cout << "====================== How to find? ======================\n";
    std::cout << "(1) Sort By ID             (2) Sort By Scores             \n";
    std::cout << "(3) Exit Sort                                             \n";
    std::cout << "==========================================================\n";
    int key = getUsersKey<int>("Enter your choice: ");

    switch (key) {
    case SortByID:
      sortflowByID();
      break;
    case SortByScores:
      sortflowByScore();
      break;
    case ExitSort:
      break;

    default:
      std::cout << "Enter a valid choice!\n";
    }

    if (key == ExitSort)
      break;
  }
  std::cout << "============= " << "Sort Complete" << " =============\n";
}

void Ui::SaveToExcel() {
  if (pScoreManager->showStuNum() == 0) {
    std::cout << "No student data to save.\n";
    return;
  }

  std::string filename =
      getUsersKey<std::string>("Enter filename (without extension): ");
  filename += ".xlsx";

  OpenXLSX::XLDocument doc;
  doc.create(filename, OpenXLSX::XLForceOverwrite);
  auto wks = doc.workbook().worksheet("Sheet1");

  // 表头
  wks.cell(1, 1).value() = "学号";
  wks.cell(1, 2).value() = "姓名";
  wks.cell(1, 3).value() = "性别";
  wks.cell(1, 4).value() = "语文";
  wks.cell(1, 5).value() = "数学";
  wks.cell(1, 6).value() = "英语";
  wks.cell(1, 7).value() = "总分";

  // 数据行
  int row = 2;
  for (auto pStu : pScoreManager->getStuList()) {
    wks.cell(row, 1).value() = pStu->getStuId();
    wks.cell(row, 2).value() = pStu->getName();
    wks.cell(row, 3).value() = pStu->getGender() ? "男" : "女";
    wks.cell(row, 4).value() = pStu->getScore().chinese;
    wks.cell(row, 5).value() = pStu->getScore().math;
    wks.cell(row, 6).value() = pStu->getScore().english;
    wks.cell(row, 7).value() = pStu->getScore().total();
    row++;
  }

  doc.save();
  doc.close();
  std::cout << "Saved " << (row - 2) << " student(s) to " << filename << "\n";
}

void Ui::Exit() {
  std::cout << pScoreManager->showStuNum() << " student(s) in system.\n";
  std::cout << "Looking forward to your next use. Bye!\n";
  isExit = true;
}

void Ui::Scheduling() {
  auto isChoiceValid = [](int &k) {
    for (int n = 1; n <= 8; n++) {
      if (n == k)
        return true;
    }
    return false;
  };
  while (!isExit) {
    showMenu();
    int mainKey = getUsersKey<int>("Enter your choice: ");

    if (isChoiceValid(mainKey)) {
      opMap[mainKey]();
      getchar();
      getchar();
    } else {
      std::cout << "Please Enter a valid choice!\n";
      getchar();
      getchar();
    }
  }
}