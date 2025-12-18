

# ASCII Art Control Engine — Updated Developer Documentation

## Purpose

The ASCII Art Control Engine is responsible for **fully managing the lifecycle of user ASCII art** in BinaryFetch.

This includes:

* Detecting missing user ASCII art
* Creating required directories
* Generating a user-editable ASCII file from a bundled default
* Loading ASCII art for rendering

The engine guarantees that **after a single call from `main`**, valid ASCII art is always available.

---

## Target User File

```
C:\Users\<USERNAME>\AppData\BinaryFetch\AsciiArt.txt
```

* All runtime operations interact **only** with this file.
* User edits to this file are preserved across runs.
* The engine **never overwrites an existing user file**.

---

## Bundled Default Asset

```
<EXE_DIR>\Default_Ascii_Art.txt
```

* Shipped with the program
* Read-only
* Used **only** when the user ASCII file does not exist

---

## High-Level Design Philosophy

> **Main should not care how ASCII art exists — only that it exists.**

Therefore:

* `main.cpp` makes **one request**
* The engine silently handles **all failure recovery**
* No multi-step orchestration in `main`

---

## Engine-Centric Workflow (Final)

### Step 1 — Main requests ASCII art

```text
main -> ASCII Engine: loadAsciiArt()
```

Main does **not** check files, paths, or defaults.

---

### Step 2 — Engine attempts to load user file

The engine internally:

1. Resolves the user ASCII path
2. Attempts to open `AsciiArt.txt`

---

### Step 3 — File missing → engine self-heals

If `AsciiArt.txt` does not exist, the engine automatically:

1. Creates the directory:

   ```
   %LOCALAPPDATA%\BinaryFetch\
   ```
2. Creates `AsciiArt.txt`
3. Copies content from `Default_Ascii_Art.txt`
4. Performs atomic write (temp → rename)
5. Immediately reloads the newly created file

This process is **invisible to main**.

---

### Step 4 — Engine returns loaded ASCII art

* ASCII content is now guaranteed to exist
* Engine provides loaded lines to the caller
* Rendering can proceed safely

---

## Final Execution Diagram

```text
main.cpp
  |
  |-- loadAsciiArt()
        |
        |-- user file exists? ---- yes --> load & return
        |                          no
        |
        |-- create directory
        |-- create AsciiArt.txt from Default_Ascii_Art.txt
        |-- reload file
        |
        |-- return loaded ASCII art
```

---

## Role of `main.cpp` (Final)

`main.cpp` is a **consumer**, not a coordinator.

### Responsibilities

* Instantiate the ASCII engine
* Request ASCII art **once**
* Handle fatal failure only (e.g., engine returns empty art)

### What `main.cpp` must NOT do

* ❌ Create directories
* ❌ Copy default files
* ❌ Retry loading
* ❌ Manage fallbacks
* ❌ Know file paths

### Example (Conceptual)

```cpp
AsciiArtControlEngine engine;
auto asciiArt = engine.loadAsciiArt();

// If this returns, ASCII art exists and is valid
```

---

## Role of `AsciiArtControlEngine.cpp` (Final)

The engine is **fully autonomous**.

### Responsibilities

1. Resolve user paths
2. Detect missing ASCII art
3. Generate user file from bundled default
4. Load ASCII art into memory
5. Sanitize and validate content
6. Guarantee availability

### Design Constraints

* No rendering
* No color handling
* No system info access
* No console output (except optional debug logs)
* No dependency on `main` logic

---

## Recommended Public API (Simplified)

```cpp
class AsciiArtControlEngine {
public:
    bool loadAsciiArt();              // guarantees existence
    const std::vector<std::string>& getLines() const;
};
```

### Internal-only status handling

Status enums remain **internal**, not exposed to `main`.

---

## Key Guarantees

* ✅ First run always succeeds
* ✅ User customization preserved
* ✅ No duplicated logic in `main`
* ✅ Clean, minimal entry point
* ✅ Easy future extension (themes, profiles, presets)

---

## Final Summary

* **Main is clean**
* **Engine is smart**
* **ASCII art always exists**
* **No orchestration spaghetti**
* **Professional-grade design**


