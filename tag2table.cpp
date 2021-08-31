#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>
#include "io.h"
using namespace std;
using namespace ptl;

struct t_location
{
  double time;
  double x,y,z;
};

bool operator < (const t_location& a, const t_location& b)
{
  return a.time < b.time;
}

typedef vector<t_location> t_user;
vector<t_user> users;

inline double interpolate(double x1, double y1, double x2, double y2, double x)
{
  return y1 + (y2-y1)*(x-x1)/(x2-x1);
}

void regularize(const t_user& input, t_user& output)
{
  int start = int(input.front().time + 1);
  for (int i = 1, t = start; i < input.size(); i ++)
  {
    if (input[i].time < t) continue;

    assert(input[i].time >= t && input[i-1].time < t);

    for (; t <= input[i].time ; t++)
    {
      t_location pt;
      if (input[i].time - input[i-1].time > 10)
      {
        pt.x = pt.y = pt.z = -1;
      }
      else
      {
        pt.x = interpolate(input[i-1].time, input[i-1].x, input[i].time, input[i].x, t);
        pt.y = interpolate(input[i-1].time, input[i-1].y, input[i].time, input[i].y, t);
        pt.z = interpolate(input[i-1].time, input[i-1].z, input[i].time, input[i].z, t);
      }
      pt.time = t;

      assert(output.size() == t-start);
      output.push_back(pt);
    }
  }
}


vector<int> filemap;

int load_file(const char* name)
{
  vector<vector<double> > data;
  filemap.resize(filemap.size()+1);
  ifstream file(name);
  if (!file.is_open())
  {
    cout << "cannot open file - " << name << endl;
    filemap.back() = -1;
    return -1;
  }
//  else cout << "load - " << name << endl;
  sscan_matrix(name,data);
  if (data.size() < 4)
  {
    cout << "wrong format" << endl;
    filemap.back() = -1;
    return -1;
  }


  t_user user;

  for (int i = 0; i < data[0].size(); i ++)
  {
    t_location t;
    t.time = data[0][i];
    t.x = data[1][i];
    t.y = data[2][i];
    t.z = data[3][i];
    user.push_back(t);
  }
/*
  while (true)
  {
    t_location t;
    file >> t.time;
    if (file.eof()) break;
    file >> t.x >> t.y >> t.z;
    cout << t.x << endl;
    user.push_back(t);
  }
 */
  sort(user.begin(), user.end());
  if (user.back().time - user.front().time < 30)
  {
    cout << "too short - " << name << endl;
    filemap.back() = -1;
    return -2;
  }

  filemap.back() = users.size();
  users.resize(users.size()+1);
  regularize(user, users.back());
  cout << name << '\t' << users.back().back().time - users.back().front().time +1 << ':' << users.back().size() << endl;

  return 0;
}

int main(int argc, char* argv[])
{
  if (argc == 1) return 0;
  string dir = "./";
  if (argc == 3) dir = argv[2];
  ifstream file(argv[1]);
  while (true)
  {
    string name, id;
    file >> name;;
    if (file.eof()) break;
    name = dir + name + ".dat";
    load_file(name.c_str());
  }
  if (users.empty()) return 0;

  int start = users[0].front().time, end = users[0].back().time;
  for (int i = 1; i < users.size(); i ++)
  {
    if (users[i].front().time < start) start = users[i].front().time;
    if (users[i].back().time > end) end = users[i].back().time;
//    cout << users[i].front().time << '\t' << users[i].back().time << endl;
  }
  cout << "all:" << start << " - " << end << " : " << end-start << endl;
  ofstream save("all_xyz.csv"), save1("all_xy.csv");
  for (int t = start; t <= end; t ++)
  {
    save << t;
    save1 << t;
    for (int i = 0; i < filemap.size(); i ++)
    {
      int j = filemap[i];
      if (j >= 0)
      {
        int k = t - users[j].front().time;
        if (k >= 0 && k < users[j].size())
        {
          t_location& pt = users[j][k];

          save << ',' << pt.x << ',' << pt.y << ',' << pt.z;
          save1 << ',' << pt.x << ',' << pt.y;
          continue;
        }
      }
      save << ',' << -1 << ',' << -1 << ',' << -1;
      save1 << ',' << -1 << ',' << -1;

    }
    save << endl;
    save1 << endl;
  }

  return 0;
}
