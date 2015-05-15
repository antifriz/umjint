//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include "atom.h"
#include "board.h"
#include "knowledge_base.h"
#include "clause.h"
#include "pricier_point_comparator.h"
#include <queue>

class Game {

private:
    Board _board;
    Point _position;
    KnowledgeBase _knowledgeBase;
    std::priority_queue<Point, std::vector<Point>, ComparatorPricierPoint> _safe;
    std::priority_queue<Point, std::vector<Point>, ComparatorPricierPoint> _unknown;
    std::set<Point> visited;
    std::set<Point> seen;
    bool _wumpusFound;
public:

    Game() : _board(), _position(1, 1), _wumpusFound(false) {
    }

    std::vector<Point> getNewNeighbours();

    void load(std::string path);

    void printVisited();

    void run();


    bool deduceAndAdd(bool prefix, Property property, Point point);

    void step();

    void moveTo(Point const &point);

    void move();

    bool hasSafe();

    void addSafe(const Point &point);

    Point nextSafe();



    template<bool prefix, Property property>
    void addPositionUnaryPremise() {
        addPositionUnaryPremise<prefix, property>(_position);
    }

    template<bool prefix, Property property>
    void addPositionUnaryPremise(Point point) {
        _knowledgeBase.addClause(Clause(prefix, Atom(property, point)));
    }

    template<Property property>
    void addNeighbouringPresumption(bool prefix) {
        if(prefix)
        {
            Clause clause;
            foreach(neighbour, _board.getNeighbours(_position))clause.addLiteral(true, Atom(property, neighbour));
            _knowledgeBase.addClause(clause);

            foreach(neighbour, _board.getNeighbours(_position))
                foreach(neighbour2, _board.getNeighbours(_position))
                    if (!(neighbour == neighbour2))
                    {
                        _knowledgeBase.addClause(Clause(false, Atom(Wumpus, neighbour),false, Atom(Pit, neighbour)));
                        _knowledgeBase.addClause(Clause(false, Atom(Wumpus, neighbour),false, Atom(Teleport, neighbour)));
                        _knowledgeBase.addClause(Clause(false, Atom(Pit, neighbour),false, Atom(Teleport, neighbour)));
                        if (property == Wumpus)
                            _knowledgeBase.addClause(Clause(false, Atom(Wumpus, neighbour), false, Atom(Wumpus, neighbour2)));

                    }
        } else{
            foreach(neighbour, _board.getNeighbours(_position))
                    _knowledgeBase.addClause(Clause(false, Atom(property, neighbour)));
        }
    }


    template<Property property>
    bool at() {
        return _board.is<property>(_position);
    }


    bool ended();


    void addUnknown(Point const &point);

    bool hasUnknown();

    Point nextUnknown();
};


#endif //PROJECT_GAME_H
