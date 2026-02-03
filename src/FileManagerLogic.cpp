/*
 * Author: Mathew Lane
 * Description: Implements filesystem operations like copying, deleting, and renaming while providing error messaging to the UI.
 * Date: 2026-02-02
 */

#include "FileManagerLogic.h"
#include <iomanip>
#include <sstream>
#include <chrono>

/*
 * Function: FileManagerLogic
 * Description: constructor for FileManagerLogic class that initializes the cd and sets the last operation to NONE.
 * Parameters: None
 * Returns: None
 */
FileManagerLogic::FileManagerLogic() : m_lastOp(ClipboardOp::NONE) {
    m_currentPath = fs::current_path();
}

/*
 * Function: ~FileManagerLogic
 * Description: destructor for FileManagerLogic class
 * Parameters: None
 * Returns: None
 */
FileManagerLogic::~FileManagerLogic() {}

/*
 * Function: CreateFolder
 * Description: creates a directory named 'name' in the current path
 * Parameters: name: folder name to create
 * Returns: true on success, false on failure
 */
bool FileManagerLogic::CreateFolder(const std::string& name) {
    try {
        return fs::create_directory(m_currentPath / name);
    } catch (const fs::filesystem_error& e) {
        SetError(e.what());
        return false;
    }
}

/*
 * Function: RenameItem
 * Description: renames a file or directory at oldPath to a new name within the same parent folder
 * Parameters: oldPath: current full path, newName: the new filename string
 * Returns: true on success, false on failure
 */
bool FileManagerLogic::RenameItem(const fs::path& oldPath, const std::string& newName) {
    try {
        fs::path newPath = oldPath.parent_path() / newName;
        fs::rename(oldPath, newPath);
        return true;
    } catch (const fs::filesystem_error& e) {
        SetError(e.what());
        return false;
    }
}

/*
 * Function: DeleteItem
 * Description: removes a file or directory at the specified path
 * Parameters: path: the full filesystem path to delete
 * Returns: true on success, false on failure
 */
bool FileManagerLogic::DeleteItem(const fs::path& path) {
    try {
        return fs::remove_all(path) > 0;
    } catch (const fs::filesystem_error& e) {
        SetError(e.what());
        return false;
    }
}


/*
 * Function: Copy
 * Description: copies the selected item to the virtual clipboard
 * Parameters: source: path of item to copy
 * Returns: void
 */
void FileManagerLogic::Copy(const fs::path& source) {
    m_clipboardSource = source;
    m_lastOp = ClipboardOp::COPY;
}

/*
 * Function: Cut
 * Description: moves the selected item to the virtual clipboard
 * Parameters: source: path of item to cut
 * Returns: void
 */
void FileManagerLogic::Cut(const fs::path& source) {
    m_clipboardSource = source;
    m_lastOp = ClipboardOp::CUT;
}

/*
 * Function: Paste
 * Description: copies or moves the item in the virtual clipboard to the destination, clearing the clipboard on success
 * Parameters: destination: target directory, overwriteConfirmed: whether to replace existing items
 * Returns: true on success, false on failure
 */
bool FileManagerLogic::Paste(const fs::path& destination, bool overwriteConfirmed) {
    try {
        if (m_clipboardSource.empty() || m_lastOp == ClipboardOp::NONE) {
            SetError("Clipboard is empty.");
            return false;
        }

        fs::path target = destination / m_clipboardSource.filename();

        // if source and target are the same location do nothing. This was to fix a bug where overwriting itself caused deletion and couldnt find the file to copy from.
        if (fs::exists(target) && fs::equivalent(m_clipboardSource, target)) {
            m_clipboardSource.clear();
            m_lastOp = ClipboardOp::NONE;
            return true; 
        }

        // if source and target are not the same location 
        if (fs::exists(target)) {
            if (!overwriteConfirmed) {
                SetError("File already exists.");
                return false;
            }
            fs::remove_all(target); 
        }

        if (m_lastOp == ClipboardOp::COPY) {
            // recursive copy for contents
            fs::copy(m_clipboardSource, target, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        } else if (m_lastOp == ClipboardOp::CUT) {
            fs::rename(m_clipboardSource, target);
        }
        m_lastOp = ClipboardOp::NONE;
        m_clipboardSource.clear();

        return true;
    } catch (const fs::filesystem_error& e) {
        SetError(e.what());
        return false;
    }
}

/*
 * Function: GetDirectoryContents
 * Description: returns a vector of FileEntry objects for each item in the specified directory
 * Parameters: path: the path of the directory to list contents of
 * Returns: vector of FileEntry objects representing each item in the cd
 */
std::vector<FileEntry> FileManagerLogic::GetDirectoryContents(const fs::path& path) {
    std::vector<FileEntry> entries;
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            FileEntry fe;
            fe.name = entry.path().filename().string();
            fe.isDirectory = entry.is_directory();
            fe.type = fe.isDirectory ? "Folder" : entry.path().extension().string();
            
            if (!fe.isDirectory) {
                fe.size = FormatSize(fs::file_size(entry));
            } else {
                fe.size = "--";
            }

            auto ftime = fs::last_write_time(entry);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
            );
            std::time_t c_ftime = std::chrono::system_clock::to_time_t(sctp);
            
            std::stringstream ss;
            ss << std::put_time(std::localtime(&c_ftime), "%Y-%m-%d %H:%M");
            fe.modified = ss.str();

            entries.push_back(fe);
        }
    } catch (const fs::filesystem_error& e) {
        SetError(e.what());
    }
    return entries;
}

/*
 * Function: FormatSize
 * Description: converts a raw byte count into a human-readable string (B, KB, MB, GB)
 * Parameters: size: the size in bytes to be formatted
 * Returns: string representing the formatted size to two decimals
 */
std::string FileManagerLogic::FormatSize(uintmax_t size) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2);

    // convert bytes to human readable format
    if (size < 1024) out << size << " B";
    else if (size < 1024 * 1024) out << size / 1024.0 << " KB";
    else if (size < 1024 * 1024 * 1024) out << size / (1024.0 * 1024.0) << " MB";
    else out << size / (1024.0 * 1024.0 * 1024.0) << " GB";
    return out.str();
}
