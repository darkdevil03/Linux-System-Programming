# IDE Setup Guide for Fast Coding

This guide will help you configure your IDE for the best autocompletion, code suggestions, and fast coding experience with this Linux System Programming project.

## CLion Configuration (Recommended)

### 1. Verify WSL Toolchain

Your project is already configured with the **WSL-Ubuntu-24.04** CMake profile. To verify:

1. Go to **File → Settings → Build, Execution, Deployment → Toolchain**
2. Ensure a **WSL** toolchain is listed and marked as default
3. The toolchain should point to:
   - C Compiler: `/usr/bin/gcc`
   - C++ Compiler: `/usr/bin/g++`
   - CMake: `/usr/bin/cmake`

### 2. Verify CMake Profile

1. Go to **File → Settings → Build, Execution, Deployment → CMake**
2. Ensure the **Profile** is set to `WSL-Ubuntu-24.04`
3. The **Build directory** should be `cmake-build-wsl`
4. Click **Load CMake Project** to reconfigure

### 3. Enable Code Completion Features

Go to **File → Settings → Editor → General → Code Completion**:

- ✅ **Autocomplete in comments and strings** — enables suggestions everywhere
- ✅ **Show suggestions as you type** — auto-popup suggestions
- ✅ **Insert selected suggestion by any keys** — press any key to accept
- ✅ **Insert selected suggestion by Enter, Tab** — quick acceptance
- Set **Autopup suggestions** to `0` ms for instant suggestions

### 4. Enable Postfix Completions

Go to **File → Settings → Editor → General → Code Completion → Postfix Completion**:

- ✅ **Enable postfix completions** — type `var.if` to expand to `if (var) { }`

### 4. Configure Live Templates for C

Go to **File → Settings → Editor → Live Templates** and add these abbreviations:

| Abbreviation | Template | Description |
|---|---|---|
| `main` | `int main(int argc, char *argv[]) {\n    \n    return 0;\n}` | Main function |
| `pr` | `printf("$END$");` | Quick printf |
| `sc` | `scanf("$END$");` | Quick scanf |
| `fori` | `for (int i = 0; i < $LIMIT$; i++) {\n    $END$\n}` | For loop with index |
| `wh` | `while ($CONDITION$) {\n    $END$\n}` | While loop |
| `ifn` | `if ($EXPR$ == NULL) {\n    $END$\n}` | Null check |
| `iff` | `if (!$EXPR$) {\n    $END$\n}` | False check |
| `pthread` | `pthread_t thread;\npthread_create(&thread, NULL, $FUNC$, NULL);\npthread_join(thread, NULL);` | Thread creation |
| `fork` | `pid_t pid = fork();\nif (pid == 0) {\n    // child\n    $END$\n} else if (pid > 0) {\n    // parent\n} else {\n    perror("fork");\n}` | Fork pattern |

### 5. Enable Inlay Hints

Go to **File → Settings → Editor → General → Appearance**:

- ✅ **Show parameter name hints** — shows parameter names at call sites
- ✅ **Show type hints** — shows variable types inline
- ✅ **Show explicit type hints for initializer expressions**

### 6. Database Navigation

Go to **File → Settings → Editor → General → Code Completion**:

- Set **Basic** → **Database** to enable fuzzy matching for faster symbol lookup

### 7. Quick Fix Shortcuts

| Action | Shortcut |
|---|---|
| Basic Autocomplete | `Ctrl + Space` |
| Smart Autocomplete | `Ctrl + Shift + Space` |
| Quick Fix | `Alt + Enter` |
| Navigate to File | `Ctrl + Shift + N` |
| Navigate to Symbol | `Ctrl + Alt + Shift + N` |
| Find Usages | `Alt + F7` |
| Run Configuration | `Shift + F10` |
| Debug Configuration | `Shift + F9` |

### 8. Invalidate Caches (If Issues Persist)

If autocompletion still doesn't work after the above steps:

1. Go to **File → Invalidate Caches**
2. Check **Clear file system cache** and **Clear VCS Log caches**
3. Click **Invalidate and Restart**

## CMakeLists.txt Changes

The following was added to `CMakeLists.txt` to enable IDE code completion:

```cmake
# Generate compile_commands.json for IDE code completion and autocomplete support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

This ensures that `compile_commands.json` is generated in the build directory, which CLion uses to understand include paths and compilation flags for each source file.

## VS Code Configuration (Alternative)

If you use VS Code with the Remote-WSL extension, create `.vscode/c_cpp_properties.json`:

```json
{
    "configurations": [
        {
            "name": "WSL-GCC",
            "intelliSenseMode": "linux-gcc-x64",
            "compilerPath": "/usr/bin/gcc",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/include/**"
            ],
            "compileCommands": "${workspaceFolder}/cmake-build-wsl/compile_commands.json",
            "cStandard": "c23",
            "cppStandard": "c++17"
        }
    ],
    "version": 4
}
```

## Recommended Extensions (VS Code)

- **C/C++** by Microsoft — IntelliSense, debugging
- **C/C++ Extension Pack** — includes C/C++ Themes, C/C++ Compile Intellisense
- **Error Lens** — inline error and warning display
- **GitLens** — enhanced Git capabilities

## Troubleshooting

### Autocompletion not working for Linux headers

1. Ensure the WSL toolchain is selected in CLion settings
2. Verify `compile_commands.json` exists in `cmake-build-wsl/`
3. Reload CMake project: **File → Reload CMake Project**
4. Invalidate caches: **File → Invalidate Caches**

### Red underlines on Linux-specific functions

1. Check that the WSL profile is active (not a local Windows profile)
2. Verify the build directory is set to `cmake-build-wsl`
3. Ensure CMake has been reloaded after any changes

### Slow indexing

1. Go to **File → Settings → Editor → General**
2. Under **Power Save Mode**, ensure it is **disabled**
3. Exclude unnecessary directories from indexing: **File → Settings → Editor → File Types → Ignore files and folders**
