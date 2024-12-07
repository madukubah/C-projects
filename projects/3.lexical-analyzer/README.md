# SIMPLEX : (Simple Lexical Analyzer)
This project is an implementation of Chapter 3 of the classic "Compiler: Principles, Techniques, and Tools" (Dragon Book), focusing on building a Lexical Analyzer. The core components utilize Finite Automata (NFA and DFA) to recognize tokens based on specified string patterns and perform associated actions when matches are found.

# Features
- Token recognition using Finite Automata:
    - Non-Deterministic Finite Automata (NFA).
    - Deterministic Finite Automata (DFA).
- Support for regular expression patterns for token definitions.
- Actions triggered upon token recognition.
- Initial implementation with a 3D array for state mapping.
- **Planned Enhancement**: Transition to a hash table for better memory efficiency and scalability.

# Getting Started
## Prerequisites
- A C compiler (e.g., GCC or Clang).
- Basic understanding of Finite Automata and Lexical Analysis.
- Familiarity with regular expressions.
- Project Structure
    - src/: Contains the source code files.
    - src/utlis/: for reusable components.
    - tests/: Unit tests and example token patterns.
    - README.md: Project documentation.

# Usage
**Building the Project**

**To compile the project:**
```
sh build.sh 
```
the result will be build in ./build folder

**Run:**
```
./simplex nfa [string] [regex]
or
./simplex dfa [string] [regex]
```

**Run test:**
```
sh test.sh 
```
**Debug Mode:**
uncomment debug flag in ./src/common.h
```
#ifndef simplex_common_h
#define simplex_common_h

//#define DEBUG_MODE

#endif
```

# TODO
- Implement a hash table-based state mapping for improved memory efficiency.
- Extend regular expression support to include additional operators (e.g., +, ?, and grouping with parentheses).
- Add a command-line interface for token pattern configuration.
- Optimize the DFA minimization process.
- some edge case like 'empty input/regex' and 'string escape as literal' still fail
