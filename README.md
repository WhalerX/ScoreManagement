# ScoreManagement

## 项目简介

ScoreManagement 是一个基于 C++ 的命令行学生成绩管理系统，支持学生信息的增删改查、按成绩或学号排序等功能，并可导出 Excel 文件。

## 功能特性

- **学生管理**：添加、删除、修改、查找学生信息
- **成绩管理**：支持语文、数学、英语三科成绩录入（满分分别为 150、200、120）
- **排序功能**：按学号或总成绩升序/降序排列
- **Excel 导出**：基于 OpenXLSX 库将成绩表导出为 `.xlsx` 文件
- **交互式菜单**：命令行菜单驱动，操作简洁

## 编译环境

- **编译器**：MinGW GCC / G++
- **C++ 标准**：C++17
- **构建工具**：CMake 4.1 + MinGW Make
- **依赖库**：[OpenXLSX](https://github.com/troldal/OpenXLSX)（Excel 文件读写）

## 编译步骤

```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
make
```

编译产物输出到 `bin/` 目录下。

## 项目结构

```
ScoreManagement/
├── CMakeLists.txt        # CMake 构建配置
├── include/              # 头文件
│   ├── student.hpp       # 学生类定义
│   ├── manager.hpp       # 成绩管理器（单例模式）
│   └── ui.hpp            # 用户界面（单例模式）
├── src/                  # 源文件
│   ├── main.cpp          # 程序入口
│   ├── student.cpp       # 学生类实现
│   ├── manager.cpp       # 管理器实现
│   └── ui.cpp            # 界面实现
├── third_party/          # 第三方依赖
│   └── OpenXLSX/         # OpenXLSX 库
└── bin/                  # 编译输出
```
