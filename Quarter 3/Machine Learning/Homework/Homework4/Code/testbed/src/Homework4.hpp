#include "FileManager.hpp"
#include "utils.hpp"
#include <memory>

// Forward declaration. 
class HMM;

class Homework4 {
public:
    
    Homework4(const char* inFilePath, const char* outFilePath);
    ~Homework4();


    state run();
    state initHMM();

    private:
    state _internal_state;
    std::unique_ptr<FileManager<int>> _inData; 
    std::unique_ptr<FileManager<int>> _outData;
    std::unique_ptr<HMM> _HMM;
};