/* #include <bits/stdc++.h>
#include "header_files/scope_table.h"
#include "header_files/symbol_table.h"

#define show(v)                \
  trav(i, v) fout << i << " "; \
  fout << "\n";
#define trav(i, v) for (auto &i : v)

#define watch(x) cerr << #x << " = " << x << "\n"
using namespace std;

void tokenize(std::string const &str, const char delim, std::string *out, int &cnt)
{
  // construct a stream from the string
  std::stringstream ss(str);
  // std::vector<string> out;

  std::string s;
  while (std::getline(ss, s, delim))
  {
    if (!s.empty())
    {
      out[cnt++] = s;
    }
  }
}

int main()
{
  ifstream fin("input.txt");
  ofstream fout("output.txt");
  int bucketSize;
  fin >> bucketSize;
  SymbolTable st(bucketSize, fout);
  int i = 0;
  string inputLine;
  getline(fin, inputLine);
  // cout<<inputLine<<endl;
  while (true)
  {
    i++;
    getline(fin, inputLine);
    char delim = ' ';
    int cnt = 0;
    string token[100];

    tokenize(inputLine, delim, token, cnt);
    fout << "Cmd " << i << ": ";
    show(token);
    if (token[0] == "I")
    {

      if (cnt != 3)
        fout << "   Wrong number of arugments for the command I" << endl;
      else
      {

        st.insert(token[1], token[2], fout);
      }
    }

    else if (token[0] == "L")
    {
      // fout<<cnt<<endl;

      if (cnt != 2)
        fout << "       Wrong number of arugments for the command L" << endl;
      else
      {

        bool ans = st.lookup(token[1], fout);
        if (!ans)
          fout << "       '" << token[1] << "' not found in any of the ScopeTables" << endl;
      }
    }

    else if (token[0] == "P")
    {

      if (cnt != 2)
        fout << "       Wrong number of arugments for the command P" << endl;

      if (token[1] == "C")
        st.printCurrentScope(fout);
      else if (token[1] == "A")
        st.printAllScope(fout);
      else
        fout << "       Invalid argument for the command P" << endl;
    }
    else if (token[0] == "D")
    {

      if (cnt != 2)
        fout << "       Wrong number of arugments for the command D" << endl;
      else
      {

        fout << st.remove(token[1]) << endl;
      }
    }
    else if (token[0] == "S")
    {

      if (cnt != 1)
        fout << "       Wrong number of arugments for the command S" << endl;
      else
      {

        st.createScope(bucketSize, fout);
      }
    }
    else if (token[0] == "E")
    {

      if (cnt != 1)
        fout << "       Wrong number of arugments for the command E" << endl;
      else
      {

        st.removeScope(fout);
      }
    }
    else if (token[0] == "Q")
    {

      if (cnt != 1)
        fout << "      Wrong number of arugments for the command Q" << endl;
      else
      {

        st.removeAll(fout);
        break;
      }
    }
  }
} */