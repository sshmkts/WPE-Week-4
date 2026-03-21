[![Unit Tests](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml/badge.svg)](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml)
# Weekly Performance Evaluator
### Week 08 – Recursion Upgrade

## Overview
This project upgrades the **Week 07 codebase** by adding **C++ recursion (Chapter 15)** while preserving all existing functionality.

Only the required portions of the code were modified. The code diff highlights the recursion-related changes.

---

## Recursion Implementation

A recursive member function was added to the `ReportManager` class:

countReportsOfType(string type)

This function counts how many stored reports match a given report type.

### Base Case
When the index reaches the end of the container, the recursion stops.

### Recursive Case
The function checks the current report and recursively processes the next element.

No loops (`for`, `while`, `do-while`) are used inside the recursive function.

---

## Integration With Existing Architecture

The recursion works with the existing project design:

- DynArray template container
- Polymorphic `WeeklyReport*` objects
- `getType()` used to identify report categories

---

## Unit Testing

Unit tests are implemented using **doctest**.

New tests verify:
- recursion on an empty manager
- recursion with multiple report types
- correct counting results

---

## Memory Safety

Memory safety is verified using the **CRT debug library**.

_CrtDumpMemoryLeaks()

The debug output confirms that no memory leaks occur after running the tests.

---

## Repository Tag

The submission commit is tagged:

Week08 Programming Assignment

---

## Author

Oleksandr Mykytsei  
Macomb Community College  
ITCS 2550

