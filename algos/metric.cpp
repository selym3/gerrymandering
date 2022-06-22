#include "./metric.hpp"
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


PartyPopulationMetric::PartyPopulationMetric() : 
    total_population {0},
    party_to_total_supporters {},
    party_population_map {}
{
}

void PartyPopulationMetric::add_node(const vec2i& pos, const Node& node)
{
    // if (!contains(node.get_district())) party_population_map[node.get_district()] = std::unordered_map();
    for(auto &[party, population] : node.get_party_population_map()) {
        party_population_map[node.get_district()][party] += population;
        party_to_total_supporters[party] += population;
        total_population += population;
    }
}

void PartyPopulationMetric::del_node(const vec2i& pos, const Node& node)
{
    if (!contains(node.get_district())) { std::cerr << "something is up???\n"; return; }
    for(auto &[party, population] : node.get_party_population_map()) {
        party_population_map[node.get_district()][party] -= population;
        party_to_total_supporters[party] -= population;
        total_population -= population;
    }

}

void PartyPopulationMetric::clear() 
{ 
    total_population = 0;
    party_to_total_supporters.clear();
    party_population_map.clear();
}

bool PartyPopulationMetric::analyze(const vec2i& pos, const Node& node, District district)
{
    auto m1 = get_average_percent_error();
    move_node(pos, node, node.get_district(), district);
    auto m2 = get_average_percent_error();
    move_node(pos, node, district, node.get_district());

    return m2 < m1;
    // std::cout << "total pop: " << total_population << std::endl;
    // std::cout << "exptexted pop: " << expected_population() << std::endl;
    // std::cout << "pop data: " << std::endl;
    /*
    for (auto& [district, district_party_to_population_map] : party_population_map)
    {
        std::cout << "District " << district << "'s Composition:" << std::endl;
        for (auto& [party, population]: district_party_to_population_map) 
        {
            std::cout << "Party " << party << ": " << population << "people" << std::endl;
        }
    }*/
}

// The average of all percent errors (what if we used N2 distance)
double PartyPopulationMetric::get_average_percent_error()
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
        result += std::abs((num_representatives - expected_winners) / (double)expected_winners / (double)party_to_total_supporters.size());
    }

    return result;
}
