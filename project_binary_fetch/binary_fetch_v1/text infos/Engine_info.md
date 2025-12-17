

# BinaryFetch Configuration & ASCII Engine Architecture

## Document Purpose

This document defines the architecture, responsibilities, and execution flow of the **BinaryFetch configuration system**, consisting of two independent engines:

1. **JSON Builder Engine**
2. **ASCII Art Generator Engine**

These engines ensure BinaryFetch operates reliably on first run, supports persistent user customization, and maintains a clean separation of concerns.

---

## Architectural Overview

BinaryFetch is structured around a **central execution engine** that consumes external resources but does not generate them directly.

All file creation and default resource generation are delegated to dedicated engines.

### Engines Summary

| Engine                     | Responsibility                                |
| -------------------------- | --------------------------------------------- |
| JSON Builder Engine        | Generates and maintains default configuration |
| ASCII Art Generator Engine | Generates and maintains ASCII art assets      |
| Main Engine                | Coordinates execution and renders output      |

---

## File System Layout

```
C:\Users\<USERNAME>\AppData\BinaryFetch\
│
├── BinaryFetch.jsonc
└── AsciiArt.txt
```

---

# 1. JSON Builder Engine

## Purpose

The JSON Builder Engine guarantees the existence of a valid configuration file required for BinaryFetch execution.

Its primary objectives are:

* Provide a safe first-run experience
* Enable persistent user customization
* Prevent runtime failures caused by missing configuration
* Isolate configuration generation logic from the main execution flow

---

## Target File

```
C:\Users\<USERNAME>\AppData\BinaryFetch\BinaryFetch.jsonc
```

---

## Execution Conditions

The JSON Builder Engine executes **only** when one or more of the following conditions are met:

* `BinaryFetch.jsonc` does not exist
* (Optional extension) Configuration file is malformed
* (Optional extension) Configuration version mismatch is detected

The engine **does not** execute during normal runs when a valid configuration file is present.

---

## Execution Flow

1. Main Engine starts
2. Configuration Loader checks for `BinaryFetch.jsonc`
3. If file exists:

   * Control proceeds to configuration parsing
4. If file is missing:

   * JSON Builder Engine is invoked
5. JSON Builder Engine:

   * Creates required directories
   * Writes a default `.jsonc` configuration
6. Control returns to Main Engine
7. Configuration Loader re-checks file existence
8. Execution continues using the generated configuration

---

## Generated Configuration Responsibilities

The default configuration defines:

* Display preferences
* Enabled or disabled sections
* Output mode (compact / extended)
* ASCII art usage
* Future extensibility via versioning

### Example Structure (Conceptual)

```jsonc
{
  "config_version": 1,

  "display": {
    "compact": false,
    "show_ascii": true
  },

  "sections": {
    "os": true,
    "cpu": true,
    "gpu": true,
    "memory": true,
    "network": true
  }
}
```

---

## Design Constraints

The JSON Builder Engine must adhere to the following rules:

* Must not read system information
* Must not parse JSON
* Must not perform console output
* Must only generate default configuration data
* Must always produce a syntactically valid JSONC file

This intentional simplicity ensures reliability and predictability.

---

# 2. ASCII Art Generator Engine

## Purpose

The ASCII Art Generator Engine provides visual assets independently of the executable, allowing customization without recompilation.

Its objectives include:

* Decoupling presentation from logic
* Supporting user-defined branding
* Allowing future themes and presets
* Maintaining consistent rendering behavior

---

## Target File

```
C:\Users\<USERNAME>\AppData\BinaryFetch\AsciiArt.txt
```

---

## Execution Conditions

The ASCII Art Generator Engine executes only when:

* The ASCII art file does not exist
* (Optional extension) Art reset is requested

The engine does not execute during standard runs when the file is present.

---

## Execution Flow

1. Main Engine requests ASCII art
2. ASCII Loader checks for `AsciiArt.txt`
3. If file exists:

   * Content is loaded and rendered
4. If file is missing:

   * ASCII Art Generator Engine is invoked
5. Generator writes default ASCII art
6. Control returns to Main Engine
7. ASCII content is loaded and rendered

---

## Generated Content Responsibilities

The default ASCII art must:

* Preserve spacing and alignment
* Remain terminal-safe
* Avoid system-specific assumptions

### Example

```
  _>> BinaryFetch_____________________________________________________
```

---

## Design Constraints

The ASCII Art Generator Engine must:

* Not access system information
* Not apply colors or formatting logic
* Not depend on configuration parsing
* Only generate ASCII text assets
* Always produce safe, renderable output

---

# 3. Main Engine Coordination

## Role

The Main Engine acts as the coordinator and renderer.
It does not generate default resources.

Responsibilities include:

* Detecting required resources
* Invoking engines when necessary
* Parsing configuration
* Collecting system information
* Rendering final output

---

## Startup Sequence

```text
START
 |
 |-- Check BinaryFetch.jsonc
 |     |-- Exists → Load
 |     |-- Missing → JSON Builder → Load
 |
 |-- Check AsciiArt.txt
 |     |-- Exists → Load
 |     |-- Missing → ASCII Generator → Load
 |
 |-- Parse configuration
 |-- Collect system data
 |-- Render output
 |
END
```

---

## Architectural Benefits

This design provides:

* Self-healing execution
* First-run stability
* Persistent customization
* Clear separation of concerns
* Minimal runtime overhead
* Scalable extension paths (flags, themes, plugins)

---

## Conclusion

The two-engine architecture establishes BinaryFetch as a robust, extensible, and professional system information tool.
This foundation supports future enhancements such as command-line flags, configuration migration, theming, and modular extensions without structural refactoring.


