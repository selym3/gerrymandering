#ifndef __METRIC_HPP__
#define __METRIC_HPP__

#include <unordered_map>
#include <memory>
#include <functional>

#include "../util/vec2.hpp"
#include "./node.hpp"

namespace gm
{

struct Metric
{
public:
    virtual ~Metric();
    virtual void clear();

    virtual std::string get_name() const;

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

    std::string get_name() const;

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

struct PartyPopulationMetric : public Metric
{
public:
    PartyPopulationMetric();
    void clear();

    std::string get_name() const;

public:
    void add_node(const vec2i& pos, const Node& node);
    void del_node(const vec2i& pos, const Node& node);
public:
    bool analyze(const vec2i& pos, const Node& node, District district);

private:
    std::unordered_map<District, int> population_map;
    std::unordered_map<District, std::unordered_map<Party, int>> party_population_map;
    std::unordered_map<Party, int> party_to_total_supporters;
    int total_population;

    int get_total_population(District d) const;
    int get_population_in_party(District d, Party p);
    bool contains(District d) const;

    double get_average_percent_error() const;
};

struct CenteringMetric : public Metric
{
public:
    CenteringMetric();
    void clear() override;

    std::string get_name() const;

public: 
    void add_node(const vec2i& pos, const Node& node) override;
    void del_node(const vec2i& pos, const Node& node) override;

public:
    bool analyze(const vec2i& pos, const Node&, District district);

private:
    std::unordered_map<District, int> populations; // total population in each district
    std::unordered_map<District, vec2i> centers; // weighted sum of the centers

    double get_distance(const vec2i& pos, District d) const;
    bool contains(District d) const;
    vec2i get_center(District d) const; 
};

struct AlternatingMetric : public Metric
{
public:
    AlternatingMetric();
    void clear() override;

    std::string get_name() const;

public: 
    void add_node(const vec2i& pos, const Node& node) override;
    void del_node(const vec2i& pos, const Node& node) override;

public:
    bool analyze(const vec2i& pos, const Node&, District district) override;

private:
    CenteringMetric fixer;
    PartyPopulationMetric upper;
    
    int evolutions;
    bool upping;

    int fix_period;
    int up_period;

    Metric& get_metric();
};

struct MetricGroup : public Metric 
{
public:
    using BoolOp = std::function<bool(bool, bool)>;

public:
    MetricGroup(const BoolOp& op, std::shared_ptr<Metric>&& lhs, std::shared_ptr<Metric>&& rhs); 
    MetricGroup(std::shared_ptr<Metric>&& lhs, std::shared_ptr<Metric>&& rhs); 
    void clear() override;

    std::string get_name() const;

public:
    public: 
    void add_node(const vec2i& pos, const Node& node) override;
    void del_node(const vec2i& pos, const Node& node) override;

public:
    bool analyze(const vec2i& pos, const Node&, District district) override;

private:
    std::shared_ptr<Metric> lhs;
    std::shared_ptr<Metric> rhs;

    BoolOp op;

};

}


#endif
