//
// Created by misha on 10.12.18.
//

#include <fstream>
#include <vector>
#include <cmath>

int dist(std::pair<int, int>& v1, std::pair<int, int>& v2)
{
    return abs(v1.first - v2.first) + abs(v1.second - v2.second);
}

int main()
{
    std::fstream in;
    in.open("../input/input.txt");
    int n;
    std::vector<std::pair<int, int>> v;
    std::vector<std::vector<std::pair<int, int>>> sectors_ind_v__dist;

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
    int ind_v_center = 0;

    for(int i = 1; i < v.size(); i++)
    {
        if(dist(v[i], p_center) < min_dist)
        {
            min_dist = dist(v[i], p_center);
            ind_v_center = i;
        }
    }

    int l_max = n / 10, l = 3;
    while(l < l_max)
    {
        l <<= 1;
    }
    l >>= 1;
    sectors_ind_v__dist.resize(static_cast<unsigned long>(l));

    double phi_sector = 2*M_PI/l;

    for(int i = 0; i < n; i++)
    {
        if(i == ind_v_center)
            continue;
        double phi_v;
        int x = v[i].first - v[ind_v_center].first, y = v[i].second - v[ind_v_center].second;
        if(x == 0)
        {
            if(y > 0)
                phi_v = M_PI/2;
            else
                phi_v = 3*M_PI/2;
        }
        else
            if(x < 0) {
                phi_v = std::atan(double(y) / double(x)) + M_PI;
            }
            else {
                phi_v = std::atan(double(y) / double(x));
            }
        if(phi_v < 0)
        {
            phi_v += M_PI*2;
        }
        auto ind_sector = static_cast<int>(phi_v / phi_sector);
        sectors_ind_v__dist[ind_sector].push_back(std::pair<int, int>(i, dist(v[i], v[ind_v_center])));
    }



    return 0;
}