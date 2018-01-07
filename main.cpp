/**
Pedro Martins, FCT-UNL, 2018

Aplication features:
1 - reorganize feature vector by columns
**/

#include "opencv2/core.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
using namespace cv;

//input vector to be processed
const char *feature_vector = "";
//list of columns to retain
const char *retain_cols = "";
//file to save result in CSV format
string outputPath ="" ;
int total_items, numberOfFeatures, numberCols;
vector< vector<double> > featureData;
vector<int> rCols;
vector< vector<double> > processedData;

///methods
string printHelp();
void countData();
void loadData();
void countColsRetain();
void loadColsRetain();
void process();
void saveData();

int main(int argc, const char **argv)
{
    auto start = chrono::high_resolution_clock::now();
    cout << "OpenCV reorganizing tool.\n";
    try
    {
        const char *keys = "{ h | | }";

        CommandLineParser cmd(argc, argv, keys);

        ///parse the arguments

        if(argc < 2 )
        {
            cout << printHelp()<<endl;
            return 0;
        }
        feature_vector = argv[1];
        retain_cols = argv[2];


        countData();
        countColsRetain();

        ///setup output files for norm and minmax values
        string tempName = feature_vector;
        string destName = tempName.substr(tempName.find_last_of('/') + 1, tempName.size());
        outputPath = destName.substr(0, destName.find_last_of('.'))+"_reorg.csv";


        cout << total_items <<" samples, "<< numberOfFeatures <<" features, "<<numberCols<<" cols to retain."<<endl;
        ///Initialize vectors
        featureData.assign(total_items, vector<double>(numberOfFeatures,0));
        processedData.assign(total_items, vector<double>(numberCols,0));
        rCols.assign(numberCols,0);

        loadData();         //1 - load feature vector from CSV file
        loadColsRetain();
        process();
        saveData();         //4 - save processed vector to CSV

    }
    catch (const exception &e)
    {
        cout << "error: " << e.what() << endl;
        return -1;
    }
    auto fim = chrono::high_resolution_clock::now();
    cout << "processed in: " << std::chrono::duration_cast<chrono::milliseconds>(fim - start).count()
         << " ms" << endl;
    return 0;
}

void countData()
{

/// Count features and samples //////////////////////////////////////////////////////
    ifstream inputFeatureCountData(feature_vector);
    string current_line;
    int sampleCount = 0;     //number of samples
    int featureCount = 0;    //number of features

    // Start reading lines as long as there are lines in the file
    while(getline(inputFeatureCountData, current_line))
    {
        stringstream temp(current_line);
        string single_value;

        if (sampleCount==0)
        {
            while(getline(temp,single_value,','))
                featureCount++;
        }
        sampleCount++;
    }
    inputFeatureCountData.close();
    total_items = sampleCount;
    numberOfFeatures = featureCount;
}

void loadColsRetain()
{

/// Count cols //////////////////////////////////////////////////////
    ifstream inputColsToRetain(retain_cols);
    string current_line;
    int colCount = 0;     //number of columns

    // Start reading lines as long as there are lines in the file
    while(getline(inputColsToRetain, current_line))
    {
        stringstream temp(current_line);
        string single_value;

        while(getline(temp,single_value,','))
        {
            rCols.at(colCount) = atoi(single_value.c_str());
            colCount++;
        }
        cout<<"cols to retain loaded!"<<endl;
    }
    inputColsToRetain.close();
}

void countColsRetain()
{
/// Count cols //////////////////////////////////////////////////////
    ifstream inputColsToRetain(retain_cols);
    string current_line;
    int colCount = 0;     //number of columns

    // Start reading lines as long as there are lines in the file
    while(getline(inputColsToRetain, current_line))
    {
        stringstream temp(current_line);
        string single_value;

        while(getline(temp,single_value,','))
            colCount++;
    }

    inputColsToRetain.close();
    numberCols = colCount;
    cout<<"counted cols: "<< numberCols<<endl;
}

void loadData()
{
    /// Set up features data //////////////////////////////////////////////////////
    ifstream inputFeatureData(feature_vector);
    string current_training_line;

    int y = 0;     //sample index
    // Start reading lines as long as there are lines in the file
    while(getline(inputFeatureData, current_training_line))
    {
        stringstream temp(current_training_line);
        string single_value;
        int z = 0;
        vector< double > tempVector(numberOfFeatures,0);
        while(getline(temp,single_value,','))
        {
            // convert the string element to a float value
            tempVector[z] = atof(single_value.c_str());
            z++;
        }
        featureData.at(y) = tempVector;
        y++;
    }
    inputFeatureData.close();
    cout<<"data loaded!"<<endl;
}

void process()
{
    for (int y = 0; y < total_items; y++)
    {
        for (int x = 0; x < numberCols; x++)
        {
            int column = rCols.at(x);
            processedData.at(y).at(x) = featureData.at(y).at(column-1);
        }
    }
}

void saveData()
{
    ofstream evalBinData (outputPath.c_str());

    for (int y = 0; y < total_items; y++)
    {
        for (int x = 0; x < numberCols; x++)
        {
            if(x!= numberCols-1)
                evalBinData  << (double)processedData.at(y).at(x) << ",";
            else
                evalBinData  << (double)processedData.at(y).at(x) << "\n";
        }
    }
    evalBinData.close();
    cout << "\nfinal data saved to file: " << outputPath << endl;
}

string printHelp()
{
    string h ="Help\n"
              "Usage: tool-reorg <featureFile> <colRetainFile>\n\n";
    return h;
}
