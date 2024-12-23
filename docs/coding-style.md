# Open Enigma M4 Plugboard Project Coding Style Guide

## General Guidelines
- **Readability**: Focus on writing clear and understandable code. Prioritize readability over cleverness.
- **Comments**: Use comments to explain the purpose and logic of code sections. Avoid obvious comments that do not add value.

## File Organization
- **Header Files**: Use `.h` files to declare functions, constants, and global variables with `extern`. Include header guards to prevent multiple inclusions.
- **Source Files**: Implement functions in `.cpp` files. Keep each source file focused on a specific module or functionality.
- **Main Sketch**: Limit the main sketch file (`.ino`) to high-level logic and setup/loop functions.

## Naming Conventions
- **Variables**: Use camelCase for variable names (e.g., `numChars`, `lampVal`). Global variables should be prefixed with `g_` (e.g., `g_mode`).
- **Constants**: Use ALL_CAPS with underscores for constants (e.g., `NUM_CHARS`, `LED_PIN`).
- **Functions**: Use camelCase for function names (e.g., `readKeyboard()`).
- **Files**: Use PascalCase for file names (e.g., `OpenEnigmaConstants.h`).

## Code Structure
- **Indentation**: Use 2 spaces for indentation. Do not use tabs.
- **Braces**: Place opening braces on the same line as the control statement. Closing braces should align with the opening statement.
  ```cpp
  if (condition) {
      // code
  } else {
      // code
  }
  ```
- **Line Length**: Limit lines to 80 characters where possible to enhance readability.
- **Whitespace**: Use spaces around operators and after commas. Avoid unnecessary whitespace.

## Function Design
- **Single Responsibility**: Ensure each function performs a single, well-defined task. Keep functions short and focused.
- **Parameters**: Limit the number of parameters to improve readability. Use structures or classes for complex data.
- **Return Values**: Prefer returning values over using global variables. Use meaningful return types.

## Error Handling
- **Assertions**: Use assertions to catch invalid assumptions during development.
- **Error Checking**: Handle potential errors gracefully, especially when dealing with hardware I/O operations.

## Documentation
- **Function Comments**: Document each function with a brief description, parameters, and return value.