// Grid Problem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Grid.h"

using namespace std;

int main(int argc, char* argv[])
{
    //string strFilePath = "C:/Users/Admin/Documents/Coding Related/Projects/Grid Problem/grid_input2.txt";
    
    if (argc > 1)
    {
        string strFilePath(argv[1]);
        Grid grid;

        grid.addPoints(strFilePath);
    }
    else
    {
        cout << "Please pass a filename as an argument to the executable!\n";
    }
}