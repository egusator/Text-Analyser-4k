#ifndef TEXTANALYSER_H
#define TEXTANALYSER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class TextAnalyser; }
QT_END_NAMESPACE

class TextAnalyser : public QWidget
{
    Q_OBJECT

public:
    TextAnalyser(QWidget *parent = nullptr);
    ~TextAnalyser();
    QTextStream *in;
    QTextStream *out;
public slots:
    QString getOutputFileName();
    QString getInputFileName();

private slots:
    void on_loadButton_clicked();

    void on_saveButton_clicked();

    void on_analysisStartButton_clicked();

    void on_findButton_clicked();

    void readData(QMap<QString, int> &, QTextStream *);

    void createUserMessage(QString);

private:
    Ui::TextAnalyser *ui;
};
#endif // TEXTANALYSER_H
