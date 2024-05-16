#pragma once
#include <unordered_map>
#include <memory>

#include "utils.hpp"
#include "FileManager.hpp"



class WordEmbedder {
public:
    WordEmbedder(const char* embeddingPath);
    ~WordEmbedder();

    // Embeds all words in the filemanager
    void embed();

    void embedWord(const char* word);
    void writeEmbedding();


    std::unordered_map<std::string, vec_dbl> wordEmbedding;
    std::unique_ptr<FileManager<std::string>> fileManager;
};