#ifndef CHARGER_CSV_H
#define CHARGER_CSV_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <algorithm>

using namespace std;

// types pour la gesstion du .csv
typedef vector<string> CVString;
typedef vector <vector<string>> CMatString;
typedef pair<unsigned, unsigned> fileSize;

// fonction chargeant le .csv en memoire
void read_csv (CMatString&, CVString&, const string&);

#endif
