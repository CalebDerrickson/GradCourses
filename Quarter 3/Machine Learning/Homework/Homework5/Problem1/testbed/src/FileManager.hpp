#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// O templates, why you must be so annoying?

//****************************
//          CLASS 
//****************************
template <typename T>
class FileManager
{

public:
    FileManager();
    FileManager(const std::string& inPath, const std::string& outPath);

    ~FileManager();

    int readData(const char del, double fraction = 1);
    int writeData(const char del, const std::ios_base::openmode mode);

    // Used when inPath and OutPath are not specified
    int readData(const char del, const std::string inPath, double fraction = 1);
    int writeData(const char del, const std::string outPath, const std::ios_base::openmode mode);
    int writeDataStr(const char del, const std::ios_base::openmode mode);
    
    std::vector<std::vector<T>> getData() {return _data;}
    void setData(std::vector<std::vector<T>> &data) {_data = data;}
    void resetData();

    const std::string& getInPath() const noexcept {return _inPath;}
    const std::string& getOutPath() const noexcept {return _outPath;}
    void setInPath(std::string inPath) {_inPath = inPath;}
    void setOutPath(std::string outPath) {_outPath = outPath;}


    void printData();
    
    std::vector<std::vector<T>>* getDataPtr(){return &_data;}

private:

    std::string _inPath;
    std::string _outPath;
    std::vector<std::vector<T>> _data;

};


//****************************
//       DEFINITIONS 
//****************************

// Private declarations
template <typename T>
std::vector<T> parseLine(std::string line, const char del);


template <typename T>
FileManager<T>::FileManager()
{
    _inPath = "";
    _outPath = "";

}

template <typename T>
FileManager<T>::FileManager(const std::string& inPath, const std::string& outPath) :
    _inPath(inPath), _outPath(outPath)
{


}


template <typename T>
FileManager<T>::~FileManager()
{
    _data.clear();
    _data.shrink_to_fit();
}


template <typename T>
int FileManager<T>::readData(const char del, double fraction)
{
    if (_inPath.empty()) {
        std::cout << "InPath has not been specified. Either specify it upon declaration or call the function which takes inPath as an argument" << std::endl;
        return 1;
    }

    if (fraction > 1 || fraction <= 0) {
        std::cout << "Fraction is a bad value. Returning." << std::endl;
        return 1;
    }

    std::ifstream file(_inPath);
    if (!file.is_open()) {
        std::cout << "File path " << _inPath << " is not found. Returning." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        _data.push_back(parseLine<T>(line, del));
    }

    std::cout << "Number of lines read: " << _data.size() << std::endl;
    
    return 0;
}



// General template for writeData function
template <typename T>
int FileManager<T>::writeData(const char del, const std::ios_base::openmode mode) {
    if (std::is_same<std::string, T>::value) return writeDataStr(del, mode);
    if (_outPath.empty()) {
        std::cout << "OutPath has not been specified. Either specify it upon declaration or call the function which takes outPath as an argument" << std::endl;
        return 1;
    }

    std::ofstream outFile(_outPath, mode);

    if (!outFile.is_open()) {
        std::cout << "File path " << _outPath << " is not found. Returning." << std::endl;
        return 1;
    }

    for (const auto& row : _data) {
        for (size_t j = 0; j < row.size(); ++j) {
            outFile << row[j];
            if (j != row.size() - 1)
                outFile << del;
        }
        outFile << std::endl;
    }

    outFile.close();
    return 0;
}


// Overload for std::string type
template <typename T>
int FileManager<T>::writeDataStr(const char del, const std::ios_base::openmode mode) {
    if (_outPath.empty()) {
        std::cout << "OutPath has not been specified. Either specify it upon declaration or call the function which takes outPath as an argument" << std::endl;
        return 1;
    }

    std::ofstream outFile(_outPath, mode);

    if (!outFile.is_open()) {
        std::cout << "File path " << _outPath << " is not found. Returning." << std::endl;
        return 1;
    }

    for (const auto& row : _data) {
        for (size_t j = 0; j < row.size(); ++j) {
            outFile << row[j];
            if (j != row.size() - 1)
                outFile << del;
        }
        outFile << std::endl;
    }

    outFile.close();
    return 0;
}


template<typename T>
int FileManager<T>::readData(const char del, const std::string inPath, double fraction)
{
    // Setting now
    _inPath = inPath;

    return readData(del, fraction);
}

template<typename T>
int FileManager<T>::writeData(const char del, const std::string outPath, const std::ios_base::openmode mode)
{
    // Setting now
    _outPath = outPath;

    return writeData(del, mode=mode);
}

template<typename T>
void FileManager<T>::resetData()
{
    for (auto& row : _data) {
        row.clear();
    }

    _data.clear();

}


template <typename T>
void FileManager<T>::printData()
{
    const int nRows = _data.size();
    const int nCols = _data[0].size(); // Assumed constant

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            std::cout<< _data[i][j] << " ";
        }
        std::cout<<std::endl;
    }
}



//****************************
// PRIVATE METHOD DEFINITIONS
//****************************

// Specialization for strings
template <>
inline std::vector<std::string> parseLine<std::string>(std::string line, const char del)
{
    std::istringstream ss(line);
    std::string token;
    std::vector<std::string> res;
    while (std::getline(ss, token, del))
    {
        res.push_back(token);
    }
    return res;
}

// General template for numeric types
template <typename T>
inline std::vector<T> parseLine(std::string line, const char del)
{
    std::istringstream ss(line);
    std::string token;
    std::vector<T> res;
    while (std::getline(ss, token, del))
    {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>)
        {
            T val;
            if (token.find_first_not_of("0123456789.-") == std::string::npos)
            {
                std::istringstream(token) >> val;
                res.push_back(val);
            }
        }
    }
    return res;
}

