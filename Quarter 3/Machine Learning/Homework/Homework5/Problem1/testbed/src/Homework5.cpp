#include "Homework5.hpp"
#include <iostream>


// Private Declarations
std::vector<std::vector<std::string>> parseLine(const std::vector<std::string>& line);
std::vector<mat_dbl> embedLine(std::vector<std::vector<std::string>>& line, std::unordered_map<std::string, vec_dbl>& wordEmbedding);

Homework5::Homework5(const char* embedPath, int numEpocs)
{
    _wordEmbedder = std::make_unique<WordEmbedder>(embedPath);
    _stringFileManager = std::make_unique<FileManager<std::string>>();
    _dblfileManager = std::make_unique<FileManager<double>>();
    _numEpocs = numEpocs;
    _lstm = std::make_unique<LSTM>();
}

Homework5::~Homework5()
{


}

void Homework5::init()
{
    // get _trainingData, _validateData, and _testData
    std::cout<<"Getting Training Data."<<std::endl;
    _stringFileManager->setInPath("../data/bobsue.seq2seq.train.tsv");
    _stringFileManager->readData(' ');
    _trainingData = std::make_unique<std::vector<std::vector<std::string>>>(_stringFileManager->getData());

    std::cout<<"Getting Testing Data."<<std::endl;
    _stringFileManager->resetData();
    _stringFileManager->setInPath("../data/bobsue.seq2seq.test.tsv");
    _stringFileManager->readData(' ');
    _testingData = std::make_unique<std::vector<std::vector<std::string>>>(_stringFileManager->getData());

    std::cout<<"Getting Validation Data."<<std::endl;
    _stringFileManager->resetData();
    _stringFileManager->setInPath("../data/bobsue.seq2seq.dev.tsv");
    _stringFileManager->readData(' ');
    _validateData = std::make_unique<std::vector<std::vector<std::string>>>(_stringFileManager->getData());
}

void Homework5::run()
{
    // Embedding words in vocabulary
    _wordEmbedder->embed();
    //_wordEmbedder->writeEmbedding(); For writing to file
    _wordEmbedder->writeEmbedding();
    for (int i = 1; i <= _numEpocs; i++) 
    {
        //train on all training data
        train(i);

        //Validate
        validate(i);
    }

    //test (finally)
    
    test();
}

void Homework5::train(int epoch) 
{
    for(int i = 0; i < _trainingData->size(); i++) {
        //parse line
        //parseLine separates the first and second sentence
        std::vector<std::vector<std::string>> parsedLine = parseLine((*_trainingData)[i]); // replace 0 with i
        
        //get embedding
        // bug here - tab is considered a word. Should be removed from sentence.
        // vector is two long. matrix is M x VECTOR_SIZE : 
        //  M is max length of the input and output sentence, VECTOR_SIZE is set in utils
        
        std::vector<mat_dbl> embedding = embedLine(parsedLine, _wordEmbedder->wordEmbedding);
        if (embedding[0].size() == 0 || embedding[1].size() == 0) continue;

        //forward through model
        std::vector<vec_dbl> sentenceOutput(embedding[1].size(), vec_dbl(VECTOR_SIZE, 0.0));
        std::vector<std::vector<vec_dbl>> corpi = forwardSentence(embedding, sentenceOutput);
        
        //collect actuals
        
        //collect corpi
        // Form W, U
        mat_dbl W(4 * VECTOR_SIZE, vec_dbl(VECTOR_SIZE, 0.0)); // W has size 4*VECTOR_SIZE x 2*VECTOR_SIZE
        mat_dbl U(4 * VECTOR_SIZE, vec_dbl(VECTOR_SIZE, 0.0)); // U has the same size as W
        mat_dbl b(4 , vec_dbl(VECTOR_SIZE, 0.0));

        std::vector<mat_dbl> Ws = _lstm->getWeights();
        std::vector<vec_dbl> bs = _lstm->getBiases();


        // Sort the Weights matrices into W, U for backprop
        // also the bias
        for (int j = 0; j < 4; ++j) {
            mat_dbl temp = Ws[j];
            for (int row = 0; row < temp.size(); ++row) {
                for (int col = 0; col < VECTOR_SIZE; ++col) {
                    W[j * VECTOR_SIZE + row][col] = temp[row][col];
                }
            }
            for (int row = 0; row < temp.size(); ++row) {
                for (int col = VECTOR_SIZE; col < 2 * VECTOR_SIZE; ++col) {
                    U[j * VECTOR_SIZE + row][col - VECTOR_SIZE] = temp[row][col];
                }
            }
            for (int k = 0; k < VECTOR_SIZE; ++k) {
                b[j][k] = bs[j][k]; 
            }
        }
        mat_dbl dW(4*VECTOR_SIZE, vec_dbl(VECTOR_SIZE, 0.0));
        mat_dbl dU(4*VECTOR_SIZE, vec_dbl(VECTOR_SIZE, 0.0));
        mat_dbl db(4, vec_dbl(VECTOR_SIZE, 0.0));
        backprop(W, U, b, corpi, embedding[1], dW, dU, db);
        // backprop (can) happen/s on each training example
        GradientDescent(W, U, b, corpi, embedding[1], dW, dU, db);
    
        std::cout<<"Training Epoch : "<<epoch<<" - "<<(double)(i+1)/_trainingData->size() * 100<<std::endl;
    }
}

void Homework5::validate(int epoch) 
{

    mat_dbl loss_data(1, vec_dbl(_validateData->size(), 0.0));
    for(int i = 0; i < _validateData->size(); i++) {
        //parse line
        //parseLine separates the first and second sentence
        std::vector<std::vector<std::string>> parsedLine = parseLine((*_validateData)[i]); // replace 0 with i
        
        //get embedding
        // bug here - tab is considered a word. Should be removed from sentence.
        // vector is two long. matrix is M x VECTOR_SIZE : 
        //  M is max length of the input and output sentence, VECTOR_SIZE is set in utils
        
        std::vector<mat_dbl> embedding = embedLine(parsedLine, _wordEmbedder->wordEmbedding);
        if (embedding[0].size() == 0 || embedding[1].size() == 0) continue;

        //forward through model
        std::vector<vec_dbl> sentenceOutput(embedding[1].size(), vec_dbl(VECTOR_SIZE, 0.0));
        std::vector<std::vector<vec_dbl>> corpi = forwardSentence(embedding, sentenceOutput);
        
        //collect actuals
        
        //collect corpi
        // Form W, U
        mat_dbl W(4 * VECTOR_SIZE, vec_dbl(VECTOR_SIZE, 0.0)); // W has size 4*VECTOR_SIZE x 2*VECTOR_SIZE
        mat_dbl U(4 * VECTOR_SIZE, vec_dbl(VECTOR_SIZE, 0.0)); // U has the same size as W
        mat_dbl b(4 , vec_dbl(VECTOR_SIZE, 0.0));

        std::vector<mat_dbl> Ws = _lstm->getWeights();
        std::vector<vec_dbl> bs = _lstm->getBiases();


        // Sort the Weights matrices into W, U for backprop
        // also the bias
        for (int j = 0; j < 4; ++j) {
            mat_dbl temp = Ws[j];
            for (int row = 0; row < temp.size(); ++row) {
                for (int col = 0; col < VECTOR_SIZE; ++col) {
                    W[j * VECTOR_SIZE + row][col] = temp[row][col];
                }
            }
            for (int row = 0; row < temp.size(); ++row) {
                for (int col = VECTOR_SIZE; col < 2 * VECTOR_SIZE; ++col) {
                    U[j * VECTOR_SIZE + row][col - VECTOR_SIZE] = temp[row][col];
                }
            }
            for (int k = 0; k < VECTOR_SIZE; ++k) {
                b[j][k] = bs[j][k]; 
            }
        }

        mat_dbl dts(corpi.size(), vec_dbl(embedding[1].size(), 0.0));
        for (int i = 0; i < dts.size(); i++) {
            dts[i] = utils::loss_func::MSE(embedding[1][i], corpi[i][4]);
        }


        double loss = 0.0;
        for (int i = 0; i < dts.size(); i++) {
            for (int j = 0; j < dts[0].size(); j++) {
                loss += dts[i][j];
            }
        }
        loss_data[0][i] = loss;
        std::cout << "Loss: " << loss << std::endl;
        std::cout<<"Validating Epoch : "<<epoch<<" - "<<(double)(i+1)/_validateData->size() * 100<<std::endl;

    }
    loss_data.push_back({0, 0, 0, 0});
    // writing loss data
    _dblfileManager->setOutPath("../data/loss_data.txt");
    _dblfileManager->setData(loss_data);
    _dblfileManager->writeData(' ', std::ios::app);

}

void Homework5::test() 
{
    // setup string file manager
    _dblfileManager->setOutPath("../data/prediction.txt");
    
    mat_dbl embededOutputs(0, vec_dbl(VECTOR_SIZE, 0.0));
    embededOutputs.reserve(VECTOR_SIZE * _trainingData->size());
    for(int i = 0; i < _testingData->size(); i++) {
        //parse line
        //parseLine separates the first and second sentence
        std::vector<std::vector<std::string>> parsedLine = parseLine((*_trainingData)[i]); 
                
        std::vector<mat_dbl> embedding = embedLine(parsedLine, _wordEmbedder->wordEmbedding);
        if (embedding[0].size() == 0 || embedding[1].size() == 0) continue;

        //forward through model
        std::vector<vec_dbl> sentenceOutput(embedding[1].size(), vec_dbl(VECTOR_SIZE, 0.0));
        auto corpi = forwardSentence(embedding, sentenceOutput);
        utils::printMat(sentenceOutput);
        for (int j = 0; j < sentenceOutput.size(); j++) {
            embededOutputs.push_back(sentenceOutput[j]);
        }
        embededOutputs.push_back({0, 0, 0, 0});
        std::cout<<"Testing... "<<(double)(i+1)/_testingData->size() * 100<<std::endl;
    }
    _dblfileManager->setData(embededOutputs);
    _dblfileManager->writeData(' ', std::ios::trunc);
}

void Homework5::backprop(mat_dbl& W, mat_dbl& U, mat_dbl& b, 
    const std::vector<std::vector<vec_dbl>>& corpi, const mat_dbl& actuals, mat_dbl& dW, mat_dbl& dU, mat_dbl& db)
{
        // Have to do this for type differences. Might change in the future
    auto convertToMatrix = [](const std::vector<vec_dbl>& subVec) -> mat_dbl {
        mat_dbl matrix;
        for (const auto& vec : subVec) {
            matrix.push_back(vec);
        }
        return matrix;
    };

    
    std::vector<mat_dbl> convertedMatrices;
    std::transform(corpi.begin(), corpi.end(), std::back_inserter(convertedMatrices), convertToMatrix);

    // Do backpropagation
    Backpropagation backprop(corpi.size(), actuals);
    backprop.setAll(convertedMatrices, actuals, &W, &U);

    backprop.doIt(dW, dU, db);

}

void Homework5::GradientDescent(mat_dbl& W, mat_dbl& U, mat_dbl& b, 
    const std::vector<std::vector<vec_dbl>>& corpi, const mat_dbl& actuals, mat_dbl& dW, mat_dbl& dU, mat_dbl& db) 
{
    


    // update stuff
    mat_dbl updateW(dW);
    mat_dbl updateU(dU);
    mat_dbl updateb(db);
    updateW = utils::matScale(updateW, -learning_rate);
    updateU = utils::matScale(updateU, -learning_rate);
    updateb = utils::matScale(updateb, -learning_rate);
    W = utils::MatMatAdd(W, updateW);
    U = utils::MatMatAdd(U, updateU);
    b = utils::MatMatAdd(b, updateb);


    // concat stuff, update gates
    //aifo
    // Iterate over each gate and update the weights and biases
    // Allocate newWeights on the heap
    mat_dbl* newWeights = new mat_dbl(VECTOR_SIZE, vec_dbl(2 * VECTOR_SIZE, 0.0));

    // Allocate newBias on the heap
    vec_dbl* newBias = new vec_dbl(VECTOR_SIZE, 0.0);
    for (int gate = 0; gate < NUM_GATES; ++gate) {


        // Update weights and biases for the current gate
        for (int i = 0; i < VECTOR_SIZE; ++i) {
            for (int j = 0; j < VECTOR_SIZE; ++j) {
                (*newWeights)[i][j] = W[i + gate * VECTOR_SIZE][j];
                (*newWeights)[i][j + VECTOR_SIZE] = U[i + gate * VECTOR_SIZE][j];
            }
            (*newBias)[i] = b[gate][i];
        }

        // Set the weights and biases for the current gate
        switch (gate) {
            case 0:
                _lstm->setCandGateWeights(*newWeights);
                _lstm->setCandGateBias(*newBias);
                
                break;
            case 1:
                _lstm->setInputGateWeights(*newWeights);
                _lstm->setInputGateBias(*newBias);
                
                break;
            case 2:
                _lstm->setForgetGateWeights(*newWeights);
                _lstm->setForgetGateBias(*newBias);
                
                break;
            case 3:
                _lstm->setOutputGateWeights(*newWeights);
                _lstm->setOutputGateBias(*newBias);
                break;
            default:
                break;
        }
    }
        // Clean up memory to avoid memory leaks
    delete newWeights;
    delete newBias;
}

std::vector<std::vector<vec_dbl>> Homework5::forwardSentence(const std::vector<mat_dbl>& embedding,  std::vector<vec_dbl>& sentenceOutput) 
{
    // First sentence is the input
    _lstm->setOutputState(output_state::DO_NOT_OUTPUT);
    std::vector<std::vector<vec_dbl>> corpus(embedding[0].size(), std::vector<vec_dbl>(6, vec_dbl(VECTOR_SIZE, 0.0)));
    
    for (int i = 0; i < embedding[0].size(); i++) {
        _lstm->forward(embedding[0][i], sentenceOutput[i], corpus[i]);
    }

    // Get predicted output.
    // override previous corpus. Want the output instead
    
    _lstm->setOutputState(output_state::OUTPUT);
    for (int i = 0; i < embedding[1].size(); i++) {
        _lstm->forward(embedding[1][i], sentenceOutput[i], corpus[i]);
    }

    return corpus;
}

// Private definitions

std::vector<std::vector<std::string>> parseLine(const std::vector<std::string>& words) {
    std::vector<std::vector<std::string>> res(2);
    bool firstSentence = true;

    // Loop through each word
    for (const auto& word : words) {
        int comp = word.compare("</s>");
        if ( comp == 0 || comp > 1 ) continue;
        comp = word.compare("<s>");
        if (comp == 0 || comp  > 1) continue;

        // Push the word to the appropriate sentence
        res[firstSentence ? 0 : 1].push_back(word);

        // If the word is ".", switch to the second sentence
        if (word == "." && firstSentence || word == "!" && firstSentence || word == "?" && firstSentence) {
            firstSentence = false;
        }
    }

    return res;
}



std::vector<mat_dbl> embedLine(std::vector<std::vector<std::string>>& line, std::unordered_map<std::string, vec_dbl>& wordEmbedding)
{
    // check the which line is longer
    int maxSize = line[0].size() > line[1].size() ? line[0].size() : line[1].size(); 
    
    std::vector<mat_dbl> embedding(2, mat_dbl(maxSize, vec_dbl(VECTOR_SIZE, 0.0)));
    for (int i = 0; i < maxSize; i++) {
        if (i < line[0].size()) embedding[0][i] = wordEmbedding[line[0][i]];
        if (i < line[1].size()) embedding[1][i] = wordEmbedding[line[1][i]];
    }

    return embedding;
}