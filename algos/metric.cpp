#include "./metric.hpp"
using namespace gm;

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
