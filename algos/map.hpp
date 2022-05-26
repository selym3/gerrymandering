#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "../util/vec2.hpp"
#include <unordered_map>

namespace gm
{

struct Node
{
    int district;
    int population;
};

struct Map
{

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

/*********
 * NODES *
 *********/ 

public:
    // accessor methods
    const std::unordered_map<vec2i, Node>& get_nodes() const;
    
    bool is_border(const vec2i& location) const;
    bool is_border(const Node& node) const;
};



}

#endif