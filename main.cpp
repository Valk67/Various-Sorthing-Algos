
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <ctime>
using namespace std;

//function declairations are in the same order as descriptions below
void quicksort (const string &s, vector<int> &indices, int low, int high);

int partition (const string &s, vector<int> &indices, int low,
                int high, int pivotIndex);

void insertion (const string &s, vector<int> &indices, int low, int high);

int selection (const string &s, vector<int> &indices, int low, int high, int k);

bool lessThan (const string &s, int &first, int &second);

char convertToLower (char ch);

bool readFromFile(string &s, string filename, vector<string> &strv, int x);

//main
int main(int argc, char *argv[]) {

  string filename1 = argv[1];
  string s;
  vector<int> indices;
  vector<string> strv;
  vector<int> A;
  vector<int> B;
  if (!readFromFile (s, filename1, strv, 1)) {
    return -1;
  }
  for (int i = 0; i < s.size(); i++) {
    A.push_back(i);
    B.push_back(i);
  }
  clock_t t1 =0, t2 = 0;
  double elapsed_q = 0.0, elapsed_i = 0.0, elapsed_s = 0.0;
  switch (argc) {
    //experiment 1
    case 2: {
      t1 = clock();
      quicksort (s, A, 0, s.size() - 1);
      t2 = clock();
      elapsed_q = double (t2 - t1) / CLOCKS_PER_SEC;

      t1 = clock();
      insertion (s, B, 0, s.size() - 1);
      t2 = clock();
      elapsed_i = double (t2 - t1) / CLOCKS_PER_SEC;

      cout << "length of S = " << s.size() << "," << " elapsed_q = " << elapsed_q
           << "," << " elapsed_i = " << elapsed_i << endl;
      break;
    }
    //experiment 2
    case 3: {
      t1 = clock();
      quicksort (s, A, 0, s.size() - 1);
      t2 = clock();
      elapsed_q = double (t2 - t1) / CLOCKS_PER_SEC;

      t1 = clock();
      int answer = selection (s, B, 0, s.size() - 1, atoi(argv[2]));
      t2 = clock();
      elapsed_s = double (t2 - t1) / CLOCKS_PER_SEC;

      cout << "length of S = " << s.size() << "," << " elapsed_q = " << elapsed_q
           << "," << " elapsed_s = " << elapsed_s << endl;
      answer = answer + 0; //used to get rid of annoying warning
      break;
    }
    //experiment 3
    case 4: {
      t1 = clock();
      insertion (s, A, 0, s.size() - 1);
      t2 = clock();
      elapsed_i = double (t2 - t1) / CLOCKS_PER_SEC;

      vector<int> answer (s.size());
      t1 = clock();
      for (int i = 0; i < s.size(); i++) {
        answer[i] = selection (s, B, 0, s.size() - 1, i);
      }
      t2 = clock();
      elapsed_s = double (t2 - t1) / CLOCKS_PER_SEC;

      cout << "length of S = " << s.size() << "," << " elapsed_i = " << elapsed_i
           << "," << " elapsed_s = " << elapsed_s << endl;
      break;
    }
    default:
      cout << "invalid command" << endl;
      break;
  }
  //for loops used to print out A and B to double check if algorithms produced
  //proper output.
  /*for (int i = 0; i < A.size(); i++) {
      cout << A[i] << " ";} cout << endl;
    for (int i = 0; i < B.size(); i++) {
      cout << B[i] << " ";} cout << endl;*/
return 0;
}
  

void quicksort (const string &s, vector<int> &indices, int low, int high) {
  if (low < high) {
    int pivotIndex = partition(s, indices, low, high, high);
    quicksort (s, indices, low, pivotIndex - 1);
    quicksort (s, indices, pivotIndex + 1, high);
  }
}

int partition (const string &s, vector<int> &indices, int low,
                int high, int pivotIndex) {
  int tmp = indices[high];
  indices[high] = indices[pivotIndex];
  indices[pivotIndex] = tmp;
  int i = low, j = high - 1;
  while (i <= j) {
    while (i < high && lessThan(s, indices[i], indices[high])) {
      i++;
    }
    while (j >= low && !lessThan(s, indices[j], indices[high])) {
      j--;
    }
    if (i < j) {
      tmp = indices[i];
      indices[i] = indices[j];
      indices[j] = tmp;
      i++;
      j--;
    }
  }
  tmp = indices[i];
  indices[i] = indices[high];
  indices[high] = tmp;
  return i;
}

void insertion (const string &s, vector<int> &indices, int low, int high) {
  for (int i = low; i <= high; i++) {
    int key = indices[i];
    int j = i - 1;

    while (j >= low && !lessThan(s, indices[j], key)) {
      indices[j+1] = indices[j];
      j--;
    }
    indices[j+1] = key;
  }
}

int selection (const string &s, vector<int> &indices, int low, int high, int k) {
  int vsize = high - low + 1;

  if (low == high)
    return indices[low];
  else if (low > high)
    return -1;

  if (vsize <= 5) {
    insertion (s, indices, low, high);
    return indices[k];
  }
  int msize = ceil((double)vsize/5);
  vector<int> medians(msize, 0);
  int med_ind = 0;
  for (int i = low; i <= high; i = i + 5) {
    vector<int> temp(5, 0);
    int j = 0;
    for (; j < 5 && (i + j) <= high; j++) {
      temp[j] = indices[i+j];
    } //for j
    if (j < 5)
      temp.resize(j);
    insertion (s, temp, 0, temp.size() - 1); //changed this
    if (j == 5)
      medians[med_ind++] = temp[2];
    else {
      int jmid = j >> 1; //divide by 2
      if (j % 2 == 0)
        jmid--;
      medians[med_ind++] = temp[jmid];
    }
  }
  //so far we selected the medians of n/5 groups, each group of 5 elements
  int mid = msize >> 1; //divide by 2
  if (msize % 2 == 0)
    mid--;
  //find the median of the medians
  int x = selection (s, medians, 0, msize - 1, mid);
  if (x == - 1) {
    cerr << "ERROR: returned value of median of medians is -1." << endl;
    exit(0);
  }
  //partition elements of v around x
  //find the index of x in v
  int ind_of_x = -1;
  for (int i = low; i <= high; i++) {
    if (indices[i] == x) {
      ind_of_x = i;
      break;
    }
  }
  int pivotIndex = partition (s, indices, low, high, ind_of_x);
  if (k == pivotIndex)
    return indices[k];
    else if (pivotIndex < k)
      return selection (s, indices, pivotIndex + 1, high, k);
    else
      return selection (s, indices, low, pivotIndex - 1, k);
}

//function that compares two suffixes (words) letter by letter
bool lessThan (const string &s, int &first, int &second) {
  int size = s.size() - first;
  int i = 0;
  char c1, c2;
  while (i < size) {
    c1 = convertToLower(s[first + i]);
    c2 = convertToLower(s[second + i]);
    if (c1 == c2)
      i++;
    else
      size = -1;
  }
  if (i == size)
    return true;

  else if (c1 < c2)
    return true;

return false;
}

//converts uppercases to lower case
char convertToLower (char ch) {
  if (ch >= 'A' && ch <= 'Z') {
    return (char)(ch - 'A' + 'a');
  }
return ch;
}

//reads from file input and creates a string string of concatinated words
//or a vector of strings depending on which x value is passed.
bool readFromFile(string &s, string filename, vector<string> &strv, int x) {
  ifstream inf;
  inf.open(filename);
  if(!inf) {
    cerr << "Error could not open " << filename << endl;
    return false;
  }
  string text;
  if (x == 1) {
    getline(inf, s);
      while(!inf.eof()) {
        getline(inf, text);
        s = s + text;
      }
  } else if (x == 2) {
      while (inf >> text) {
        strv.push_back (text);
      }
  }
  inf.close();
  return true;
}

//end
