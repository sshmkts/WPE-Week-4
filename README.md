[![Unit Tests](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml/badge.svg)](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml)

==============================================================
        WEEK 05 – WEEKLY PERFORMANCE EVALUATOR
                (Upgrade from Week 04)
==============================================================

Author: Oleksandr Mykytsei
--------------------------------------------------------------
PROJECT OVERVIEW
--------------------------------------------------------------
This version upgrades Week 04 by implementing:

• Abstract base class
• Pure virtual function
• Virtual destructor
• Polymorphism
• Dynamic memory management
• Custom dynamic array (no STL)

--------------------------------------------------------------
CORE ARCHITECTURE
--------------------------------------------------------------

ABSTRACT BASE:
  Class: WeeklyReport
    - virtual ~WeeklyReport()
    - virtual string getType() const = 0
    - virtual void print() const

DERIVED CLASSES:
  - LevelReport
  - TrainingPlanReport
  - RecoveryReport

COMPOSITION:
  - SessionStats (stores session count, total, average)

MANAGER / CONTAINER:
  Class: ReportManager
    - WeeklyReport** items
    - int size
    - int capacity
    - add()
    - removeAt()
    - resize()
    - printAll()

Copying disabled (Rule of 3/5 safety):
    ReportManager(const ReportManager&) = delete;
    ReportManager& operator=(const ReportManager&) = delete;

--------------------------------------------------------------
PROGRAM FLOW
--------------------------------------------------------------
1) User enters weekly training data
2) Program calculates:
     - total hours
     - average hours
     - readiness score
     - advice
3) Menu options:
     1) Add Level Report
     2) Add Training Plan Report
     3) Add Recovery Report
     4) Print All Reports
     5) Delete Report
     0) Exit

--------------------------------------------------------------
UNIT TESTS (_DEBUG)
--------------------------------------------------------------
• SessionStats validation
• Polymorphism via base pointer
• ReportManager add/remove/resize
• TrainingLog stat updates

--------------------------------------------------------------
MEMORY SAFETY
--------------------------------------------------------------
• All reports allocated with new
• Manager owns and deletes objects
• Base class has virtual destructor
• No STL containers used
• No memory leaks

==============================================================

