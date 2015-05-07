//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

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

    void load(std::string path) {
        _board.load(path);
        _position = Point(1, 1);
        _wumpusFound = false;
    }

    void initialPremisesAbout(Point p, const std::vector<Point> &world) {
        // todo: if not seen as adjacent

        createRuleForNeighbours<false, Stench, false, Wumpus>(p);
        createRuleForNeighbours<false, Breeze, false, Pit>(p);
        createRuleForNeighbours<false, Glow, false, Teleport>(p);

        // wumpus at point
        foreach(pt, world)
            if (p != pt)
                _knowledgeBase.addClause(Clause<Atom>(false, Atom(Wumpus, p), false, Atom(Wumpus, pt)));
    }

    template<bool tPoint, Property pPoint, bool tNeighbour, Property pNeighbour>
    void createRuleForNeighbours(Point p) {
        Clause<Atom> c(tPoint, Atom(pPoint, p));
        foreach(pt, _board.getNeighbours(p))c.addLiteral(tNeighbour, Atom(pNeighbour, pt));
        _knowledgeBase.addClause(c);
    }

    void run() {
        //foreach(p, _board.getAllPoints())
        //    initialPremisesAbout(p,_board.getAllPoints());

        while (!ended()) {
            _position.print();
            std::cout << std::endl;
            step();
            break;
        }
    }


    bool wumpusFound() {
        if (!_wumpusFound) return false;
        _wumpusFound = true;
        return true;
    }

    void deduceAndAdd(Point point, Property property) {
        _knowledgeBase.deduceAndAdd(Atom(property, point));
    }

    bool check(Point point, bool prefix, Property property) {
        return _knowledgeBase.clauseExists(Clause<Atom>(prefix, Atom(property, point)));
    }

    bool unknown(Point point, Property property) {
        return !check(point, true, property) && !check(point, false, property);
    }

    void step() {


        auto &&neighbours = _board.getNeighbours(_position);

        initialPremisesAbout(_position, _board.getAllPoints());


        foreach(neighbourPt, neighbours)initialPremisesAbout(neighbourPt, _board.getAllPoints());


        addPositionUnaryPremise<false, Pit>();
        addPositionUnaryPremise<false, Teleport>();
        addPositionUnaryPremise<false, Wumpus>();


        if (at<Glow>()) addPositionUnaryPremise<true, Glow>();
        if (at<Breeze>()) addPositionUnaryPremise<true, Breeze>();
        if (at<Stench>()) addPositionUnaryPremise<true, Stench>();

        if (atSafe()) addSafe(_position);


        ntimes(neighbours.size())foreach(neighbourPt, neighbours) {
                deduceAndAdd(neighbourPt, Teleport);

                if (check(neighbourPt, true, Teleport)) {
                    moveTo(neighbourPt);
                    return;
                }

                deduceAndAdd(neighbourPt, Pit);
                deduceAndAdd(neighbourPt, Wumpus);

                //updateWumpusInfo(neighbourPt);

                // redundant if(_knowledgeBase.check(neighbour,false,Teleport))
                if (check(neighbourPt, false, Wumpus)) if (check(neighbourPt, false, Pit))
                    addSafe(neighbourPt);

                if (unknown(neighbourPt, Wumpus) || unknown(neighbourPt, Pit)) if (!check(neighbourPt, true, Wumpus) &&
                                                                                   !check(neighbourPt, true,
                                                                                          Pit));//addUnknown(neighbourPt);

            }
        move();
    }

    void moveTo(Point const &point) {
        _position = point;
    }

    void move() {
        if (!hasSafe()) {
            moveTo(nextSafe());
        } else
            assert("jej");


    }

    bool hasSafe() {
        return _safe.empty();
    }

    void addSafe(const Point &point) {
        printMe("addedSafe:");
        point.print();
        endline();
        _safe.push(point);
    }

    Point nextSafe() {
        Point p(_safe.top());
        _safe.pop();
        return p;
    }

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

    bool atSafe() {
        return !at<Breeze>() && !at<Stench>();
    }


    bool ended() {
        if (at<Teleport>()) {
            printMe("Pikard se spasio, pronasao je teleporter :)");
            return true;
        }
        if (at<Wumpus>()) {
            printMe("Pikarda je ubio Wumpus :(, tko god to bio ?!");
            return true;
        }
        if (at<Pit>()) {
            printMe("Pikard je pao u jamu i umro :(");
            return true;
        }
        return false;
    }

    void printBoard() {
        _board.print();
    }
};

#include <string>
#include <vector>
#include <iomanip>
#include <queue>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <set>
#include <iostream>
#include "point.h"
#include "point_atom.h"
#include "board.h"
#include "literal.h"
#include "clause.h"
#include "knowledge_base.h"

#endif //PROJECT_GAME_H
