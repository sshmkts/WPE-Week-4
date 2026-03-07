[![Unit Tests](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml/badge.svg)](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml)
Weekly Performance Evaluator
Week 08 – Recursion Upgrade
Overview

This project is an upgrade of the Week 07 codebase.
The purpose of Week 08 is to implement C++ recursion (Chapter 15) while preserving all existing functionality.

Only the required portions of the code were modified. The code diff clearly reflects recursion-related changes.

🔹 Key Concepts Implemented (Chapter 15)
1. Recursive Member Function

A recursive function was added to the ReportManager class.

countReportsOfType(string type)

This function counts how many stored reports match a specific report type.

The implementation follows proper recursion design:

Base case: when the index reaches the end of the container

Recursive case: check the current element and recursively evaluate the next element

No loops (for, while, do-while) are used inside the recursive function.

2. Recursive Helper Function

To support recursion, a private helper function was implemented:

countTypeRecursive(string type, int index)

This helper allows the algorithm to move through the container one element at a time using recursion.

3. Integration With Existing Architecture

The recursive logic integrates with the existing project design:

Uses the DynArray template container

Accesses reports via polymorphic WeeklyReport pointers

Uses getType() to determine the report category

This keeps the recursion implementation consistent with the current object-oriented structure.

🔹 Unit Testing (doctest)

Existing tests from previous weeks remain intact.

New tests were added to verify the recursion behavior:

Counting reports when the manager is empty

Counting multiple report types

Ensuring correct totals are returned

These tests confirm that both the base case and recursive case function correctly.

🔹 Memory Safety Verification (CRT)

The project continues to verify memory safety using the CRT debug library.

Dynamic memory (new / delete) is monitored using:

_CrtDumpMemoryLeaks()

The debug output confirms that no memory leaks are present after running the unit tests.

🔹 Git Repository Tag

The repository commit for this assignment is tagged:

Week08 Programming Assignment

This tag marks the exact version of the code used for the Week 08 submission.

Author

Oleksandr Mykytsei

