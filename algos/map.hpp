#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "../util/vec2.hpp"   // vec2i
#include "../util/Random.hpp" // Engine

#include <unordered_map> // unordered_map
#include <unordered_set> // unordered_set
#include <vector>        // vector

#include <optional>   // optional
#include <functional> // reference_wrapper

#include "./node.hpp"   // District, Node
#include "./metric.hpp" // PopulationMetric, BiasedMetric

namespace gm
{
    
struct Map
{

/*********
 * UTILS *
 *********/ 
private:
    const Random::Engine random;
    const std::vector<vec2i> to_neighbors;

/*********
 * NODES *
 *********/ 
private:
    std::unordered_map<vec2i, Node, vec2i_hash> node_map;
    std::vector<vec2i> node_layout;

private:
    const vec2i& get_random_node_location() const;
    Node& get_random_node();

public:
    bool has_node(const vec2i& v) const;
    bool add_node(const vec2i& v, const Node& node);
    const std::unordered_map<vec2i, Node, vec2i_hash>& get_node_map() const;

    // std::optional<std::reference_wrapper<Node>> get_node(const vec2i& v);
    // std::optional<std::reference_wrapper<const Node>> get_node(const vec2i& v) const;

    // std::vector<std::reference_wrapper<const Node>> get_neighbors(const vec2i& v) const;

/***********
 * BORDERS *
 ***********/
private:
    std::unordered_set<vec2i, vec2i_hash> border_set;
    std::vector<vec2i> border_layout;

private:

    // determines from the surrounding districts whether or not 
    // the node at pos is a border. can be used to update border 
    // and layout and to keep it in sync as it needs to be changed.
    bool calculate_border(const vec2i& pos) const;

    // adds the pos to the border set and layout (no effect if already added)
    void add_border_one(const vec2i& pos);

    // removes the pos from the border set and layout (no effect if not in border set)
    void remove_border_one(const vec2i& pos);

    // if this pos is a border, adds to the border set and layout.
    // otherwise, it removes the pos from the border set and layout.
    void update_border_one(const vec2i& pos);

    // updates the pos and surrounding neighbors whose neighbor status
    // may have changed as a result of pos changing
    void update_border(const vec2i& pos);

private:
    const vec2i& get_random_border_location() const;

public: 
    bool is_border(const vec2i& pos) const;

    std::unordered_set<District> get_neighboring_districts(const vec2i& v) const;

/*******************
 * FACTORY METHODS *
 *******************/ 

public:
    static Map make_grid(int width, int height);

protected:
    Map();


/******************
 * INITIALIZATION *
 ******************/ 

private:
    // District get_district(int index) const;

    void randomize(int districts);
public: 
    void find_borders();

public:
    void reset(int districts);


/*************
 * EVOLUTION *
 *************/ 

private:
    BiasedMetric metric;

    void evolve(const vec2i& v);

public:

    /**
     * - grab a random border node
     * - get that nodes neighboring districts
     * - calculate fairness before and after flipping it to a neighbor
     * - remove node from border if it is flipped
     */
    void evolve();

};


}

#endif