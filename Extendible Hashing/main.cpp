#include "directory.h"
#include <iostream>

using namespace std;

int main()
{
    Directory dir;
    DataItem d;
    d.setData(15);

    //1
    d.setKey(5);
    dir.insertItem(d);
    dir.print();
    //2
    d.setKey(4);
    dir.insertItem(d);
    dir.print();
    //3
    d.setKey(6);
    dir.insertItem(d);
    dir.print();
    //4
    d.setKey(40);
    dir.insertItem(d);
    dir.print();
    //5
    d.setKey(3);
    dir.insertItem(d);
    dir.print();
    //6
    d.setKey(50);
    dir.insertItem(d);
    dir.print();
    //7
    d.setKey(60);
    dir.insertItem(d);
    dir.print();
    
}
