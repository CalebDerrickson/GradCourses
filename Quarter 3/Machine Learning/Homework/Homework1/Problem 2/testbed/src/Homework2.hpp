#include <vector>
#include <string>

enum flags {RANDOM_CLUSTERS, KMEANSPP};
class Homework2 {
public:
    Homework2(const char* filePath, const int k, const char* outPath, flags flag);
    ~Homework2();

    int Run();

private:
// Private Functions
    int ReadInData();
    int WriteOutData();
    void PrintData();
    int KMeans();
    std::vector<double> ParseLine(std::string& row);
    void InitClusters();
    void CalculateDistortion();
    void RandomInitClusters();
    void KMeansPlusPlus();

// Private values
    // _data is the number of points being read in.
    std::vector<std::vector<double>> _data;

    // _outData will be the result; here kmeans has been applied, and we have cluster assignments.
    std::vector<std::vector<double>> _outData;

    // the path of the data
    const char* _filePath;
    
    // TODO: Outputs only distortion, want to also output classification 
    const char* _outPath;

    // The centroids of each cluster
    std::vector<std::vector<double>> _clusterCentroids;

    // Contains the cluster assignment for every point in _data
    std::vector<int> _clusterAssignment;

    std::vector<double> _distortion;

    int _numClusters;

    flags _initClustersFlag;
};