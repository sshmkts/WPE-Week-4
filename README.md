[![Unit Tests](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml/badge.svg)](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml)
# Weekly Performance Evaluator — Week 06

This project is my **Week 06 Programming Assignment** for ITCS 2550.  
It builds on Week 05 and introduces **operator overloading** and **C++ templates** (Chapter 13).

---

## ✅ What the program does
The program tracks a player's weekly training and generates different reports:
- Level report
- Training plan report
- Recovery report

The user can:
- add reports
- print all saved reports
- delete reports
- store multiple reports dynamically

---

## ✅ Week 06 Concepts Implemented

### Operator Overloading
The following operators were implemented:

- **operator== (LevelReport)**  
  Two reports are equal when identity fields match (name, age, level, sessions, total training).

- **operator<< (WeeklyReport)**  
  Outputs a clean one-line summary using polymorphism via `toStream()`.

- **operator[] (ReportManager)**  
  Provides safe indexing with bounds checking.  
  Invalid index returns `nullptr` (no exceptions used).

- **operator+= (ReportManager)**  
  Adds a report pointer to the container and resizes dynamically if needed.

- **operator-= (ReportManager)**  
  Removes a report by index, deletes the object, and shifts remaining items.

---

### Templates
- **Function template:**  
  `clampMin<T>()` ensures values do not go below a minimum.

- **Class template:**  
  `DynArray<T>` replaces the Week 05 dynamic array logic and provides:
  - dynamic resizing
  - add/remove
  - safe indexing
  - generic storage

Ownership of dynamic memory remains in `ReportManager`.

---

## ✅ OOP Design
- **Abstract base class:** `WeeklyReport`
- **Derived classes:** `LevelReport`, `TrainingPlanReport`, `RecoveryReport`
- **Composition:** `SessionStats` used inside derived reports
- **Manager class:** `ReportManager` manages dynamic report storage
- **Template container:** `DynArray<WeeklyReport*>`
- **Enum:** `PlayerLevel`

---

## ✅ Unit Tests (doctest)
New tests were added for:
- equality operator
- stream output operator
- indexing operator
- add/remove operators
- template function
- template class

All tests pass in `_DEBUG` mode.

---

## ✅ How to run
### Run program
Compile in **Release mode** and run normally.

### Run tests
Compile in **Debug mode** to execute doctest unit tests.

---

## ✅ Notes
- Only concepts up to Chapter 13 were used.
- STL containers were not used.
- Dynamic memory is safely managed through `ReportManager`.

---

## ✅ Git Tag
The commit for this assignment is tagged:
