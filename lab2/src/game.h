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
    std::set<Point> _visited;
    bool _wumpusFound;
public:
    enum Op { Or, And };

    Game() : _board(), _position(1, 1), _wumpusFound(false) {
    }

    std::vector<Point> getNewNeighbours();

    void load(std::string path);

    void printVisited();

    void initialPremisesAbout(Point p);

    template<bool tPoint, Property pPoint, bool tNeighbour, Property pNeighbour, Op op>
    void createRuleForNeighbours(Point p) {
        if (op == Op::Or) {
            Clause<Atom> c(!tPoint, Atom(pPoint, p));
            foreach(pt, _board.getNeighbours(p))c.addLiteral(tNeighbour, Atom(pNeighbour, pt));
            _knowledgeBase.addClause(c);
        } else {
            foreach(pt, _board.getNeighbours(p)) _knowledgeBase.addClause(
                        Clause<Atom>(!tPoint, Atom(pPoint, p), tNeighbour, Atom(pNeighbour, pt)));
        }
    }

    void run();


    bool wumpusFound();

    bool deduceAndAdd(bool prefix, Property property, Point point);

    bool check(bool prefix, Property property, Point point);

    bool unknown(Property property, Point point);

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


    template<Property property>
    void addPositionUnaryPremise(bool prefix) {
        addPositionUnaryPremise<property>(prefix, _position);
    }

    template<Property property>
    void addPositionUnaryPremise(bool prefix, Point point) {
        _knowledgeBase.addClause(Clause<Atom>(prefix, Atom(property, point)));
    }

    template<bool prefix, Property property>
    void addNeighbouringPresumption() {
        Clause<Atom> clause;
        foreach(neighbour, _board.getNeighbours(_position))clause.addLiteral(prefix, Atom(property, neighbour));
        _knowledgeBase.addClause(clause);
    }


    template<Property property>
    bool at() {
        return _board.is<property>(_position);
    }


    bool ended();

    void printBoard();

    void addUnknown(Point const &point);

    bool hasUnknown();

    Point nextUnknown();
};


#endif //PROJECT_GAME_H
