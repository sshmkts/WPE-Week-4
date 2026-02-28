[![Unit Tests](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml/badge.svg)](https://github.com/sshmkts/WPE-Week-4/actions/workflows/tests.yml)
# Weekly Performance Evaluator  
## Week 07 – Exception Handling Upgrade

### Overview
This project is an upgrade of the Week 06 codebase.  
The purpose of Week 07 is to implement C++ exception handling (Chapter 14) while preserving all existing functionality.

Only the required portions of the code were modified. The diff clearly reflects exception-related changes.

---

## 🔹 Key Concepts Implemented (Chapter 14)

### 1. Custom Exception
A custom exception class `AppException` was created.

- Derived from `std::runtime_error`
- Stores an error message
- Uses `what()` for reporting

This ensures meaningful and consistent error handling across the program.

---

### 2. Template Container Safety (DynArray<T>)

The template class now enforces safe access:

- `at(index)` throws `AppException` if index is invalid
- `operator[]` delegates to `at()` and throws on invalid index
- `removeAt(index)` throws if index is invalid

This prevents undefined behavior and protects memory integrity.

---

### 3. Manager Operator Requirements

`ReportManager` now:

- `operator[]` throws on invalid index
- `operator-=` throws on invalid removal

All validation logic is delegated to the underlying template container.

---

### 4. try/catch in User Interface

User-driven deletion is wrapped in a `try/catch` block.

This prevents program crashes due to invalid input and ensures controlled error handling.

---

## 🔹 Unit Testing (doctest)

Existing tests from previous weeks remain intact.

New tests were added to verify:
- Invalid indexing throws exceptions
- Invalid removal throws exceptions
- Manager operator[] throws correctly
- Manager operator-= throws correctly

`CHECK_THROWS_AS` is used to confirm exception types.

---

## 🔹 Memory Safety Verification (CRT)

Since dynamic memory is used (`new` / `delete`), the program verifies memory safety using:

