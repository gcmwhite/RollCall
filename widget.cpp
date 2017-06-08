#include "widget.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTime>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle(tr("随机点名器v1.0"));      //设置窗口名
    this->setFixedSize(230,120);        //设置窗口固定大小

    QTime time = QTime::currentTime();  //获得当前时间
    qsrand(time.msec()+time.second()*10000);    //用时间初始化随机数种子

    mainLayout = new QGridLayout(this);       //创建格子布局

    nameDisplayLabel = new QLabel(tr("随机点名器"));       //新建QLabel
    nameDisplayLabel->setAlignment(Qt::AlignCenter);    //QLabel控件居中显示
    nameDisplayLabel->setFont(QFont("华文行楷",30));        //设置字体格式
    switchButton = new QPushButton(tr("开始"));       //新建开关按钮
    importButton = new QPushButton(tr("导入"));       //新建导入按钮

    switchTime = new QTimer;        //新建一个定时器
//    switchTime->setInterval(500);   //设置定时器500ms

    //添加nameDisplayLabel控件到mainLayout的第0行0列，并占用1行2列
    mainLayout->addWidget(nameDisplayLabel,0,0,1,2);
    //添加importButton控件到mainLayout的第1行0列
    mainLayout->addWidget(importButton,1,0);
    //添加switchButton控件到mainLayout的第1行1列
    mainLayout->addWidget(switchButton,1,1);

    //将“导入”按钮链接到importButtonClicked()槽函数
    this->connect(importButton,SIGNAL(clicked(bool)),this,SLOT(importButtonClicked()));
    //将“开关”按钮链接到switchButtonClicked()槽函数
    this->connect(switchButton,SIGNAL(clicked(bool)),this,SLOT(switchButtonClicked()));
    //将定时器链接到switchTimeOut()槽函数
    this->connect(switchTime,SIGNAL(timeout()),this,SLOT(switchTimeOut()));
}

Widget::~Widget()
{

}
void Widget::importButtonClicked()
{
    if (strListImport.isEmpty())        //数据为空则导入，否则给出提示信息
    {
        importData();       //数据导入函数
    }
    else
    {
        //提示信息
        switch(QMessageBox::question(this,tr("提示："),
                              tr("本次导入会覆盖上一次数据，是否继续？"),
                              QMessageBox::No|QMessageBox::No,QMessageBox::Yes))
        {
            case QMessageBox::Yes:
                strListImport.clear();  //选择Yes则清空数据表，计数器清0，再导入数据信息
                count = 0;
                importData();
                break;
            default:    //默认不进行任何操作
                break;
        }
    }
}
void Widget::switchButtonClicked()
{
    if (count == 0)     //若计数器为0，给出提示信息
    {
        QMessageBox::information(this,tr("提示："),tr("您还没有导入数据！"));
    }
    else
    {
        //开关按钮标志位，每按一次取反一次
        switchButtonFlag = !switchButtonFlag;
        if (switchButtonFlag)
        {
            switchButton->setText(tr("停止"));
            switchTime->start(50);
        }
        else
        {
            switchButton->setText(tr("开始"));
            switchTime->stop();
        }
    }
}
void Widget::switchTimeOut()    //定时器溢出处理函数
{
    int test = qrand()%count;   //初始化随机数
    nameDisplayLabel->setText(strListImport.at(test));
    qDebug() << test;
}
void Widget::importData()       //数据导入函数
{
    //获取文件路径
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        //以只读方式打开文本信息
        if (file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream textStream(&file);
            while(!textStream.atEnd())
            {
                //一行一行的读取数据
                QString temp = textStream.readLine();
                if (!temp.isEmpty())    //数据吧不为空则加入QStringList表中，且计数器加1
                {
                    strListImport.append(temp);
                    count++;
                }
            }
            QMessageBox::information(this,tr("提示："),tr("您已成功导入%1个名字！").arg(count));
        }
    }
    else
    {
        QMessageBox::information(this,tr("提示："),tr("导入失败，请检查格式是否正确！"));
    }
}
