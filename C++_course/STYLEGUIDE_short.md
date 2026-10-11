# C/C++ Style Guide — Quick Reference & Requirements Checklist

Concise summary of coding standards and clean code requirements based on [STYLEGUIDE.md](file:///home/alex/programming/kursCPP/STYLEGUIDE.md).

---

## 1. Formatting & Layout (*Formatowanie i układ*)

### Line Width & Indentation
- [ ] **Line width limit:** Maximum **80 characters** per line. Break long expressions/arguments across lines.
- [ ] **Indentation:** Use **2 or 4 spaces** consistently throughout the entire project.
- [ ] **No mixed indents:** Never mix tabs and spaces.

### Vertical Spacing (*Odstępy pionowe*)
- [ ] **1 blank line:** Separate logical sections, variable declarations, and function definitions with exactly one blank line.
- [ ] **No excessive blank lines:** Avoid multiple consecutive blank lines.

### Horizontal Spacing (*Odstępy poziome*)
- **MUST include spaces:**
  - Around binary operators (`=`, `+`, `-`, `*`, `/`, `==`, `!=`, `<`, `>`, `&&`, `||`, etc.).
  - After commas in argument/parameter lists (`foo(a, b)`).
  - After `#include`.
  - After keywords: `if`, `for`, `while`, `do`, `else`, `switch`, `case`.
  - Before an opening curly brace `{`.
  - Before `(` and after `)` when grouping expressions to change precedence.
- **MUST NOT include spaces:**
  - Between function name and `(` in declarations and calls: `func(x)` (not `func (x)` or `func( x )`).
  - Before commas `,` or semicolons `;`.
  - Inside square brackets: `arr[i]` (not `arr[ i ]`).
  - Around member access operators `.` and `->`.
  - Between unary operators and their operand: `-x`, `++i`, `--i`.
  - After address-of `&` and dereference `*` operators.
  - Before a closing brace `}`.

---

## 2. Braces, Control Flow & Declarations

### Braces & Statements
- [ ] **Brace style:** Opening brace `{` on the same line; closing brace `}` aligned with the statement.
- [ ] **`else` placement:** Place `else` on a new line after the closing brace:
  ```c
  if (condition) {
      statement1;
  }
  else {
      statement2;
  }
  ```
- [ ] **Brace requirement:** Always use braces if at least one branch in an `if-else` chain requires them.
- [ ] **Single-statement loops/conditionals:** Braces are strongly recommended even for single lines to prevent bugs.

### Pointer Declarations
- [ ] **Pointer asterisk:** Attach `*` to the variable name, not the type:
  ```c
  type *name;
  type *name1, *name2, name3; // Note: name3 is not a pointer!
  ```

---

## 3. Translation Unit / File Structure (*Struktura pliku*)

Organize each source file in the following order:
1. `#include` directives (standard library first, then local headers).
2. `#define` macros and constants.
3. Type definitions (`struct`, `typedef`, `enum`).
4. Global variables (if strictly unavoidable).
5. Forward declarations of local helper functions (`static`).
6. Definitions of local helper functions (`static`).
7. Exported functions (e.g., `main()` or public module API).

*Note: For multi-file projects, create a corresponding `.h` header file for exported functions.*

---

## 4. Naming Conventions (*Nazewnictwo*)

- [ ] **Language:** Use **English** for all identifiers and comments (`patientList`, not `listaPacjentow`).
- [ ] **Descriptive names:** Use full, self-documenting names (e.g., `loadPatientsListFromFile()`, not `get()`).
- [ ] **No arbitrary abbreviations:** Do not omit vowels or invent abbreviations (e.g., avoid `ptntLst`).
- [ ] **Single-letter variables:** Allowed only for math/loop conventions:
  - Loop counters: `i`, `j`, `k`.
  - Coordinates: `x`, `y`, `z`.
- [ ] **Casing conventions:**
  - `snake_case` or `camelCase` for variables and functions (choose one and stay consistent).
  - `ALL_CAPS_WITH_UNDERSCORES` for constants and macros (`PATIENTS_FILE_NAME`).
  - `PascalCase` or `snake_case_t` for user-defined types.

---

## 5. Functions & Clean Code (*Funkcje i czysty kod*)

- [ ] **Single Responsibility:** Each function must do one thing at a single level of abstraction.
- [ ] **DRY (Don't Repeat Yourself):** Extract repeated logic into reusable functions.
- [ ] **Function length:**
  - Ideal: **3 to 6 lines**.
  - Strict maximum: Must easily fit on one screen (**< 50 lines**).
- [ ] **Parameter count:** Keep parameters minimal — ideally **<= 3 arguments**. Use a `struct` if more are required.
- [ ] **Pure functions / No side effects:** Avoid mutating state outside the function whenever possible. Given identical inputs, return identical output.
- [ ] **High-level `main()`:** The `main()` function should read like a high-level summary of the program steps.

---

## 6. Prohibited & Restricted Practices (*Zakazy i ograniczenia*)

- [ ] **No Magic Numbers:** All non-trivial literals must be named constants (`const`, `constexpr`, or `#define`).
- [ ] **No Global Variables:** Pass state via parameters or return values instead.
- [ ] **No Unjustified `goto`:** Using `goto` without valid justification (e.g., breaking out of deeply nested loops) is treated as an error.
- [ ] **No Dead Code:** Remove commented-out code before submitting; rely on version control (Git).
- [ ] **No Trivial Comments:** Do not write comments that merely restate the code (e.g., `// increment counter`). Comment **why**, not **what** (algorithms, non-obvious bug fixes, preconditions).

