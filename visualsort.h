#ifndef VISUALSORT_H
#define VISUALSORT_H

#include <QWidget>
#include <QtWidgets>
#include <algorithm.h>

namespace Ui {
class visualsort;
}

class Algorithm;

class visualsort : public QWidget
{
    Q_OBJECT

public:
    explicit visualsort(QWidget *parent = nullptr);
    ~visualsort();

private slots:
    void on_SortButton_clicked();
    void on_SortList_itemClicked(QListWidgetItem *item);
    void on_Amount_valueChanged(int);
    void on_Speed_valueChanged(int);

    void on_comparision(int, int);
    void ButtonStatus(int);
    void sortDone(int);
    void resetColumns(int,int);

private:
    Ui::visualsort *ui;
    Algorithm *algorithm;
    QGraphicsScene *scene;

    void columsSetup(int ,int);
    void Update(int ,int);

    std::vector<QGraphicsRectItem*> columns;
    std::vector<double> columnsHeight;
    QMap<int,QString> AlgorithmList;

    int speed;
    int amounts;

    double columnsWidth;
    double sceneHeight;
    double sceneWidth;

    //建立排序算法的关键
    int key;
    //当appStatus为 -1 时，程序开始
    //当appStatus为 0 时，准备排序
    //当appStatus为 1 时，排序开始
    //当appStatus为 2 时，排序结束
    int appStatus;

};

#endif // VISUALSORT_H
