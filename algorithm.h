#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QThread>
#include <QtWidgets>


class Algorithm : public QThread
{
    Q_OBJECT
public:
    Algorithm(int, int, int, std::vector<double>, QObject*);

signals:
    void comparision(int, int);
    void sortDone(int);
    void arrayAccess(int);
    void buttonChange(int);

protected:
    void run() override;

private:
    std::vector<double> columnsHeight;
    int d;
    int speed;
    int amount;
    int sortWith;
    int array;

    void swap(int a, int b);
    void isAccessToArray();
    void Sorted();

    //排序方法
    void BubbleSort();

    int QuickSortPartition(int a, int b);
    void QuickSort(int arrayBegin, int arrayEnd);

    void Heapify(int n, int i);
    void HeapSort();
};

#endif // ALGORITHM_H
