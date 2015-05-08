//
// Created by ivan on 5/7/15.
//

#include "game.h"

void Game::load(std::string path) {
    _board.load(path);
    _position = Point(1, 1);
    _wumpusFound = false;
}

void Game::initialPremisesAbout(Point p) {
    // todo: if not seen as adjacent

    createRuleForNeighbours<true, Stench, true, Wumpus, Op::Or>(p);
    createRuleForNeighbours<false, Stench, false, Wumpus, Op::And>(p);
    createRuleForNeighbours<true, Breeze, true, Pit, Op::Or>(p);
    createRuleForNeighbours<false, Breeze, false, Pit, Op::And>(p);
    createRuleForNeighbours<true, Glow, true, Teleport, Op::Or>(p);
    createRuleForNeighbours<false, Glow, false, Teleport, Op::And>(p);
}

void Game::run() {
    //foreach(p, _board.getAllPoints())
    //    initialPremisesAbout(p,_board.getAllPoints());

    while (!ended()) {
        _position.print();
        std::cout << std::endl;
        step();

        //break;
    }
}

bool Game::wumpusFound() {
    if (!_wumpusFound) return false;
    _wumpusFound = true;

    foreach(pt, _board.getAllPoints())if (_position != pt)
            addPositionUnaryPremise<Wumpus>(false, pt);
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
    endline();
    endline();
    _knowledgeBase.print();
    endline();

    auto &&neighbours = _board.getNeighbours(_position);
    initialPremisesAbout(_position);


    foreach(neighbourPt, neighbours)initialPremisesAbout(neighbourPt);


    addPositionUnaryPremise<Pit>(false);
    addPositionUnaryPremise<Teleport>(false);
    addPositionUnaryPremise<Wumpus>(false);

    addPositionUnaryPremise<Glow>(at<Glow>());
    addPositionUnaryPremise<Breeze>(at<Breeze>());
    addPositionUnaryPremise<Stench>(at<Stench>());
    //if (atSafe()) addSafe(_position);

    //endline();

    // _knowledgeBase.print();

    foreach(neighbourPt, neighbours)addUnknown(neighbourPt);

    //ntimes(neighbours.size())
/*        foreach(neighbourPt, neighbours) {
            deduceAndAdd(neighbourPt, Teleport);

            if (check(neighbourPt, true, Teleport)) {
                moveTo(neighbourPt);
                return;
            }

            deduceAndAdd(neighbourPt, Pit);
            deduceAndAdd(neighbourPt, Wumpus);
        }*/

    std::vector<Point> v;
    while (!_unknown.empty()) {
        Point pt = _unknown.top();
        _unknown.pop();
        v.push_back(pt);
    }


    while (!v.empty()) {
        Point pt = v.back();
        v.pop_back();

        deduceAndAdd(pt, Teleport);

        if (check(pt, true, Teleport)) {
            moveTo(pt);
            return;
        }

        deduceAndAdd(pt, Pit);
        if (check(pt, true, Wumpus))
            wumpusFound();


        deduceAndAdd(pt, Wumpus);


        if (check(pt, false, Wumpus)) if (check(pt, false, Pit))
            addSafe(pt);

        if (unknown(pt, Wumpus) || unknown(pt, Pit)) if (!check(pt, true, Wumpus) && !check(pt, true, Pit))
            addUnknown(pt);
    }


    //_knowledgeBase.print();
/*

    foreach(neighbourPt, neighbours) {
        //updateWumpusInfo(neighbourPt);

        // redundant if(_knowledgeBase.check(neighbour,false,Teleport))
        if (check(neighbourPt, false, Wumpus)) if (check(neighbourPt, false, Pit))
            addSafe(neighbourPt);

        if (unknown(neighbourPt, Wumpus) || unknown(neighbourPt, Pit)) if (!check(neighbourPt, true, Wumpus) &&
                                                                           !check(neighbourPt, true, Pit))
            addUnknown(neighbourPt);
    }
*/


    move();
    //_visited.insert(_position);
}

void Game::moveTo(Point const &point) {
    std::cout << "moved to: ";
    point.print();
    endline();
    endline();
    _position = point;

}

void Game::move() {
    if (hasSafe()) {
        printMe("safe move");
        moveTo(nextSafe());

    } else if (hasUnknown()) {
        printMe("unknown move");

        moveTo(nextUnknown());

    } else {
        printMe("predah se");
        exit(0);
    }

}

void Game::addSafe(const Point &point) {
    _safe.push(point);
}

void Game::addUnknown(const Point &point) {
    _unknown.push(point);
}

bool Game::hasSafe() {
    return !_safe.empty();
}

bool Game::hasUnknown() {
    return !_unknown.empty();
}

Point Game::nextUnknown() {
    Point p(_unknown.top());
    _unknown.pop();
    return p;
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