#include "textanalyser.h"
#include "ui_textanalyser.h"
#include "qfiledialog.h"
#include <QMessageBox>
#include <QLineEdit>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <map>
#include <QTextStream>
#include <vector>
#include <QPair>
#include <QRegularExpression>
using namespace std;

TextAnalyser::TextAnalyser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextAnalyser)
{
    ui->setupUi(this);
}

QString inputFileName;
QString outputFileName;

vector<pair<QString, int>> VectorForSorting;
vector<pair<QString, int>>::iterator iteratorForSorting;
vector<pair<QString, int>>::iterator prevIter;

map<QString, int> amount;
map<QString, int>::iterator it;

void TextAnalyser::loadFromFile()
{
   inputFileName = QFileDialog::getOpenFileName(this,
        tr("Open text file for analysis"), "",
        tr("Text File, .txt(*.txt)"));

    if (inputFileName.isEmpty())
        return;
    else {

        QFile file (inputFileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        in = new QTextStream(&file);


        while (!(in->atEnd())) {
               QString line = in->readLine(1000);
               QString word;
               for (int i = 0; i < line.length(); i++) {
                   if(line[i].isLetter())
                       word += line[i];
                   else
                       it = amount.find(word);
                       if (it != amount.end())
                            it->second += 1;
                        else
                            amount.insert(it, make_pair(word, 1));
               }

           }

        struct compareClass1 {
              bool operator() (pair<QString, int> pair1,pair<QString, int> pair2) {
                  return pair1.second > pair2.second;
              }
            } valueComparator;
            struct compareClass2 {
              bool operator() (pair<QString, int> pair1,pair<QString, int> pair2) {
                  return pair1.first < pair2.first;
              }
            } keyComparator;
            int prevAmount = 0;

            for (it = amount.begin(); it != amount.end(); it++) {
                VectorForSorting.push_back(make_pair(it->first, it->second));
            }
            std::sort(VectorForSorting.begin(), VectorForSorting.end(), valueComparator);


        for (iteratorForSorting = VectorForSorting.begin(); iteratorForSorting != VectorForSorting.end(); iteratorForSorting++) {
            if (prevAmount == 0) {
                prevAmount = iteratorForSorting->second;
                prevIter = iteratorForSorting;
            } else if (iteratorForSorting->second != prevAmount) {
                std::sort(prevIter, iteratorForSorting, keyComparator);
                prevAmount = iteratorForSorting->second;
                prevIter=iteratorForSorting;
            }
    }

        return;
    }

}

TextAnalyser::~TextAnalyser()
{
    delete ui;
}




void TextAnalyser::saveToFile()
{
    outputFileName = QFileDialog::getSaveFileName(this,
        tr("Save analysis data to text file"), "",
        tr("Text File, .txt(*.txt)"));

    if (outputFileName.isEmpty())
        return;
    else {
        QFile file(outputFileName);

        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        out = new QTextStream(&file);
        for (iteratorForSorting = VectorForSorting.begin(); iteratorForSorting!=VectorForSorting.end(); iteratorForSorting++)
            *out << iteratorForSorting->first << " " << iteratorForSorting->second;
        return;
    }
}



void TextAnalyser::on_loadButton_clicked()
{
    loadFromFile();
    ui->inputFileNameLabel->setText(inputFileName);
}


void TextAnalyser::on_saveButton_clicked()
{
    saveToFile();
    ui->outputFileNameLabel->setText(outputFileName);
}



void TextAnalyser::on_analysisStartButton_clicked()
{

    QMessageBox userMessage;

    if(inputFileName.isEmpty()) {        if (outputFileName.isEmpty()) {
            userMessage.setText(tr("Open files for input and output!"));
            userMessage.exec();
            return;
        }    else   {
            userMessage.setText(tr("Open files for input!"));
            userMessage.exec();
            return;
        }
}   else   {
        if (outputFileName.isEmpty()) {
            userMessage.setText(tr("Open files for output!"));
            userMessage.exec();
            return;
        }    else   {
            userMessage.setText(tr("You can check data in file you have chosen for output."));
            userMessage.exec();
        }
    }
}





