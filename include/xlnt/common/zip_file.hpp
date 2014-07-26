// Copyright (c) 2014 Thomas Fussell
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, WRISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE
//
// @license: http://www.opensource.org/licenses/mit-license.php
// @author: see AUTHORS file
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <zip.h>
#include <unzip.h>

namespace xlnt {

/// <summary>
/// Defines constants for read, write, or read/write access to a file.
/// </summary>
enum class file_access
{
    /// <summary>
    /// Read access to the file. Data can only be read from the file. Combine with write for read/write access.
    /// </summary>
    read = 0x01,
    /// <summary>
    /// Read and write access to the file. Data can be both written to and read from the file.
    /// </summary>
    read_write = 0x03,
    /// <summary>
    /// Write access to the file. Data can only be written to the file. Combine with read for read/write access.
    /// </summary>
    write = 0x02
};

/// <summary>
/// Specifies how the operating system should open a file.
/// </summary>
enum class file_mode
{
    /// <summary>
    /// Opens the file if it exists and seeks to the end of the file, or creates a new file.This requires FileIOPermissionAccess.Append permission.file_mode.Append can be used only in conjunction with file_access.Write.Trying to seek to a position before the end of the file throws an IOException exception, and any attempt to read fails and throws a NotSupportedException exception.
    /// </summary>
    append,
    /// <summary>
    /// Specifies that the operating system should create a new file. If the file already exists, it will be overwritten. This requires FileIOPermissionAccess.Write permission. file_mode.Create is equivalent to requesting that if the file does not exist, use CreateNew; otherwise, use Truncate. If the file already exists but is a hidden file, an UnauthorizedAccessException exception is thrown.
    /// </summary>
    create,
    /// <summary>
    /// Specifies that the operating system should create a new file. This requires FileIOPermissionAccess.Write permission. If the file already exists, an IOException exception is thrown.
    /// </summary>
    create_new,
    /// <summary>
    /// Specifies that the operating system should open an existing file. The ability to open the file is dependent on the value specified by the file_access enumeration. A System.IO.FileNotFoundException exception is thrown if the file does not exist.
    /// </summary>
    open,
    /// <summary>
    /// Specifies that the operating system should open a file if it exists; otherwise, a new file should be created. If the file is opened with file_access.Read, FileIOPermissionAccess.Read permission is required. If the file access is file_access.Write, FileIOPermissionAccess.Write permission is required. If the file is opened with file_access.ReadWrite, both FileIOPermissionAccess.Read and FileIOPermissionAccess.Write permissions are required.
    /// </summary>
    open_or_create,
    /// <summary>
    /// Specifies that the operating system should open an existing file. When the file is opened, it should be truncated so that its size is zero bytes. This requires FileIOPermissionAccess.Write permission. Attempts to read from a file opened with file_mode.Truncate cause an ArgumentException exception.
    /// </summary>
    truncate
};
    
class zip_file
{
private:
    enum class state
    {
        read,
        write,
        closed
    };
    
public:
    zip_file(const std::string &filename, file_mode mode, file_access access = file_access::read);
    
    ~zip_file();

    std::string get_file_contents(const std::string &filename) const;
    
    void set_file_contents(const std::string &filename, const std::string &contents);
    
    void delete_file(const std::string &filename);
    
    bool has_file(const std::string &filename);
    
    void flush(bool force_write = false);
    
    std::string get_filename() const { return filename_; }
    
private:
    void read_all();
    void write_all();
    std::string read_from_zip(const std::string &filename);
    void write_to_zip(const std::string &filename, const std::string &content, bool append = true);
    void change_state(state new_state, bool append = true);
    static bool file_exists(const std::string& name);
    void start_read();
    void stop_read();
    void start_write(bool append);
    void stop_write();
    
    zipFile zip_file_;
    unzFile unzip_file_;
    state current_state_;
    std::string filename_;
    std::unordered_map<std::string, std::string> files_;
    bool modified_;
    file_access access_;
    std::vector<std::string> directories_;
};
    
} // namespace xlnt
