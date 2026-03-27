[![Unit Tests](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml/badge.svg)](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml)
# Weekly Performance Evaluator

## Week 10 – Linked List Upgrade

---

## Overview

This project upgrades the **Week 09 version** by introducing concepts from **Chapter 17: Linked Lists**, while preserving all previous functionality.

Only the required parts of the code were modified. The system now replaces one core data structure with a **custom linked list ADT**, while maintaining compatibility with all previous features.

---

## Linked List Implementation

A **custom unordered linked list ADT** was implemented to replace the previous session storage structure.

### Key Characteristics
- Custom `Node` structure (data + next pointer)
- Dynamic memory allocation
- No use of `std::list` (as required)

### Supported Operations
- `insertFirst()` – insert at beginning  
- `insertLast()` – insert at end  
- `deleteValue()` – remove by value  
- `search()` – find element index  
- `print()` – traverse and display  

---

## Iterator Implementation

A custom **Iterator class** was added for safe traversal of the linked list.

### Features
- `operator*` – access current value  
- `operator++` – move to next node  
- `operator== / !=` – comparison  

### Usage
The iterator is actively used in:
- Training calculations  
- Session traversal  
- Data conversion (`toVector()`)  

---

## Integration with Previous Features

The linked list is now the **primary data structure** for session storage.

A `std::vector` is still used **only as a snapshot**, to preserve compatibility with existing Week 09 functionality (report generation).

> This ensures the project remains an **upgrade, not a full rewrite**, as required.

---

## Searching Implementation

### Sequential (Linear) Search
- Traverses the linked list  
- Returns index if found  
- Returns `-1` if not found  

### Binary Search
- Works on sorted data  
- Uses indexed access (`getAt`) on linked list  

---

## Sorting Implementation

### Bubble Sort
- Implemented as a class member function  
- Uses `getAt()` and `setAt()`  
- Sorts data in ascending order  

---

## Key Features

- Custom Linked List ADT  
- Custom Iterator  
- Sequential Search  
- Binary Search  
- Bubble Sort  
- Object-Oriented Design (OOP)  
- Inheritance and Polymorphism  
- Composition  
- Templates  
- Custom Exceptions  
- Recursion (Week 08)  
- Operator Overloading  
- File Output  
- Doctest Unit Testing  

---

## Program Behavior

The program:
- Stores session data using a **linked list**  
- Calculates performance metrics  
- Generates multiple report types  
- Manages reports dynamically  
- Supports searching and sorting  
- Maintains full backward compatibility  

---

## Testing

Doctest was used to verify:

- Linked list operations  
- Iterator traversal  
- Edge cases (empty list, invalid operations)  
- Sorting correctness  
- Searching accuracy  
- Exception handling  
- Stability of previous features  

---

## Summary

This version improves the system by replacing a core structure with a **custom linked list**, strengthening understanding of dynamic memory and pointers, while preserving a clean and scalable object-oriented design.

---

## Author

**Oleksandr Mykytsei**  
Macomb Community College  
ITCS 2550

