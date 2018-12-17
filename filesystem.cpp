#include "filesystem.h"
//#include "stdafx.h"

/** Writes the opened Filedata to disk.
** @param Name of the file to be saved under
** @return True if successful
**/
bool baelothelib::FileWriter::write(const std::string& file_name)
{
    // Check if a file's set to write
    if(m_file == nullptr) throw FILE_ERROR_NOT_OPEN; // Throw error

    m_stream.open(file_name, std::ios::out | std::ios::trunc); // Create the file
    if(m_stream.is_open()) // Check if file is open
    {
        m_stream << m_headers.at(0) << "\n\n"; // Print begin header
        // Write down data
        if(m_file->m_strs.size() > 0) // If there's string data
        {
            m_stream << m_headers.at(1) << "\n"; // Print strings header
            for(const std::string& s : m_file->m_strs) // Print each value (each on new line)
                m_stream << s << "\n";
            m_stream << "\n"; // Add extra new line
        }
        if(m_file->m_nums.size() > 0) // If there's integer data
        {
            m_stream << m_headers.at(2) << "\n"; // Print integers header
            for(const int& n : m_file->m_nums) // Print each value (each on new line)
                m_stream << n << "\n";
            m_stream << "\n"; // Add extra new line
        }
        if(m_file->m_floats.size() > 0) // If there's float data
        {
            m_stream << m_headers.at(3) << "\n"; // Print floats header
            for(const float& f : m_file->m_floats) // Print each value (each on new line)
                m_stream << f << "\n";
            m_stream << "\n"; // Add extra new line
        }
        m_stream << m_headers.at(4) << "\n"; // Print end header
    }
    else throw FILE_ERROR_FAILED; // Throw error if file failed to open

    m_stream.close(); // Close stream
    m_file = nullptr; // Unload file
    return true; // Success
}

/** Opens Filedata to later write to disk.
** @param Filedata to open.
** @return *this
**/
baelothelib::FileWriter& baelothelib::FileWriter::open(Filedata& f)
{
    if(m_file != nullptr) // Deallocate dynamic memory
        delete m_file;
    m_file = &f; // Set pointer
    return *this;
}

/** Returns if the given string is a float.
** @param String to check
** @return True if float
**/
bool baelothelib::FileReader::is_float(const std::string& str)
{
    std::istringstream iss(str);
    float f;
    iss >> std::noskipws >> f;
    // Check if entire string was consumed and if either fail bit or bad bit is set
    return iss.eof() && !iss.fail();
}

/** Returns if the given string is a single integer.
** @param String to check
** @return True if int
**/
bool baelothelib::FileReader::is_int(const std::string& str)
{
    std::istringstream iss(str);
    int n;
    iss >> std::noskipws >> n;
    // Check if entire string was consumed and if either fail bit or bad bit is set
    return iss.eof() && !iss.fail();
}

/** Reads from a file on disk to be stored on the passed Filedata.
** @param Filedata to be written to.
** @return True if successful
**/
bool baelothelib::FileReader::read(Filedata& r)
{
    // Check if a file's set to read
    if(m_name == "\n") throw FILE_ERROR_NOT_OPEN; // Throw error

    m_stream.open(m_name, std::ios::in); // Open the file
    if(m_stream.is_open()) // Check if file is open
    {
        std::string line; // Holds last read line
        std::vector<std::string> buffer; // Holds valid lines to be parsed
        while(std::getline(m_stream, line)) // Get lines until end of file (EOF) is reached
        {
            if(line[0] != '[' && !line.empty()) // If line isn't a header, and isn't empty
            {
                // Add to vector
                buffer.push_back(line);
            }
        }
        // Parse data
        for(const std::string& s : buffer)
        {
            std::istringstream iss(s);
            if(is_int(s))
            {
                int n;
                iss >> std::noskipws >> n;
                r.m_nums.push_back(n);
            }
            else if(is_float(s))
            {
                float f;
                iss >> std::noskipws >> f;
                r.m_floats.push_back(f);
            }
            else r.m_strs.push_back(s);
        }
    }
    else throw FILE_ERROR_FAILED; // Throw error if file failed to open

    m_stream.close(); // Close stream
    return true; // Success
}

/** Opens file on disk to later read to Filedata.
** @param File name to open.
** @return *this
**/
baelothelib::FileReader& baelothelib::FileReader::open(const std::string& file_name)
{
    m_name = file_name; // Set path
    return *this;
}
