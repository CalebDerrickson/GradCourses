#include <vector>
#include <string>
#include <iostream>
#include <fstream>
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
    if (_inPath == "") {
        std::cout<< 
            "InPath has not been specified. Either Specify it upon declaration\
            or call the function which takes inPath as an argument"<<std::endl; 
        return 1;
    }

    if (fraction > 1 || fraction <= 0) {
        std::cout<<"Fraction is bad value. Returning."<<std::endl;
        return 1;
    }

    std::ifstream file;
    file.open(_inPath);

    if (!file.is_open()) {
        std::cout<<" File path "<< _inPath << " is not found. Returning."<<std::endl;
        return 1;
    } 

    // Counting lines
    // I am stupid
    int lineCount = 0;
    std::string line1;
    while (std::getline(file, line1)) {
        ++lineCount;
    }

    // Reset file pointer
    file.clear();
    file.seekg(0, std::ios::beg);

    int i = 0;
    std::string line;
    while (std::getline(file, line)) {
        _data.push_back(parseLine<T>(line, del));
        i++;
        if (i > lineCount * fraction) break;
    }

    if (abs(fraction -1) > __FLT_EPSILON__) {
        std::cout<<"number of lines read : "<<i-1<<std::endl;
    }
    file.close();
    return 0;
}
template <typename T>
int FileManager<T>::writeData(const char del, const std::ios_base::openmode mode)
{
    
    if (_outPath == "") {
        std::cout<< 
            "outPath has not been specified. Either Specify it upon declaration\
            or call the function which takes outPath as an argument"<<std::endl; 
        return 1;
    }
    
    std::ofstream outFile;
    outFile.open(_outPath, mode);

    if (!outFile.is_open()) {
        std::cout<<" File path "<< _outPath << " is not found. Returning."<<std::endl;
        return 1;
    } 

    
    for (int i = 0; i < _data.size(); i++) {
        std::string line = "";
        for (int j = 0; j < _data[i].size(); j++) {
            if (j != _data[i].size() - 1) line.append(std::to_string(_data[i][j]) + del);
            else line.append(std::to_string(_data[i][j]));
        }
        outFile << line<<std::endl;
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

template <typename T>
std::vector<T> parseLine(std::string line, const char del)
{
    int size = line.length();
    int lastdel = 0;
    std::vector<T> res{};


    for (int i = 0; i < size; i++) {
        
        if (line[i] != del) {
            continue;
        }

        // Hit delimiter, push back
        std::string temp = line.substr(lastdel, i - lastdel);

        if (temp[0] == '-') {
            temp = temp.substr(1);
            if (std::is_same<T, int>::value) {res.push_back(-1*std::stoi(temp));}
            else if (std::is_same<T, double>::value) {res.push_back(-1*std::stod(temp));}
        }
        else{
            if (std::is_same<T, int>::value) {res.push_back(std::stoi(temp));}
            else if (std::is_same<T, double>::value) {res.push_back(std::stod(temp));}
        } 

        lastdel = i; 
    }

    // Get last data point
    std::string temp = line.substr(lastdel, size - lastdel);
    if (temp[0] != ' ')
    {
        if (temp[0] == '-') {
            temp = temp.substr(1);
            if (std::is_same<T, int>::value) {res.push_back(-1*std::stoi(temp));}
            else if (std::is_same<T, double>::value) {res.push_back(-1*std::stod(temp));}
        }
        else {
            if (std::is_same<T, int>::value) {res.push_back(std::stoi(temp));}
            else if (std::is_same<T, double>::value) {res.push_back(std::stod(temp));}
        } 
    }        

    return res;
}

