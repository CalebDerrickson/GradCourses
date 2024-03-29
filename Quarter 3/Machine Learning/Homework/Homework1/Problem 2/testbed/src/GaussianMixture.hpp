#include <vector>
#include <string>

enum flags {RANDOM_CLUSTERS, KMEANSPP};

typedef struct Sigma {
    std::vector<std::vector<double>> values;
    int numRows;
    int NumColumns;
} Sigma;


class GaussianMixture {
public:
    GaussianMixture(const char* filePath, const char* outPath, const int k);
    ~GaussianMixture();

    int Run();

private:
// Private Functions
    int ReadInData();
    int WriteOutData();
    void PrintData();
    std::vector<double> ParseLine(std::string& row);
    int InitGaussians();
    void AssignPointstoClusters();
    void updateClusterPriors();
    void updateClusterMeans();
    void updateClusterCovariance();
    double computePij(const int i, const int j);
    double calculateLogLikelihood();

    int GaussianMixtureCalc();

// Private values
    // _data is the number of points being read in.
    std::vector<std::vector<double>> _data;

    // _outData will be the result; here kmeans has been applied, and we have cluster assignments.
    std::vector<std::vector<double>> _outData;

    // the path of the data
    const char* _filePath;
    
    const char* _outPath;

    int _numClusters;

    std::vector<int> _gaussianAssignment;

    std::vector<double> _logLikelihood;

    std::vector<double> _clusterPriors;
    std::vector<std::vector<double>> _clusterMean;
    std::vector<Sigma> _clusterCovariance;

};