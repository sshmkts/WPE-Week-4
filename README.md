[![Unit Tests](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml/badge.svg)](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml)
# Weekly Performance Evaluator

## Week 09 – Searching, Sorting, and Vector Upgrade

---

## Overview

This project upgrades the **Week 08 version** by adding concepts from **Chapter 16: Searching, Sorting, and the Vector Type**, while preserving all previous functionality.

Only the required parts of the code were modified. The code diff highlights the changes related to vector usage, searching, and sorting.

---

## Vector Implementation

A `std::vector<double>` was introduced to replace at least one existing array.

This allows:
- Dynamic resizing
- Easier data management
- Compatibility with searching and sorting algorithms

---

## Searching Implementation

The following search algorithms were added as **class member functions**:

### Sequential (Linear) Search
- Iterates through the vector
- Returns the index of the found element
- Returns `-1` if not found

### Binary Search
- Works on **sorted data only**
- Efficient search using divide-and-conquer approach

---

## Sorting Implementation

### Bubble Sort

A manual sorting algorithm was implemented:

- Compares adjacent elements
- Swaps them if needed
- Repeats until sorted

---

## Key Features

- Use of **std::vector**
- Sequential search implementation
- Binary search implementation
- Bubble sort implementation
- Object-Oriented Design (OOP)
- Inheritance and Polymorphism
- Composition
- Templates
- Custom Exceptions
- Recursion (from previous week)
- Operator Overloading
- File Output
- Doctest Unit Testing

---

## Program Behavior

The program:
- Stores training session data using a vector
- Calculates performance metrics
- Generates different types of reports
- Saves and manages reports
- Allows searching within session data
- Sorts session data for efficient processing

---

## Testing

Doctest was used to verify:

- Vector operations
- Sequential search correctness
- Bubble sort accuracy
- Binary search functionality
- Exception handling
- Stability of previous features

---

## Summary

This version improves the project by introducing **modern data structures (vector)** and **fundamental algorithms (searching and sorting)**, making the system more flexible and efficient while maintaining a strong object-oriented structure.
## Author

Oleksandr Mykytsei  
Macomb Community College  
ITCS 2550

