#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "../util/vec2.hpp"   // vec2i
#include "../util/Random.hpp" // Engine

#include <unordered_map> // unordered_map
#include <unordered_set> // unordered_set
#include <vector>        // vector

#include <optional>   // optional
#include <functional> // reference_wrapper


namespace gm
{

struct Node
{
    int district;
    int population;
};

struct Map
{

    Random::Engine random;

/*********
 * NODES *
 *********/ 
private:
    std::unordered_map<vec2i, Node, vec2i_hash> node_map;
    std::vector<vec2i> node_layout;

public:
    bool has_node(const vec2i& v) const;
    bool add_node(const vec2i& v, const Node& node);
    const std::unordered_map<vec2i, Node, vec2i_hash>& get_node_map() const;

    // std::optional<std::reference_wrapper<Node>> get_node(const vec2i& v);
    // std::optional<std::reference_wrapper<const Node>> get_node(const vec2i& v) const;

/***********
 * BORDERS *
 ***********/
private:
    std::unordered_set<vec2i, vec2i_hash> border_set;
    std::vector<vec2i> border_layout;

    const std::vector<vec2i> to_neighbors;

public: 
    bool is_border(const vec2i& location) const;

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
    void randomize(int districts);
    void find_borders();

public:
    void reset(int districts);


/*************
 * EVOLUTION *
 *************/ 

public:
    void update_border();

};



}

#endif