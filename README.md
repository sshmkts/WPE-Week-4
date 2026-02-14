[![Unit Tests](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml/badge.svg)](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml)

Week 05 – Weekly Performance Evaluator (Upgrade from Week 04)

Course: ITCS 2550
Author: Oleksandr Mykytsei
Term: Winter 2026

What I did (Week 5 changes)

This week I upgraded my Week 04 project to use abstract classes, virtual functions, polymorphism, and dynamic memory.

Main features

Abstract base class: WeeklyReport

Has a virtual destructor

Has a pure virtual function: getType()

print() is virtual and used polymorphically

Derived classes:

LevelReport

TrainingPlanReport

RecoveryReport

Composition:

SessionStats is used inside derived reports to store session totals/averages

Manager / container (no STL): ReportManager

Stores WeeklyReport** items (dynamic array of base pointers)

Tracks size and capacity

Supports add, removeAt, resize

Deletes all allocated objects (no memory leaks)

Copying is disabled to avoid Rule of 3/5 problems

How the program works

User enters weekly training data (sessions + sleep).

The program calculates training stats and readiness.

Menu allows:

Add Level Report

Add Training Plan Report

Add Recovery Report

Print all saved reports (polymorphism)

Delete a report

Unit Tests

In _DEBUG, the program runs doctest tests.

Tests cover:

SessionStats default behavior

Polymorphism (getType() on base pointers)

ReportManager add/remove/resize

TrainingLog session logic (without cin)

Notes

All objects are created with new and cleaned using a virtual base destructor.

main() runs only in Release mode (#ifndef _DEBUG).
