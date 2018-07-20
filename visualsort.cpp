#include "visualsort.h"
#include "ui_visualsort.h"

#include <random>
#include <algorithm>

visualsort::visualsort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::visualsort)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->background->setScene(scene);

    appStatus = -1;

    sceneHeight = ui->background->size().height();
    sceneWidth = ui->background->size().width();

    for(int i = 0;i < ui->SortList->count();i++)
    {
        QListWidgetItem *item = ui->SortList->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(),100));
        AlgorithmList.insert(i,item->text());
    }
    //setDisabled是启用属性的一个方便函数，对于信号——槽连接非常有用。
    //如果b为true，动作被禁用;否则它是启用的。
    ui->Amount->setDisabled(true);
    ui->Speed->setDisabled(true);
    ui->SortButton->setDisabled(true);
    //初始化排序速度 = 1 ms和个数 = 100
    columsSetup(1,100);
}

void visualsort::columsSetup(int s, int n)
{
    speed = s;
    amounts = n;
    columnsWidth = sceneWidth/amounts;
    columns.resize(static_cast<unsigned>(amounts));
    double unit = sceneHeight/amounts;
    for(auto i = unit;i <= sceneHeight; i += unit)
    {
        columnsHeight.push_back(i);
    }
    //生成随机数组设置columnsHeight
    std::random_device m_random;
    std::mt19937 e_random(m_random());
    std::shuffle(columnsHeight.begin(),columnsHeight.end(),e_random);

    auto j = 0;
    auto k = 0.0;
    for(auto &p : columns)
    {
        p = new QGraphicsRectItem;
        p->setRect(k, (sceneHeight - columnsHeight[j]), columnsWidth , columnsHeight[j]);
        p->setBrush(QBrush(QColor(150, 150 , 150, 150)));

        if(amounts <= 100)
            p->setPen(QPen(Qt::red));
        else if(amounts <= 200 && amounts >100)
            p->setPen(QPen(Qt::yellow));
        else if(amounts <= 300 && amounts >200)
            p->setPen(QPen(Qt::green));
        else
            p->setPen(QPen(Qt::blue));
        scene->addItem(p);
        j++;
        k += columnsWidth;
    }
}

void visualsort::resetColumns(int s, int n)
{
    for(auto &p : columns)
        scene->removeItem(p);
    columnsHeight.clear();
    columsSetup(s,n);
    Update(speed,key);
}

void visualsort::Update(int s, int m)
{
    algorithm = new Algorithm(s,m,static_cast<int>(amounts),columnsHeight,this);
    connect(algorithm,SIGNAL(comparision(int,int)),this,SLOT(on_comparision(int,int)));
    connect(algorithm,SIGNAL(sortDone(int)),this,SLOT(sortDone(int)));
    connect(algorithm,SIGNAL(buttonChange(int)),this,SLOT(ButtonStatus(int)));
}
void visualsort::on_SortList_itemClicked(QListWidgetItem *item)
{
    if(appStatus == -1)
    {
        ui->SortButton->setEnabled(true);
        ui->Amount->setEnabled(true);
        ui->Speed->setEnabled(true);
        appStatus = 0;
        ButtonStatus(appStatus);
    }
    if(appStatus == 0)
    {
        key = AlgorithmList.key(item->text());
        ui->Sort->setText(item->text());
    }
}

void visualsort::on_Amount_valueChanged(int m)
{
    if(appStatus == 0)
        resetColumns(speed,m);
}

void visualsort::on_Speed_valueChanged(int s)
{
    if(appStatus == 0)
        resetColumns(s,static_cast<int>(amounts));
}


void visualsort::on_SortButton_clicked()
{
    switch(appStatus){
    case 0:
        ButtonStatus(1);
        Update(speed,key);
        algorithm->start();
        break;
    case 1:
        algorithm->terminate();
        ButtonStatus(2);
        break;
    case 2:
        for(auto &p : columns)
            scene->removeItem(p);
        columnsHeight.clear();
        columsSetup(speed,static_cast<int>(amounts));
        ButtonStatus(0);
        break;
    default:
        break;
    }
}

void visualsort::ButtonStatus(int a)
{
    QString buttonText;
    QString color;
    switch(a){
    case 0:
        buttonText = "排 序";
        ui->Amount->setEnabled(true);
        ui->Speed->setEnabled(true);
        ui->SortList->setEnabled(true);
        color = "background-color: rgba(100, 100, 100, 100); color: #fff";
        break;
    case 1:
        buttonText = "取 消";
        ui->Amount->setDisabled(true);
        ui->Speed->setDisabled(true);
        ui->SortList->setDisabled(true);
        color = "background-color: rgba( 100,100, 100, 100); color: #fff";
        break;
    case 2:
        buttonText = "选 择";
        ui->SortButton->setEnabled(true);
        color = "background-color: rgba( 100, 100,100, 100); color: #fff";
        break;
    default:
        break;
    }
    appStatus = a;
    ui->SortButton->setText(buttonText);
    ui->SortButton->setStyleSheet(color);
}


void visualsort::on_comparision(int n, int m)
{
    auto nRect = columns[n]->rect();
    auto mRect = columns[m]->rect();
    auto nColumn = nRect.left();
    auto mColumn = mRect.left();
    nRect.moveLeft(mColumn);
    mRect.moveLeft(nColumn);
    columns[n]->setRect(nRect);
    columns[m]->setRect(mRect);
    std::swap(columns[n],columns[m]);
}

void visualsort::sortDone(int a)
{
    columns[a]->setBrush(QBrush(QColor(0,0,0,0)));
}

visualsort::~visualsort()
{
    algorithm->terminate();
    delete ui;
}

