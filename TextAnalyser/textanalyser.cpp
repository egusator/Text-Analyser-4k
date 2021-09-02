#include "textanalyser.h"
#include "qfiledialog.h"
#include "ui_textanalyser.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <QLineEdit>
#include <QMap>
#include <QMessageBox>
#include <QPair>
#include <QRegularExpression>
#include <QTextEdit>
#include <QTextStream>
using namespace std;

TextAnalyser::TextAnalyser(QWidget *parent) : QWidget(parent), ui(new Ui::TextAnalyser)
{
    ui->setupUi(this);
}

struct compareClass1 {
    bool operator()(pair<QString, int> pair1, pair<QString, int> pair2) {
        return pair1.second > pair2.second;
    }
} valueComparator;

struct compareClass2 {
    bool operator()(pair<QString, int> pair1, pair<QString, int> pair2) {
        return pair1.first < pair2.first;
    }
} keyComparator;
vector<pair<QString, int>> VectorForSorting;
vector<pair<QString, int>>::iterator iteratorForSorting;
vector<pair<QString, int>>::iterator prevIter;

QMap<QString, int> amount;

QString TextAnalyser::getInputFileName() {
    QString inputFileName;
    inputFileName = QFileDialog::getOpenFileName(this,
            tr("Open text file for analysis"), "",
            tr("Text File, .txt(*.txt)"));

    if (inputFileName.isEmpty()) {
        QMessageBox errorMessage;
        errorMessage.setText("Error:file-name is empty. Try one more time.");
        errorMessage.exec();
        return "";

    } else {
        ui->inputFileNameLabel->setText(inputFileName);
        return inputFileName;
    }

}

TextAnalyser::~TextAnalyser() {
    delete ui;
}

QString TextAnalyser::getOutputFileName() {

    QString outputFileName;
    outputFileName = QFileDialog::getSaveFileName(this,
            tr("Save analysis data to text file"), "",
            tr("Text File, .txt(*.txt)"));

    if (outputFileName.isEmpty()) {
        QMessageBox errorMessage;
        errorMessage.setText("Error:file-name is empty. Try one more time.");
        errorMessage.exec();
        return "";

    } else {
        ui->outputFileNameLabel->setText(outputFileName);
        return outputFileName;
    }
}

QString inputFileName, outputFileName;

void TextAnalyser::readData(QMap<QString, int> &map, QTextStream *stream)
{
    QMap<QString, int>::iterator Iter;
    QString word;
    while (!(stream->atEnd())) {
        QString line = stream->readLine(11111111); //smth big in limit
        for (int i = 0; i < line.length(); i++) {
            QChar c = line.at(i);
            if (c.isLetter()) {
                word += c;
            } else if (word.length()) {
                word = word.toUpper();
                if ((Iter = map.find(word)) != map.end()) {
                    map[word] += 1;
                } else {
                    map.insert(word, 1);
                }
                word.clear();
            }
        }
        if (word.length()) {
            word = word.toUpper();
            if ((Iter = map.find(word)) != map.end()) {
                map[word] += 1;
            } else {
                map.insert(word, 1);
            }
            word.clear();
        }
    }
}

void TextAnalyser::on_loadButton_clicked()
{
    inputFileName = getInputFileName();
    amount.clear();
    QFile inputFile(inputFileName);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), inputFile.errorString());
        return;
    }
    in = new QTextStream(&inputFile);
    readData(amount, in);
    createUserMessage("The data is loaded. Click the gogogo button");
}

void TextAnalyser::createUserMessage(const QString s)
{
    QMessageBox userMessage;
    userMessage.setText(s);
    userMessage.exec();
    return;
}

void TextAnalyser::on_saveButton_clicked() {
    outputFileName = getOutputFileName();

    QFile outputFile(outputFileName);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), outputFile.errorString());
        return;
    }

    out = new QTextStream(&outputFile);

    for (iteratorForSorting = VectorForSorting.begin();
         iteratorForSorting != VectorForSorting.end();
         iteratorForSorting++)
        *out << iteratorForSorting->first << " " << iteratorForSorting->second << "\n";
    createUserMessage("You can check results in the file you've chosen");
}

void TextAnalyser::on_analysisStartButton_clicked() {
    VectorForSorting.clear();

    if (inputFileName.isEmpty()) {
        createUserMessage("Open files for input");
        return;
    }

    int prevAmount = 0;

    foreach (QString s, amount.keys()) {
        VectorForSorting.push_back(make_pair(s, amount[s]));
    }

    std::sort(VectorForSorting.begin(), VectorForSorting.end(),
            valueComparator);

    for (iteratorForSorting = VectorForSorting.begin();
            iteratorForSorting != VectorForSorting.end();
            iteratorForSorting++) {
        if (prevAmount == 0) {
            prevAmount = iteratorForSorting->second;
            prevIter = iteratorForSorting;
        } else if (iteratorForSorting->second != prevAmount) {
            std::sort(prevIter, iteratorForSorting, keyComparator);
            prevAmount = iteratorForSorting->second;
            prevIter = iteratorForSorting;
        }
    }
    QString str;
    for (iteratorForSorting = VectorForSorting.begin();
         iteratorForSorting != VectorForSorting.end();
         iteratorForSorting++) {
        str += iteratorForSorting->first + " " + QString::number(iteratorForSorting->second) + '\n';
    }

    ui->textEdit->setPlainText(str);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    createUserMessage("Analysis is done. Choose file to save results");
    return;
}
void TextAnalyser::on_findButton_clicked()
{
    if (ui->lineEdit->text().length()) {
        QString searchString = ui->lineEdit->text().toUpper();
        ui->textEdit->find(searchString);
    }
}
