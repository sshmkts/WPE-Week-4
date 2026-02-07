[![Unit Tests](https://github.com/USERNAME/REPO/actions/workflows/tests.yml/badge.svg)](https://github.com/USERNAME/REPO/actions/workflows/tests.yml)

# Weekly Performance Evaluator (Week 04) — Inheritance + Composition

This project is my Week 04 assignment for ITCS 2550.  
It continues Week 03, but now the program is restructured using **inheritance** and **composition** (Chapter 11).

## What the program does
The program tracks a player's weekly training:
- asks for name, age, number of sessions, hours per session, and average sleep
- calculates total/average training and a readiness score
- shows different reports from a menu:
  1) Level report  
  2) Training plan report  
  3) Recovery report  

## Class design (Week 04 requirements)

### Base Class: `WeeklyReport`
Contains the required base fields:
- `string` player name  
- `int` age  
- `enum PlayerLevel` (Week 01 enum requirement)

Also includes:
- default + parameterized constructors
- getters/setters
- `virtual print()` to display base info
- `protected` member: `level` (used by derived classes)

### Composition Class: `SessionStats`
This class stores session summary:
- session count
- total hours
- average hours

Includes:
- constructors
- getters/setters
- helper method: `isEmpty()`

### Derived Classes
Both derived classes override `print()` and call `WeeklyReport::print()` first.

#### `TrainingPlanReport : public WeeklyReport`
Adds:
- focus (string)
- technical minutes (double)
- conditioning minutes (double)
- composition member: `SessionStats stats`

#### `RecoveryReport : public WeeklyReport`
Adds:
- fatigue (string)
- rest days (int)
- tip (string)
- composition member: `SessionStats stats`

### Manager Class: `TrainingLog`
Handles user input/menu and creates the base/derived reports.

## Unit Tests (doctest)
This project includes **6+ doctest tests**, focused on:
- constructors
- getters/setters
- composition class behavior (`SessionStats`)
- derived classes keeping base + derived fields
- `TrainingLog` stats updates using `addSession()`

Tests run when `_DEBUG` is enabled.

## How to run

### Program mode (Release)
Run normally to use the menu and interactive input.

### Test mode (Debug)
Run in Debug to execute doctest unit tests.

## GitHub Actions
This repo includes a GitHub Actions workflow that runs tests on push.  
The status badge should show passing when all tests succeed.

## Class Diagram
A Visual Studio **Class Diagram (.cd)** file is included in the repo to show:
- inheritance (`WeeklyReport` → derived classes)
- composition (`SessionStats` inside derived classes)

## Notes
- No magic numbers: constants are defined at the top.
- Only topics up to Chapter 11 are used.
