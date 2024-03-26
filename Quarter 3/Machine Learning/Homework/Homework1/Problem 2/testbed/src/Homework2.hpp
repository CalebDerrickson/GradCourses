#include <vector>
#include <string>

class Homework2 {
public:
    Homework2(const char* filePath, const int k);
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

// Private values
    // _data is the number of points being read in.
    std::vector<std::vector<double>> _data;

    // _outData will be the result; here kmeans has been applied, and we have cluster assignments.
    std::vector<std::vector<double>> _outData;

    // the path of the data
    const char* _filePath;
    
    // The centroids of each cluster
    std::vector<std::vector<double>> _clusterCentroids;

    // Contains the cluster assignment for every point in _data
    std::vector<int> _clusterAssignment;

    std::vector<double> _distortion;

    int _numClusters;
};