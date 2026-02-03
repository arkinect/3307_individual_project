/*
 * Author: Mathew Lane
 * Description: <short file description>
 * Date: 2026-02-02
 */

#include "FileManagerLogic.h"
#include <iomanip>
#include <sstream>
#include <chrono>

// Core Operations Implementation
FileManagerLogic::FileManagerLogic() : m_lastOp(ClipboardOp::NONE) {
    m_currentPath = fs::current_path();
}

FileManagerLogic::~FileManagerLogic() {
    // Standard cleanup; filesystem paths handle their own memory
}

bool FileManagerLogic::CreateFolder(const std::string& name) {
    try {
        // Construct the full path and create the directory
        return fs::create_directory(m_currentPath / name);
    } catch (const fs::filesystem_error& e) {
        SetError(e.what());
        return false;
    }
}

bool FileManagerLogic::RenameItem(const fs::path& oldPath, const std::string& newName) {
    try {
        // Standard rename takes the old path and a full new path
        fs::path newPath = oldPath.parent_path() / newName;
        fs::rename(oldPath, newPath);
        return true;
    } catch (const fs::filesystem_error& e) {
        SetError(e.what());
        return false;
    }
}

bool FileManagerLogic::DeleteItem(const fs::path& path) {
    try {
        // remove_all handles both single files and non-empty directories
        return fs::remove_all(path) > 0;
    } catch (const fs::filesystem_error& e) {
        SetError(e.what());
        return false;
    }
}

// Virtual Clipboard Implementation

void FileManagerLogic::Copy(const fs::path& source) {
    m_clipboardSource = source;
    m_lastOp = ClipboardOp::COPY;
}

void FileManagerLogic::Cut(const fs::path& source) {
    m_clipboardSource = source;
    m_lastOp = ClipboardOp::CUT;
}

bool FileManagerLogic::Paste(const fs::path& destination, bool overwriteConfirmed) {
    try {
        if (m_clipboardSource.empty() || m_lastOp == ClipboardOp::NONE) {
            SetError("Clipboard is empty.");
            return false;
        }

        fs::path target = destination / m_clipboardSource.filename();

        // 1. Check if source and target are the same location
        if (fs::exists(target) && fs::equivalent(m_clipboardSource, target)) {
            // Simply return true because the "work" is already done
            m_clipboardSource.clear();
            m_lastOp = ClipboardOp::NONE;
            return true; 
        }

        // 2. Handle overwrite logic for DIFFERENT locations
        if (fs::exists(target)) {
            if (!overwriteConfirmed) {
                SetError("File already exists.");
                return false;
            }
            fs::remove_all(target); 
        }

        if (m_lastOp == ClipboardOp::COPY) {
            // copy_options::recursive ensures we copy folder contents
            fs::copy(m_clipboardSource, target, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        } else if (m_lastOp == ClipboardOp::CUT) {
            fs::rename(m_clipboardSource, target);
        }
        m_lastOp = ClipboardOp::NONE; // Reset after a move
        m_clipboardSource.clear();

        return true;
    } catch (const fs::filesystem_error& e) {
        SetError(e.what());
        return false;
    }
}

std::vector<FileEntry> FileManagerLogic::GetDirectoryContents(const fs::path& path) {
    std::vector<FileEntry> entries;
    try {
        // Iterate through all items in the directory
        for (const auto& entry : fs::directory_iterator(path)) {
            FileEntry fe;
            fe.name = entry.path().filename().string();
            fe.isDirectory = entry.is_directory();
            fe.type = fe.isDirectory ? "Folder" : entry.path().extension().string();
            
            // Get human-readable size for files
            if (!fe.isDirectory) {
                fe.size = FormatSize(fs::file_size(entry));
            } else {
                fe.size = "--";
            }

            // Convert filesystem time to human-readable format
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

std::string FileManagerLogic::FormatSize(uintmax_t size) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2);
    // Convert bytes to the largest relevant unit
    if (size < 1024) out << size << " B";
    else if (size < 1024 * 1024) out << size / 1024.0 << " KB";
    else if (size < 1024 * 1024 * 1024) out << size / (1024.0 * 1024.0) << " MB";
    else out << size / (1024.0 * 1024.0 * 1024.0) << " GB";
    return out.str();
}
