#include "./metric.hpp"
#include <algorithm>
#include <iostream>

using namespace gm;

// Base Metric

Metric::~Metric() 
{
}

void Metric::move_node(const vec2i& pos, Node node, District from, District to) 
{
    if (from == to) return;

    node.set_district(from);
    del_node(pos, node);

    node.set_district(to);
    add_node(pos, node);
}

void Metric::move_node(const vec2i& pos, Node node, District to)
{
    move_node(pos, node, node.get_district(), to);
}

void Metric::clear() 
{
}

void Metric::add_node(const vec2i& pos, const Node& node)
{

}

void Metric::del_node(const vec2i& pos, const Node& node)
{

}

std::string Metric::get_name() const 
{
    return "Metric";
}

// Population Metric

PopulationMetric::PopulationMetric()
{

}

void PopulationMetric::clear()
{
    total_population = 0;
    population_map.clear();
}

void PopulationMetric::add_node(const vec2i& pos, const Node& node)
{
    if (!contains(node.get_district()))
        population_map[node.get_district()] = 0;
    
    population_map[node.get_district()] += node.get_population();
    total_population += node.get_population();
}

void PopulationMetric::del_node(const vec2i& pos, const Node& node)
{
    if (!contains(node.get_district())) return;
    
    population_map[node.get_district()] -= node.get_population();
    total_population -= node.get_population();
}

bool PopulationMetric::analyze(const vec2i& pos, const Node& node, District district)
{
    auto m1 = get_average_percent_error(node.get_district(), district);
    move_node(pos, node, node.get_district(), district);
    auto m2 = get_average_percent_error(node.get_district(), district);
    move_node(pos, node, district, node.get_district());

    return m2 < m1;
}

double PopulationMetric::get_expected_population() const
{
    return total_population / population_map.size();
}

double PopulationMetric::get_average_percent_error(District d1, District d2) const
{
    const double e = get_expected_population();

    double pe1 = std::abs((e - get_population(d1))  / e);
    double pe2 = std::abs((e - get_population(d2))  / e);
        
    return (pe1 + pe2) / 2.0; 
}

int PopulationMetric::get_population(District d) const
{
    auto iter = population_map.find(d);
    if (iter == population_map.end()) return 00;

    return iter->second;
}

bool PopulationMetric::contains(District d) const
{
    return population_map.find(d) != population_map.end();
}


std::string PopulationMetric::get_name() const { return "Population"; }


PartyPopulationMetric::PartyPopulationMetric() 
{
}

void PartyPopulationMetric::add_node(const vec2i& pos, const Node& node)
{
    // if (!contains(node.get_district())) party_population_map[node.get_district()] = std::unordered_map();
    for(auto &[party, population] : node.get_party_population_map()) {
        population_map[node.get_district()] += population;
        party_population_map[node.get_district()][party] += population;
        party_to_total_supporters[party] += population;
        total_population += population;
    }
}

void PartyPopulationMetric::del_node(const vec2i& pos, const Node& node)
{
    if (!contains(node.get_district())) { std::cerr << "something is up???\n"; return; }
    for(auto &[party, population] : node.get_party_population_map()) {
        population_map[node.get_district()] -= population;
        party_population_map[node.get_district()][party] -= population;
        party_to_total_supporters[party] -= population;
        total_population -= population;
    }

}

void PartyPopulationMetric::clear() 
{ 
    total_population = 0;
    population_map.clear();
    party_to_total_supporters.clear();
    party_population_map.clear();
}

bool PartyPopulationMetric::analyze(const vec2i& pos, const Node& node, District district)
{
    auto m1 = get_average_percent_error();
    move_node(pos, node, node.get_district(), district);

/*
    std::cout << "total pop: " << total_population << std::endl;
    // std::cout << "exptexted pop: " << expected_population() << std::endl;
    std::cout << "pop data: " << std::endl;
    
    for (auto& [district, party_to_population_map] : party_population_map)
    {
        std::cout << "District " << district << "'s Composition:" << std::endl;
        for (auto& [party, population]: party_to_population_map) 
        {
            std::cout << "Party " << party << ": " << population << "people" << std::endl;
        }
    }*/

    auto m2 = get_average_percent_error();
    move_node(pos, node, district, node.get_district());

    /*
    std::cout << "total pop: " << total_population << std::endl;
    // std::cout << "exptexted pop: " << expected_population() << std::endl;
    std::cout << "pop data: " << std::endl;
    
    for (auto& [district, party_to_population_map] : party_population_map)
    {
        std::cout << "District " << district << "'s Composition:" << std::endl;
        for (auto& [party, population]: party_to_population_map) 
        {
            std::cout << "Party " << party << ": " << population << "people" << std::endl;
        }
    }*/

    return m2 < m1;
}

// The average of all percent errors (what if we used N2 distance)
double PartyPopulationMetric::get_average_percent_error() const
{   
    double result = 0.0;
    std::unordered_map<Party, double> parties_to_winners;
    // Sum of all proportions
    std::unordered_map<Party, double> parties_to_proportion_sum;

    for (auto& [district, district_party_to_population_map] : party_population_map)
    {
        Party winning_party = 
            std::max_element(
                district_party_to_population_map.cbegin(),
                district_party_to_population_map.cend(),
                [] (std::pair<Party, int> p1, std::pair<Party, int> p2) -> bool
                {
                    return p1.second < p2.second;
                }
            )->first;

        parties_to_winners[winning_party]++;
        parties_to_proportion_sum[winning_party] += district_party_to_population_map.at(winning_party) / (double)population_map.at(district);
    }

    for (auto& [party, num_representatives] : parties_to_proportion_sum) {
        double expected_winners = party_to_total_supporters.at(party) * party_population_map.size() / (double)total_population;
        // std::cout << "Party " << party << ":\n";
        // std::cout << "Expected/should win: " << expected_winners << " districts.\n";
        // std::cout << "Districts won: " << parties_to_winners[party] << ".\n";
        // std::cout << "Sum of each winner's proportion of their captured voters out of their district population: " << parties_to_proportion_sum[party] << ".\n";
        
        // L2 distance
        result += (num_representatives - expected_winners) * (num_representatives - expected_winners);
        // average percent error result += std::abs((num_representatives - expected_winners) / (double)expected_winners / (double)party_to_total_supporters.size());
    }

    return sqrt(result);
}

bool PartyPopulationMetric::contains(District d) const
{
    return party_population_map.find(d) != party_population_map.end();
}

std::string PartyPopulationMetric::get_name() const { return "Party Population"; }

CenteringMetric::CenteringMetric() :
    populations{},
    centers { }
{
}

void CenteringMetric::clear()
{
    populations.clear();
    centers.clear();
}

void CenteringMetric::add_node(const vec2i& pos, const Node& node)
{
    if (!contains(node.get_district())) 
    {
        centers[node.get_district()] = vec2i{ 0, 0 };
        populations[node.get_district()] = 0;
    }
    populations[node.get_district()] += node.get_population();
    centers[node.get_district()] += pos * node.get_population();
}

void CenteringMetric::del_node(const vec2i& pos, const Node& node)
{
    if (!contains(node.get_district()))
    {
        std::cerr << "something funny is going on " << std::endl;
        return;
    }
    populations[node.get_district()] -= node.get_population();
    centers[node.get_district()] -= pos * node.get_population();
}

bool CenteringMetric::analyze(const vec2i& pos, const Node& node, District district)
{
    auto m1 = get_distance(pos, node.get_district());
    move_node(pos, node, node.get_district(), district);
    auto m2 = get_distance(pos, district);
    move_node(pos, node, district, node.get_district());

    return m1 > m2;
}

double CenteringMetric::get_distance(const vec2i& pos, District d) const
{
    return pos.distance(get_center(d));
}

bool CenteringMetric::contains(District d) const
{
    return centers.count(d) > 0 && populations.count(d) > 0;
}

vec2i CenteringMetric::get_center(District d) const
{
    if (!contains(d)) 
    {
        std::cout << "something weird going on here " << std::endl;
        return vec2i{-1, -1};
    }
    return centers.at(d) / populations.at(d);
}

std::string CenteringMetric::get_name() const { return "Centering"; }


AlternatingMetric::AlternatingMetric() :
    fixer{}, upper{}, 
    evolutions{0}, up_period { 100000 }, fix_period { 10000 }, upping { true }
{
}

Metric& AlternatingMetric::get_metric() 
{
    Metric& a = fixer;
    Metric& b = upper;

    return upping ? b : a;
}

void AlternatingMetric::clear()
{
    fixer.clear();
    upper.clear();
}

void AlternatingMetric::add_node(const vec2i& pos, const Node& node) 
{
    fixer.add_node(pos, node);
    upper.add_node(pos, node);
}

void AlternatingMetric::del_node(const vec2i& pos, const Node& node)
{
    fixer.del_node(pos, node);
    upper.del_node(pos, node);
}

bool AlternatingMetric::analyze(const vec2i& pos, const Node& node, District district)
{
    evolutions++;
    if (upping) {
        if (evolutions > up_period)
        {
            evolutions = 0;
            upping = false;
        }
    } else {
        if (evolutions > fix_period)
        {
            evolutions = 0;
            // upping = true;
        }
    }

    return get_metric().analyze(pos, node, district);
}

std::string AlternatingMetric::get_name() const
{
    return upping ?  upper.get_name() : fixer.get_name();
}

