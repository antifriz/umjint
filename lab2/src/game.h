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
    bool operator()(const Point
    &a,
    const Point
    &b) {
        return (a.x * 10 + a.y) > (b.x * 10 + b.y);
    }
};

class Game {
    typedef PointAtom Atom;


private:
    Board _board;
    Point _position;
    KnowledgeBase<Atom> _knowledgeBase;
    std::priority_queue<Point, std::vector<Point>, ComparatorPricierPoint> _safe;
    bool _wumpusFound;
public:
    Game() : _board(), _position(1, 1), _wumpusFound(false) {
    }

    void load(std::string path);

    void initialPremisesAbout(Point p, const std::vector <Point>
    &world);

    template<bool tPoint, Property pPoint, bool tNeighbour, Property pNeighbour>
    void createRuleForNeighbours(Point p) {
        Clause<Atom> c(tPoint, Atom(pPoint, p));
        foreach(pt, _board.getNeighbours(p))c.addLiteral(tNeighbour, Atom(pNeighbour, pt));
        _knowledgeBase.addClause(c);
    }

    void run();


    bool wumpusFound();

    void deduceAndAdd(Point point, Property property);

    bool check(Point point, bool prefix, Property property);

    bool unknown(Point point, Property property);

    void step();

    void moveTo(Point const
    &point);

    void move();

    bool hasSafe();

    void addSafe(const Point
    &point);

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

    bool atSafe();


    bool ended();

    void printBoard();
};


#endif //PROJECT_GAME_H
