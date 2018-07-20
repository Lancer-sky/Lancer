#include "algorithm.h"

Algorithm::Algorithm(int s, int m, int n, std::vector<double> c, QObject *parent = nullptr)
    : QThread(parent)
{
    sortWith = m;
    amount = n;
    columnsHeight = c;
    array = 0;
    d = (amount >= 300 ? 1 : 5);
    speed = s;
}

void Algorithm::run()
{
    switch(sortWith)
    {
        case 0:
            BubbleSort();
            break;
        case 1:
            QuickSort(0, amount-1);
            break;
        case 2:
            HeapSort();
            break;
    }
    Sorted();
}

void Algorithm::Sorted()
{
    for(auto i=0; i<amount; i++)
    {
        //emit把connect把信号和槽进行关联
        //emit某个信号，就相当于调用这个信号connect时所关联的槽函数
        emit sortDone(i);
        msleep(d);
    }
    emit buttonChange(2);
}

void Algorithm::swap(int a, int b)
{
    std::swap(columnsHeight[a], columnsHeight[b]);
    emit comparision(a, b);
}

void Algorithm::isAccessToArray()
{
    array++;
    emit arrayAccess(array);
}

//冒泡排序
void Algorithm::BubbleSort()
{
    for(auto i = 0; i < amount; i++)
            for(auto j = 0; j< amount-1; j++)
            {
                if(columnsHeight[j] > columnsHeight[j+1])
                    swap(j, j+1);
                isAccessToArray();
                msleep(speed);
            }
}

//快速排序
int Algorithm::QuickSortPartition(int a, int b)
{
    auto p = columnsHeight[b];
    auto i = (a-1);

    for (auto j = a; j <= b-1; j++)
    {
        if (columnsHeight[j] <= p)
        {
            i++;
            swap(i, j);
        }
        isAccessToArray();
        msleep(speed);
    }
    swap(i+1, b);
    msleep(speed);
    return (i+1);
}

void Algorithm::QuickSort(int arrayBegin, int arrayEnd)
{
    if (arrayBegin < arrayEnd)
    {
        auto p = QuickSortPartition(arrayBegin, arrayEnd);

        QuickSort(arrayBegin, p - 1);
        QuickSort(p + 1, arrayEnd);
    }
}

//堆排序
void Algorithm::Heapify(int n, int i)
{
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;

    if (l < n && columnsHeight[l] > columnsHeight[largest])
    {
        largest = l;
    }
    isAccessToArray();

    if (r < n && columnsHeight[r] > columnsHeight[largest])
    {
        largest = r;
    }
    isAccessToArray();

    if (largest != i)
    {
        swap(i, largest);
        Heapify(n, largest);
        msleep(speed);
    }
}

void Algorithm::HeapSort()
{
    auto n = amount;

    for (int i = n/2 - 1; i >= 0; i--)
    {
        Heapify(n, i);
    }
    for (int i = n-1; i >= 0; i--)
    {
        swap(0, i);
        msleep(speed);
        Heapify(i, 0);
    }
}
