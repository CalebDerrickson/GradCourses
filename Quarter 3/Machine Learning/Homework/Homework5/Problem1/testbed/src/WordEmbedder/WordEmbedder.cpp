#include "WordEmbedder.hpp"
#include <random>


WordEmbedder::WordEmbedder(const char* embeddingPath)
{
    fileManager = std::make_unique<FileManager<std::string>>();
    fileManager->setInPath(embeddingPath);
    fileManager->readData(' ');
}

WordEmbedder::~WordEmbedder()
{


}

void WordEmbedder::embed()
{
    std::cout<<"Embedding data randomly"<<std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    std::vector<std::vector<std::string>> data = fileManager->getData();
    for (int i = 0; i < data.size(); i++) { 
        if (wordEmbedding.find(data[i][0]) != wordEmbedding.end()) {
            continue;
        }
        vec_dbl temp(VECTOR_SIZE, 0.0);
        for (int j = 0; j < VECTOR_SIZE; j++) {
            temp[j] = dis(gen);
        }

        wordEmbedding[data[i][0]] = temp;
    }

}

void WordEmbedder::embedWord(const char* word)
{


}

void WordEmbedder::writeEmbedding()
{

    std::cout<<"Writing embedding to ../data/wordEmbedding.txt"<<std::endl;
    fileManager->setOutPath("../data/wordEmbedding.txt");
    fileManager->resetData();
    std::vector<std::vector<std::string>> data(wordEmbedding.size(), std::vector<std::string>());
    int index = 0;
    for (const auto& [key, val] : wordEmbedding) {
        std::string temp = key;
        for (double d : val) {
            temp += " " + std::to_string(d);
        }
        data[index++] = {temp};
    }

    fileManager->setData(data);
    fileManager->writeData(' ', std::ios::trunc);
}