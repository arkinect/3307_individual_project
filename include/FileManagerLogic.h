/*
 * Author: Mathew Lane
 * Description: <short file description>
 * Date: 2026-02-02
 */

#ifndef FILE_MANAGER_LOGIC_H
#define FILE_MANAGER_LOGIC_H

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// struct for meta data
struct FileEntry {
    std::string name;
    std::string type;
    std::string size;     // for human readable size
    std::string modified; // for date
    bool isDirectory;
};

class FileManagerLogic {
public:
    enum class ClipboardOp { COPY, CUT, NONE };

    FileManagerLogic();
    ~FileManagerLogic();

    // core operations
    bool CreateFolder(const std::string& name);
    bool RenameItem(const fs::path& oldPath, const std::string& newName);
    bool DeleteItem(const fs::path& path);

    // virtual clipboard operations
    void Copy(const fs::path& source);
    void Cut(const fs::path& source);
    bool Paste(const fs::path& destination, bool overwriteConfirmed = false);

    // navigation / directory listing
    std::vector<FileEntry> GetDirectoryContents(const fs::path& path);
    fs::path GetCurrentPath() const { return m_currentPath; }
    void SetCurrentPath(const fs::path& path) { m_currentPath = path; }

    // helpers / error handling
    static std::string FormatSize(uintmax_t size);
    std::string GetLastError() const { return m_lastError; }
    fs::path GetClipboardPath() const { return m_clipboardSource; }

private:
    fs::path m_currentPath;
    fs::path m_clipboardSource;
    ClipboardOp m_lastOp;
    std::string m_lastError;

    void SetError(const std::string& error) { m_lastError = error; }
};

#endif // FILE_MANAGER_LOGIC_H
