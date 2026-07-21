#include "PathFinderFactory.hpp"

#include "../Dijkstra/DijkstraPathFinder.hpp"


namespace algorithms
{

std::unique_ptr<IPathFinder>
PathFinderFactory::create(
    AlgorithmType algorithm
)
{
    switch (algorithm)
    {
        case AlgorithmType::Dijkstra:
            return std::make_unique<DijkstraPathFinder>();

        case AlgorithmType::AStar:
            return nullptr;

        default:
            return nullptr;
    }
}

}
