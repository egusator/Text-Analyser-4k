#include "textanalyser.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextAnalyser w;
    w.show();
    return a.exec();
}
