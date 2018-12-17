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
 cost, {ind_v1, ind_v2}
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
    int ind_sector_min = -1;
    for(int i = ind_begin_sector; i <= ind_end_sector; i++)
    {
        if(ind_sector_min != -1)
        {
            if(sectors[i].empty())
                continue;
            if(dist(v[init_ind_v], v[sectors[i].back().second])
                < dist(v[init_ind_v], v[sectors[ind_sector_min].back().second]))
                ind_sector_min = i;

        }
        else {
            if(!sectors[i].empty())
                ind_sector_min = i;
        }
    }

    if(ind_sector_min == -1)
        return;
    init_ind_v = move_last_v_from_sector(init_ind_v, ind_sector_min);

    f(init_ind_v, ind_begin_sector, (ind_begin_sector + ind_end_sector) / 2);
    f(init_ind_v, (ind_begin_sector + ind_end_sector) / 2 + 1, ind_end_sector);
}

void f_main(int ind_v_center, int l, double phi_sector) {
    cost = 0;
    ribs.clear();
    for (auto &s : sectors)
        s.clear();

    for (int j = 0; j < v.size(); j++) {
        if (j == ind_v_center)
            continue;
        double phi_v;
        int x = v[j].first - v[ind_v_center].first, y = v[j].second - v[ind_v_center].second;
        if (x == 0) {
            if (y > 0)
                phi_v = M_PI / 2;
            else
                phi_v = 3 * M_PI / 2;
        } else if (x < 0) {
            phi_v = std::atan(double(y) / double(x)) + M_PI;
        } else {
            phi_v = std::atan(double(y) / double(x));
        }
        if (phi_v < 0) {
            phi_v += M_PI * 2;
        }
        auto ind_sector = static_cast<int>(phi_v / phi_sector);
        sectors[ind_sector].push_back(std::pair<int, int>(dist(v[j], v[ind_v_center]), j));
    }

    for (auto &sector : sectors) {
        std::sort(sector.rbegin(), sector.rend());
    }

    int step_tree = l / 3;
    for (int j = 0; j < l; j += step_tree) {
        f(ind_v_center, j, j + step_tree - 1);
    }
}


int main()
{
    std::fstream in;
    in.open("../input/Taxicab_2018.txt");
    int n;

    std::pair<int, int> p_min, p_max, p_center;
//    ... чтение графа ...
    in >> n;
    v.emplace_back();
    in >> v.back().first >> v.back().second;
//    p_min.first = v.back().first;
//    p_max.first = v.back().first;
//    p_min.second = v.back().second;
//    p_max.second = v.back().second;
//
    for(int i = 1; i < n; i++)
    {
        v.emplace_back();
        in >> v.back().first >> v.back().second;
//
//        if(p_min.first > v.back().first)
//            p_min.first = v.back().first;
//
//        if(p_max.first < v.back().first)
//            p_max.first = v.back().first;
//
//        if(p_min.second > v.back().second)
//            p_min.second = v.back().second;
//
//        if(p_max.second < v.back().second)
//            p_max.second = v.back().second;
//
    }
    in.close();

    int l_max = n / 10, l = 3;
    while(l < l_max)
    {
        l <<= 1;
    }
    l >>= 1;
    sectors.resize(static_cast<unsigned long>(l));

    double phi_sector = 2*M_PI/l;

    f_main(0, l, phi_sector);
    int cost_min = cost, ind_v_min = 0;

    for (int j = 1; j < v.size(); ++j) {
        f_main(j, l, phi_sector);
        if (cost < cost_min) {
            cost_min = cost;
            ind_v_min = j;
        }
    }

    f_main(ind_v_min, l, phi_sector);

    std::vector<int> pow_v(v.size(), 0);
    for(auto &i : ribs)
    {
        pow_v[i.second.first] += 1;
        pow_v[i.second.second] += 1;
    }
    int count_leaf = 0;
    for(auto &i : pow_v)
    {
        if(i == 1)
            count_leaf += 1;
    }


    std::fstream result("../result_2018.txt");
    result << "c Вес дерева = " << cost << ", число листьев = " << count_leaf << ',' << std::endl;
    result << "c число вершин и ребер" << std::endl;
    result << "p edge " << n << ' ' << ribs.size() << std::endl;
    result << "c ребра" << std::endl;
    for (auto &rib : ribs) {
        result << "e " << rib.second.first+1 << ' ' << rib.second.second+1 << std::endl;
    }

    return 0;
}