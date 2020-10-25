#include "charger_csv.h"

CVString split (const string &, const char &);
pair<unsigned, unsigned> size (const string &);


CVString split (const string &line, const char &sep)
{
     CVString out;
     string value = "";
     unsigned inc(0);
     for (auto c = line.begin(); c != (line.end()); ++c)
     {
        if (*c != sep)
           value.push_back(*c);
        if( *c == sep || (inc == (line.size() - 1)))
        {
            out.push_back(value);
            value.clear();
        }
        ++inc;
     }
     return out;
}

pair<unsigned,unsigned> size (const string & fileName)
{
    unsigned output (0),ncols;
    vector<unsigned> nbCols;
    string line;
    ifstream file (fileName.c_str(), std::ifstream::in);
   // while ( file.good() )
    for (std::string lgne; getline(file, lgne);)
    {
        ncols = 1;
         getline (file, line);
         ++output;
         for (auto &c:line) {
             if (c ==';')
                 ++ncols;
         }
         nbCols.push_back(ncols);
    }
    file.close();
    return pair<unsigned,unsigned>(output,nbCols[0]);
};

void read_csv (CMatString &mat, CVString &vet, const string &fileName)
{
    fileSize _size = size(fileName);
    unsigned nbLines (_size.first), nbCols(_size.second);

    vet.clear();
    vet.reserve(nbCols);

    mat.clear();
    mat.reserve(nbLines);

    ifstream file (fileName.c_str(), std::ifstream::in);

    string header;
    getline(file, header);
    vet = split(header, ';');

    for (std::string lgne; getline(file, lgne);)
       mat.push_back(split(lgne,';'));

    file.close();
}
