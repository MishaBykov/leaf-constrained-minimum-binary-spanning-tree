//
// Created by misha on 10.12.18.
//

#include <fstream>
#include <vector>
#include <math.h>

int dist(std::pair<int, int>& v1, std::pair<int, int>& v2)
{
    return abs(v1.first - v2.first) + abs(v1.second - v2.second);
}

int main()
{
    std::fstream in;
    in.open("../input/Taxicab_100.txt");
    int n;
    std::vector<std::pair<int, int>> v;
    std::vector<std::vector<std::pair<int, int>>> sectors_ind_v__cost;

    std::pair<int, int> p_min, p_max, p_center;
//    ... чтение графа ...
    in >> n;
    v.emplace_back();
    in >> v.back().first >> v.back().second;
    p_min.first = v.back().first;
    p_max.first = v.back().first;
    p_min.second = v.back().second;
    p_max.second = v.back().second;

    for(int i = 1; i < n; i++)
    {
        v.emplace_back();
        in >> v.back().first >> v.back().second;

        if(p_min.first > v.back().first)
            p_min.first = v.back().first;

        if(p_max.first < v.back().first)
            p_max.first = v.back().first;

        if(p_min.second > v.back().second)
            p_min.second = v.back().second;

        if(p_max.second < v.back().second)
            p_max.second = v.back().second;

    }
    in.close();

    p_center.first = (p_min.first - p_max.first) / 2;
    p_center.second = (p_min.second - p_max.second) / 2;

    int min_dist = dist(v[0], p_center);
    std::pair<int, int> v_center = v[0];

    for(auto &i : v)
    {
        if(dist(i, p_center) < min_dist)
        {
            min_dist = dist(i, p_center);
            v_center = i;
        }
    }

    int l_max = n / 10, l = 3;
    while(l < l_max)
    {
        l <<= 1;
    }
    l >>= 1;
    sectors_ind_v__cost.resize(static_cast<unsigned long>(l));

    double phi_sector = 2*M_PI/l;

    for(int i = 0; i < n; i++)
    {
        int x = v[i].first - v_center.first, y = v[i].second - v_center.second;
        double phi_v;
        if(y < 0)
            phi_v = std::atan(double(y) / double(x));
        else
            phi_v = std::atan(double(y) / double(x)) + M_PI;
        int ind_sector = static_cast<int>(phi_v / phi_sector);
        sectors_ind_v__cost[ind_sector].push_back(std::pair<int, int>(i, dist(v[i], v_center)));
    }

    return 0;
}