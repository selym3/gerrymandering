#ifndef __METRIC_HPP__
#define __METRIC_HPP__

#include <iostream>
#include <unordered_map>

#include "../util/vec2.hpp"
#include "./node.hpp"

namespace gm
{

struct Metric
{
public:
    virtual ~Metric();
    virtual void clear();

public:
    virtual void add_node(const vec2i& pos, const Node& node);
    virtual void del_node(const vec2i& pos, const Node& node);

    void move_node(const vec2i& pos, Node node, District from, District to);
    void move_node(const vec2i& pos, Node node, District to);

public:
    virtual bool analyze(const vec2i& pos, const Node& node, District district) = 0;
};

struct PopulationMetric : public Metric
{
public:
    PopulationMetric();
    void clear();

public:
    void add_node(const vec2i& pos, const Node& node);
    void del_node(const vec2i& pos, const Node& node);
    
public:
    bool analyze(const vec2i& pos, const Node& node, District district);

private:
    double get_expected_population() const;
    double get_average_percent_error(District d1, District d2) const;

private:
    std::unordered_map<District, int> population_map;
    int total_population;

    int get_population(District d) const;
    bool contains(District d) const;
};

/*
struct PopulationMetric 
{
private:
    std::unordered_map<District, int> population_map;
    int total_population;

    bool contains(District d) const { return population_map.find(d) != population_map.end(); }

public:
    PopulationMetric() : 
        total_population {0},
        population_map {}
    {
    }

public:
    void add_node(const vec2i& pos, const Node& node)
    {
        if (!contains(node.district)) population_map[node.district] = 0;
        population_map[node.district] += node.population;
        
        // std::cout << "CHANGING POP" << std::endl;
        total_population += node.population;
    }

    void remove_node(const vec2i& pos, const Node& node)
    {
        if (!contains(node.district)) { std::cerr << "something is up???\n"; return; }
        population_map[node.district] -= node.population;

        // std::cout << "CHANGING POP" << std::endl;
        total_population -= node.population;

    }

    void move_node(const vec2i& pos, const Node& node, District d1, District d2)
    {
        if (d1 == d2) return;

        Node temp = node;
        temp.district = d1;
        remove_node(pos, temp);

        temp.district = d2;
        add_node(pos, temp);
    }

    void move_node(const vec2i& pos, const Node& node, District dest)
    {
        move_node(pos, node, node.district, dest);
    }

    void clear() 
    { 
        total_population = 0;
        population_map.clear();
    }

public:
    bool analyze(const vec2i& pos, const Node& node, District district)
    {
        auto m1 = get_measurement(district, node.district);
        // std::cout << m1 << " ";
        move_node(pos, node, node.district, district);
        auto m2 = get_measurement(district, node.district);
        // std::cout << m2 << "\n";
        move_node(pos, node, district, node.district);

        // std::cout << "total pop: " << total_population << std::endl;
        // std::cout << "expected pop: " << expected_population() << std::endl;
        // std::cout << "pop data: " << total_population << std::endl;
        // for (auto& [district, pop] : population_map)
        // {
        //     std::cout << district << " " << pop << std::endl;
        // }

        return m2 > m1;
    }

private:
    const int DISTRICTS = 4; //TODO

    double expected_population() const { return static_cast<double>(total_population) / DISTRICTS; }

    double get_measurement(const District &d1, const District &d2)
    { 
        double e1 = std::abs((expected_population() - population_map[d1])  / expected_population());
        double e2 = std::abs((expected_population() - population_map[d2])  / expected_population());
        
        return 1.0 - (e1 + e2) / 2.0; 
    }

};
*/

/*

using Party = int;
struct PartyPopulationMetric 
{
private:
    std::unordered_map<District, std::unordered_map<Party, int>> party_population_map;
    std::unordered_map<Party, int> party_to_total_supporters;
    int total_population;

    bool contains(District d) const { return party_population_map.find(d) != party_population_map.end(); }

public:
    PartyPopulationMetric() : 
        total_population {0},
        party_to_total_supporters {},
        party_population_map {}
    {
    }

public:
    void add_node(const vec2i& pos, const Node& node)
    {
       // if (!contains(node.district)) party_population_map[node.district] = std::unordered_map();
        for(auto &[party, population] : node.populations_of_each_party) {
            party_population_map[node.district][party] += population;
            party_to_total_supporters[party] += population;
            total_population += population;
        }
    }

    void remove_node(const vec2i& pos, const Node& node)
    {
        if (!contains(node.district)) { std::cerr << "something is up???\n"; return; }
        for(auto &[party, population] : node.populations_of_each_party) {
            party_population_map[node.district][party] -= population;
            party_to_total_supporters[party] -= population;
            total_population -= population;
        }

    }

    District move_node(const vec2i& pos, const Node& node, District district)
    {
        District old_district = node.district;
        remove_node(pos, node);

        Node moved = node;
        moved.district = district;
        add_node(pos, moved);
        return old_district;
    }

    void clear() 
    { 
        total_population = 0;
        party_to_total_supporters.clear();
        party_population_map.clear();
    }

public:
    bool analyze(const vec2i& pos, const Node& node, District district)
    {
        auto m1 = get_measurement();
        std::cout << m1 << " ";
        District old = move_node(pos, node, district);
        auto m2 = get_measurement();
        std::cout << m2 << "\n";
        move_node(pos, node, old);

        // std::cout << "total pop: " << total_population << std::endl;
        // std::cout << "exptexted pop: " << expected_population() << std::endl;
        // std::cout << "pop data: " << std::endl;
        for (auto& [district, district_party_to_population_map] : party_population_map)
        {
            std::cout << "District " << district << "'s Composition:" << std::endl;
            for (auto& [party, population]: district_party_to_population_map) 
            {
                std::cout << "Party " << party << ": " << population << "people" << std::endl;
            }
        }

        return m2 > m1;
    }

private:
    const int DISTRICTS = 3; //TODO
    const int PARTIES = 2; // TODO
    // The average of all percent errors (what if we used N2 distance)
    double get_measurement()
    {   
        double result = 0.0;
        std::unordered_map<Party, int> parties_to_winners;

        for (auto& [district, district_party_to_population_map] : party_population_map)
        {
            Party winning_party = 
                std::max_element(
                    district_party_to_population_map.begin(),
                    district_party_to_population_map.end(),
                    [] (std::pair<Party, int> p1, std::pair<Party, int> p2) -> bool
                    {
                        return p1.second < p2.second;
                    }
                )->first;

            ++parties_to_winners[winning_party];
        }

        for (auto& [party, num_representatives] : parties_to_winners) {
            int expected_winners = party_to_total_supporters[party];
            result += std::abs((num_representatives - expected_winners) / (double)expected_winners / (double)PARTIES);
        }

        return result;
    }

};
*/
}


#endif
