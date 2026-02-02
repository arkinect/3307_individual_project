# 3307_individual_project
* **User Interface**: Implement a file listing showing name, type, size, and modified date, alongside an editable path bar, a status bar for updates, and dialogs for errors or user input.
* **Core Operations**: Enable opening files using system defaults, directory navigation via double-clicking, and creating, renaming, or deleting items with mandatory confirmation.
* **Virtual Clipboard**: Support cutting, copying, and pasting files by tracking paths, including logic to prompt the user before overwriting existing files.
* **Technical Framework**: Hand-code the GUI in C++ using **wxWidgets** (version 3.0+) and manage all filesystem interactions through the **C++17 `<filesystem>` library**.
* **Structure and Build**: Organize code into classes with separate header and source files, provide a **Makefile**, and ensure all menu operations include keyboard shortcuts.
* **Refresh and Exit**: Provide functionality to manually refresh the directory listing and a standard exit option for the application.