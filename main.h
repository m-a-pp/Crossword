#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <locale.h>
#include <commctrl.h>
#include <atlstr.h>
#include <time.h>

using namespace std;

char** generate(int size, char* c_path, int answer);
int ShowWindow();
int main();