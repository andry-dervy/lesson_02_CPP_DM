//============================================================================
// Name        : lesson_02_CPP_DM.cpp
// Author      : andry antonenko
// IDE         : Qt Creator 4.14.2 based on Qt 5.15.2
// Description : lesson 02 of the C++: difficult moments course
//============================================================================
#include <QCoreApplication>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <tuple>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <memory>
#include <codecvt>

#include "timer.h"

using namespace std;
//----------------------------------------------------------------------------
int getRandomNum(int min, int max)
{
  const static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::mt19937_64 generator(seed);
  std::uniform_int_distribution<int> dis(min, max);
  return dis(generator);
}
//----------------------------------------------------------------------------
/*
1. Реализуйте шаблонную функцию Swap,
которая принимает два указателя и обменивает местами значения,
на которые указывают эти указатели
(нужно обменивать именно сами указатели,
переменные должны оставаться в тех же адресах памяти).
//*/

template<typename T>
void Swap(shared_ptr<T> &pFirst, shared_ptr<T> &pSecond)
{
  shared_ptr<T> temp = pSecond;
  pSecond = pFirst;
  pFirst = temp;
}

void task_1()
{
  cout << "Task 1\n" << endl;

  shared_ptr<int> pFirst = make_shared<int>(10);
  shared_ptr<int> pSecond = make_shared<int>(20);

  cout << "First value = " << *pFirst << endl;
  cout << "Second value = " << *pSecond << endl;

  cout << "Swap(First, Second)" << endl;
  Swap(pFirst,pSecond);

  cout << "First value = " << *pFirst << endl;
  cout << "Second value = " << *pSecond << endl;
}

//----------------------------------------------------------------------------
/*
2. Реализуйте шаблонную функцию SortPointers,
которая принимает вектор указателей и
сортирует указатели по значениям, на которые они указывают.
//*/

template<typename T>
void SortPointers(vector<shared_ptr<T>> &v)
{
  size_t size = v.size();
//  Timer timer("Bubble sort");
  for (size_t i = 0; i < size - 1; i++)
    for (size_t j = 0; j < size - i - 1; j++)
      if (*v[j] > *v[j + 1])
        Swap(v[j],v[j + 1]);
//  timer.print();
}

template<typename T>
void printVector(const vector<shared_ptr<T>> &v)
{
  for(auto a: v)
    cout << *a << " ";
  cout << endl;
}

void task_2()
{
  cout << "\nTask 2\n" << endl;

  const size_t SZ = 10;
  const int MIN = 0;
  const int MAX = 100;
  vector<shared_ptr<int>> Massive(SZ);

  for(size_t i = 0; i < SZ; ++i)
  {
    Massive[i] = make_shared<int>(getRandomNum(MIN,MAX));
  }
  printVector(Massive);
  SortPointers(Massive);
  printVector(Massive);
}

//----------------------------------------------------------------------------
/*
3. Подсчитайте количество гласных букв в книге “Война и мир”.
Для подсчета используйте 4 способа:
- count_if и find
- count_if и цикл for
- цикл for и find
- 2 цикла for
Замерьте время каждого способа подсчета и сделайте выводы.
//*/

const string VOWEL_LETTERS = "АаЕеЁёИиЙйОоУуЫыЭэЮюЯя";

//count_if и find
size_t countOfLetters_CountIfAndFind(shared_ptr<u16string> ptr)
{
  wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t> convert;
  const u16string VL = convert.from_bytes(VOWEL_LETTERS);

  Timer timer("count_if и find");
  size_t cnt = count_if(ptr->begin(),ptr->end(),[&VL](char16_t ch)
  {
    if(VL.find(ch) != string::npos)
      return true;
    return false;
  });
  timer.print();
  return cnt;
}

//count_if и цикл for
size_t countOfLetters_CountIfAndOneFor(shared_ptr<u16string> ptr)
{
  wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t> convert;
  const u16string VL = convert.from_bytes(VOWEL_LETTERS);

  Timer timer("count_if и цикл for");
  size_t cnt = count_if(ptr->begin(),ptr->end(),[&VL](char16_t ch)
  {
    for(size_t i = 0; i < VL.length(); ++i)
      if(VL[i] == ch)
        return true;
    return false;
  });
  timer.print();
  return cnt;
}

//цикл for и find
size_t countOfLetters_OneForAndFind(shared_ptr<u16string> ptr)
{
  wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t> convert;
  const u16string VL = convert.from_bytes(VOWEL_LETTERS);

  Timer timer("цикл for и find");
  size_t cnt = 0;
  for(char16_t ch: *ptr)
  {
    if(VL.find(ch) != string::npos)
      ++cnt;
  }
  timer.print();
  return cnt;
}

//2 цикла for
size_t countOfLetters_TwoFor(shared_ptr<u16string> ptr)
{
  wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t> convert;
  const u16string VL = convert.from_bytes(VOWEL_LETTERS);

  Timer timer("2 цикла for");
  size_t cnt = 0;
  for(char16_t ch: *ptr)
  {
    for(size_t i = 0; i < VL.length(); ++i)
      if(VL[i] == ch)
      {
        ++cnt;
        break;
      }
  }
  timer.print();
  return cnt;
}

shared_ptr<u16string> getText(ifstream &inFile)
{
  shared_ptr<u16string> pU16Text = make_shared<u16string>();
  wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t> convert;

  if (inFile.is_open())
  {
    string word;
    u16string dest;

    while (!inFile.eof())
    {
      inFile >> word;
      *pU16Text += convert.from_bytes(word + " ");
    }
    inFile.close();
  }
  else
  {
    cout << "File did not open!" << endl;
  }
  return pU16Text;
}

void task_3()
{
  cout << "\nTask 3\n" << endl;

  ifstream fin("Tolstoi_Lev__Voina_i_mir._Tom_1.txt"); // путь к файлу txt

  shared_ptr<u16string> pTxt = getText(fin);

  if(pTxt == nullptr)
  {
    cout << "The pointer is not validate." << endl;
    return;
  }

  int cnt;
  cnt = 10;
  while(--cnt)
    cout << "Amount of vowels: " << countOfLetters_CountIfAndFind(pTxt) << endl;
  cnt = 10;
  while(--cnt)
    cout << "Amount of vowels: " << countOfLetters_CountIfAndOneFor(pTxt) << endl;
  cnt = 10;
  while(--cnt)
    cout << "Amount of vowels: " << countOfLetters_OneForAndFind(pTxt) << endl;
  cnt = 10;
  while(--cnt)
    cout << "Amount of vowels: " << countOfLetters_TwoFor(pTxt) << endl;

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  //--------------------------------------------------------------------------
  // Task 1
  /*
  task_1();
  //*/
  //--------------------------------------------------------------------------
  // Task 2
  /*
  task_2();
  //*/
  //--------------------------------------------------------------------------
  // Task 3
  //*
  task_3();
  //*/
  //--------------------------------------------------------------------------
  return a.exec();
}
