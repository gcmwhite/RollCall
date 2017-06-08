#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void importData();
private:
    QLabel *nameDisplayLabel;
    QPushButton *switchButton;
    QPushButton *importButton;
    QGridLayout *mainLayout;
    QTimer *switchTime;
    QStringList strListImport;
    int count = 0;
    bool switchButtonFlag = false;
private slots:
    void importButtonClicked();
    void switchButtonClicked();
    void switchTimeOut();
};

#endif // WIDGET_H
