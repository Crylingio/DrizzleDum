
#ifndef FILESYSTEM_H
#define FILESYSTEM_H
//#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>


namespace baelothelib
{
    struct Filedata
    {
        // Data storage
        std::vector<std::string> m_strs;
        std::vector<int> m_nums;
        std::vector<float> m_floats;

        Filedata() {} // ctor
        Filedata(const std::initializer_list<std::string>& strs,
                 const std::initializer_list<int>& nums,
                 const std::initializer_list<float>& floats)
                 {
                     // Add data to each vector
                     m_strs.reserve(strs.size());
                     m_nums.reserve(nums.size());
                     m_floats.reserve(floats.size());
                     for(auto s = strs.begin(); s != strs.end(); ++s)
                        m_strs.push_back(*s);
                     for(auto n = nums.begin(); n != nums.end(); ++n)
                        m_nums.push_back(*n);
                     for(auto f = floats.begin(); f != floats.end(); ++f)
                        m_floats.push_back(*f);
                 } // ctor
        Filedata(const Filedata& other) // copy ctor
        {
            this->m_strs = other.m_strs;
            this->m_nums = other.m_nums;
            this->m_floats = other.m_floats;
        }
    };

    // Error Codes
    static const unsigned int FILE_ERROR_NOT_OPEN = 1;
    static const unsigned int FILE_ERROR_FAILED = 2;

    class FileWriter
    {
    private:
        /// Pointer to current file data.
        Filedata* m_file = nullptr;
        /// Output stream instance.
        std::ofstream m_stream;
        /// Headers in the file.
        std::vector<std::string> m_headers =
        {
            "[file begin]", // File begin header
            "[strings]", // String data header
            "[integers]", // Integer data header
            "[floats]", // Float data header
            "[file end: created with baelothelib vers. 1.2]" // File end header
        };
    public:
        FileWriter() {} // ctor
        FileWriter(Filedata& f) // ctor
        {
            m_file = &f; // Set pointer
        }
        bool write(const std::string& file_name);
        FileWriter& open(Filedata& f);
    };

    class FileReader
    {
    private:
        /// Filedata most recently read.
        Filedata m_file;
        /// Input stream instance.
        std::ifstream m_stream;
        /// File name to be read.
        std::string m_name;

        bool is_float(const std::string& str);
        bool is_int(const std::string& str);
    public:
        FileReader() {} // ctor
        FileReader(const std::string& file_name) // ctor
        {
            m_name = file_name; // Set name
        }
        bool read(Filedata& r);
        FileReader& open(const std::string& file_name);
    };
}

#endif // FILESYSTEM_H header guard