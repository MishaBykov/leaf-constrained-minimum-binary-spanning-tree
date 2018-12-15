//
// Created by misha on 10.12.18.
//

#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

int cost = 0;
std::vector<std::pair<int, int>> v;
/**
 * pair<dist, ind_v>
 */
std::vector<std::vector<std::pair<int, int>>> sectors;
/**
 pair<cost, pair<ind_v1, ind_v2>
 */
std::vector<std::pair<int, std::pair<int, int>>> ribs;

int dist(std::pair<int, int>& v1, std::pair<int, int>& v2)
{
    return abs(v1.first - v2.first) + abs(v1.second - v2.second);
}

void add_rib(int& ind_v1, int& ind_v2)
{
    ribs.emplace_back();
    ribs.back().second.first = ind_v1;
    ribs.back().second.second = ind_v2;
    ribs.back().first = dist(v[ind_v1], v[ind_v2]);
    cost += ribs.back().first;
}

int move_last_v_from_sector(int init_ind_v, int ind_sector)
{
    int result = sectors[ind_sector].back().second;
    add_rib(init_ind_v, sectors[ind_sector].back().second);
    sectors[ind_sector].pop_back();
    return result;
}

void f(int init_ind_v, int ind_begin_sector, int ind_end_sector) {
    if (ind_begin_sector == ind_end_sector)
    {
        int pred_ind = init_ind_v;
        while(!sectors[ind_begin_sector].empty())
        {
            pred_ind = move_last_v_from_sector(pred_ind, ind_begin_sector);
        }
        return;
    }
    int ind_sector_min = ind_end_sector;
    for(int i = ind_begin_sector; i < ind_end_sector; i++)
    {
        if(dist(v[init_ind_v], v[sectors[i].back().second])
        < dist(v[init_ind_v], v[sectors[ind_sector_min].back().second]))
            ind_sector_min = i;
    }

    init_ind_v = move_last_v_from_sector(init_ind_v, ind_sector_min);

    f(init_ind_v, ind_begin_sector, (ind_begin_sector + ind_end_sector) / 2);
    f(init_ind_v, (ind_begin_sector + ind_end_sector) / 2 + 1, ind_end_sector);
}



int main()
{
    std::fstream in;
//    in.open("../input/Taxicab_2018.txt");
    in.open("../input/Taxicab_2018.txt");
    int n;

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

    p_center.first = (p_min.first + p_max.first) / 2;
    p_center.second = (p_min.second + p_max.second) / 2;

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
    sectors.resize(static_cast<unsigned long>(l));

    double phi_sector = 2*M_PI/l;

//    debug
//    l = 8;
//    sectors.clear();
//    sectors.resize(static_cast<unsigned long>(l));
//    phi_sector = 2*M_PI/l;
//--------------
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
        sectors[ind_sector].push_back(std::pair<int, int>(dist(v[i], v[ind_v_center]), i));
    }

    for(auto &sector : sectors)
    {
        std::sort(sector.rbegin(), sector.rend());
    }
//debug
//    f(ind_v_center, 0, (l - 1)/2);
//    f(ind_v_center, (l - 1)/2+1, l - 1);
//------------------
    int step_tree = l/3;
    for(int i = 0; i < l; i += step_tree)
    {
        f(ind_v_center, i, i + step_tree - 1);
    }

    std::fstream result("../result_2018.txt");
    result << "c Вес дерева = " << cost << ", число листьев = " << l << ',' << std::endl;
    result << "c число вершин и ребер" << std::endl;
    result << "p edge " << n << ' ' << ribs.size() << std::endl;
    result << "c ребра" << std::endl;
    for (auto &rib : ribs) {
        result << "e " << rib.second.first+1 << ' ' << rib.second.second+1 << std::endl;
    }

    return 0;
}