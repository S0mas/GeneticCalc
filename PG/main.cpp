#include <QCoreApplication>
#include "IExpressionTree.h"
#include "Logger.h"
#include "GeneticAlgorithm.h"
#include <fstream>
#include <time.h>
#include <cstdio>
#include <vector>
#include <QDir>
#include <QTextStream>

bool Logger::debugPrintsOn;
std::vector<std::string> AbstractExpressionNode::deletedAddresses;

bool loadDataFile(const QString& fileName, GeneticAlgorithm::Data& data)
{
    QString dir = QDir::currentPath();
    QFile file(dir+"//"+fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        Logger::printError("Opening file error! " + dir.toStdString());
        return false;
    }

    QTextStream in(&file);
    QRegExp rx("(\\;)");
    while (!in.atEnd())
    {
       QString line = in.readLine();
       QStringList list = line.split(rx);

       std::vector<double> valuesVec;

       for(int i = 0; i < 3; ++i)
       {
           QString str = list[i];
           if(i%3 != 0)
               str.remove(0, 1);

           bool ok = false;
           if(i%3 != 2)
               valuesVec.push_back(str.toDouble(&ok));
           else
               data.dataResults.push_back(str.toDouble(&ok));

           if(!ok)
           {
               Logger::printError("Error while casting to double: " + str.toStdString());
               return false;
           }
       }
       data.dataValues.push_back(valuesVec);
    }
    return true;
}

void writeResult(const GeneticAlgorithm::Result& result, const std::string& setup)
{
    std::string dir = QDir::currentPath().toStdString();
    std::ofstream file;

    file.open(dir+"results.txt", std::ios_base::app);

    std::string res = setup + " Result: " + std::to_string(result.value) + " Expression: " + result.expression + "\n";
    file << res;

    file.close();
}

void insertFileNames(std::vector<QString>& files)
{
    files.push_back("ZMPO_lista_4_00_punkt.txt");
    files.push_back("ZMPO_lista_4_01_2_punkty.txt");
    files.push_back("ZMPO_lista_4_02_3_punkty.txt");
    files.push_back("ZMPO_lista_4_03_sin_x.txt");
    files.push_back("ZMPO_lista_4_04_sin_x_y.txt");
    files.push_back("ZMPO_lista_4_05_x_y_pl_x_x.txt");
    files.push_back("ZMPO_lista_4_06_x_y_pl_x_x_noise.txt");
}

int main(int argc, char *argv[])
{
    qsrand(time(0));
    QCoreApplication a(argc, argv);
    std::vector<QString> files;
    insertFileNames(files);

    GeneticAlgorithm genAlg;
    GeneticAlgorithm::Data data;
    GeneticAlgorithm::Result result;
    GeneticAlgorithm::Setup setup(200, 45, 50, 100, 1);

    for(auto& fileName : files)
    {
        data.dataResults.clear();
        data.dataValues.clear();
        loadDataFile(fileName, data);
        result.choosenOne = false;
        std::cout<<std::endl<<std::endl<<std::endl<<std::endl;
        Logger::printInfo("File: " + fileName.toStdString() + " - Start processing..");
        long startSingle = clock();
        long startChoosenOne = clock();
        long startGoodOne = clock();
        bool goodResult = false;
        while(!result.choosenOne)
        {
            try
            {
                startSingle = clock();
                qsrand(time(0));
                result = genAlg.process(setup, data);
            }
            catch(...)
            {
                Logger::printDebug("Dividing by 0 removing population.");
                continue;
            }
            if(result.value > 0.000001 && result.value < 0.25 && !goodResult)
            {
                goodResult = true;
                long stopGoodOne = clock();

                Logger::printInfo("GoodOne: " + setup.toString() + " " + result.toString());
                Logger::printInfo("Time:" + std::to_string(stopGoodOne-startGoodOne) + "ms (single: " + std::to_string(stopGoodOne - startSingle) + "ms)");
                writeResult(result, setup.toString() + " ");
            }
            else if(result.choosenOne)
            {
                Logger::printInfo("ChoosenOne: " + setup.toString() + " " + result.toString());
                break;
            }
        }
        long stopChoosenOne = clock();
        Logger::printInfo("Time:" + std::to_string(stopChoosenOne-startChoosenOne) + "ms");
    }

    return a.exec();
}
