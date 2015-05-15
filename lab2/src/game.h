//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include "point_atom.h"
#include "board.h"
#include "knowledge_base.h"
#include "clause.h"
#include <queue>
#include <vector>

class ComparatorPricierPoint {
public:
    bool operator()(const Point &a, const Point &b) {
        return (a.x * 10 + a.y) == (b.x * 10 + b.y) ? (a < b) : (a.x * 10 + a.y) > (b.x * 10 + b.y);
    }
};

class Game {
    typedef PointAtom Atom;


private:
    Board _board;
    Point _position;
    KnowledgeBase<Atom> _knowledgeBase;
    std::priority_queue<Point, std::vector<Point>, ComparatorPricierPoint> _safe;
    std::priority_queue<Point, std::vector<Point>, ComparatorPricierPoint> _unknown;
    std::set<Point> visited;
    std::set<Point> seen;
    bool _wumpusFound;
public:/*
    enum Op { Or, And };*/

    Game() : _board(), _position(1, 1), _wumpusFound(false) {
    }

    std::vector<Point> getNewNeighbours();

    void load(std::string path);

    void printVisited();
/*
    void initialPremisesAbout(Point p);*/
/*
    template<bool tPoint, Property pPoint, bool tNeighbour, Property pNeighbour, Op op>
    void createRuleForNeighbours(Point p) {
        if (op == Op::Or) {
            Clause<Atom> c(!tPoint, Atom(pPoint, p));
            foreach(pt, _board.getNeighbours(p)) {
                c.addLiteral(tNeighbour, Atom(pNeighbour, pt));
            }
            _knowledgeBase.addClause(c);
        } else {
            foreach(pt, _board.getNeighbours(p)) _knowledgeBase.addClause(
                        Clause<Atom>(!tPoint, Atom(pPoint, p), tNeighbour, Atom(pNeighbour, pt)));
        }

    }*/

    void run();

/*
    bool wumpusFound();*/

    bool deduceAndAdd(bool prefix, Property property, Point point);
/*
    bool check(bool prefix, Property property, Point point);*/
/*

    bool unknown(Property property, Point point);
*/

    void step();

    void moveTo(Point const &point);

    void move();

    bool hasSafe();

    void addSafe(const Point &point);

    Point nextSafe();

/*    void updateWumpusInfo(Point point) {
        if (!check(point, true, Wumpus)) return;
        if (wumpusFound()) return;

        foreach(otherPoint, _board.allFields())if (otherPoint != point)
                addPositionUnaryPremise<false, Wumpus>(otherPoint);
    }*/


    template<bool prefix, Property property>
    void addPositionUnaryPremise() {
        addPositionUnaryPremise<prefix, property>(_position);
    }

    template<bool prefix, Property property>
    void addPositionUnaryPremise(Point point) {
        _knowledgeBase.addClause(Clause<Atom>(prefix, Atom(property, point)));
    }

    template<Property property>
    void addNeighbouringPresumption(bool prefix) {
        if(prefix)
        {
            Clause<Atom> clause;
            foreach(neighbour, _board.getNeighbours(_position))clause.addLiteral(true, Atom(property, neighbour));
            _knowledgeBase.addClause(clause);

            foreach(neighbour, _board.getNeighbours(_position))
                foreach(neighbour2, _board.getNeighbours(_position))
                    if (!(neighbour == neighbour2))
                    {
                        _knowledgeBase.addClause(Clause<Atom>(false, Atom(Wumpus, neighbour),false, Atom(Pit, neighbour)));
                        _knowledgeBase.addClause(Clause<Atom>(false, Atom(Wumpus, neighbour),false, Atom(Teleport, neighbour)));
                        _knowledgeBase.addClause(Clause<Atom>(false, Atom(Pit, neighbour),false, Atom(Teleport, neighbour)));
                        if (property == Wumpus)
                            _knowledgeBase.addClause(Clause<Atom>(false, Atom(Wumpus, neighbour), false, Atom(Wumpus, neighbour2)));

                    }
        } else{
            foreach(neighbour, _board.getNeighbours(_position))
                    _knowledgeBase.addClause(Clause<Atom>(false, Atom(property, neighbour)));
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
