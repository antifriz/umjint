//
// Created by ivan on 5/7/15.
//

#include "game.h"
#include <assert.h>

void Game::load(std::string path) {
    _board.load(path);
    _position = Point(1, 1);
    _wumpusFound = false;
}

void Game::initialPremisesAbout(Point p, const std::vector<Point> &world) {
    // todo: if not seen as adjacent

    createRuleForNeighbours<false, Stench, false, Wumpus>(p);
    createRuleForNeighbours<false, Breeze, false, Pit>(p);
    createRuleForNeighbours<false, Glow, false, Teleport>(p);

    // wumpus at point
    foreach(pt, world)if (p != pt)
            _knowledgeBase.addClause(Clause<Atom>(false, Atom(Wumpus, p), false, Atom(Wumpus, pt)));
}

void Game::run() {
    //foreach(p, _board.getAllPoints())
    //    initialPremisesAbout(p,_board.getAllPoints());

    while (!ended()) {
        _position.print();
        std::cout << std::endl;
        step();
        break;
    }
}

bool Game::wumpusFound() {
    if (!_wumpusFound) return false;
    _wumpusFound = true;
    return true;
}

void Game::deduceAndAdd(Point point, Property property) {
    _knowledgeBase.deduceAndAdd(Atom(property, point));
}

bool Game::unknown(Point point, Property property) {
    return !check(point, true, property) && !check(point, false, property);
}

bool Game::check(Point point, bool prefix, Property property) {
    return _knowledgeBase.clauseExists(Clause<Atom>(prefix, Atom(property, point)));
}

void Game::step() {


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


    ntimes(neighbours.size())
        foreach(neighbourPt, neighbours) {
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

void Game::moveTo(Point const &point) {
    _position = point;
}

void Game::move() {
    if (!hasSafe()) {
        moveTo(nextSafe());
    } else
        assert("jej");


}

void Game::addSafe(const Point &point) {
    printMe("addedSafe:");
    point.print();
    endline();
    _safe.push(point);
}

bool Game::hasSafe() {
    return _safe.empty();
}

Point Game::nextSafe() {
    Point p(_safe.top());
    _safe.pop();
    return p;
}

bool Game::atSafe() {
    return !at<Breeze>() && !at<Stench>();
}

bool Game::ended() {
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

void Game::printBoard() {
    _board.print();
}