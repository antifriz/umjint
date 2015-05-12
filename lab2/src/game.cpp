//
// Created by ivan on 5/7/15.
//

#include "game.h"


void printSet(std::priority_queue<Point, std::vector<Point>, ComparatorPricierPoint> sett) {
    std::vector<Point> tmp;
    while (!sett.empty()) {
        auto pt = sett.top();
        tmp.push_back(pt);
        pt.print();
        std::cout << pt.x * 10 + pt.y << " ";
        sett.pop();
    }
    while (!tmp.empty()) {
        sett.push(tmp.back());
        tmp.pop_back();
    }

    endline();

}


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

    while (!ended()) {
        printVisited();
        //printSet(_safe);
        //printSet(_unknown);
        step();
    }
}

bool Game::wumpusFound() {
    if (!_wumpusFound) return false;
    _wumpusFound = true;

    foreach(pt, _board.getAllPoints())if (_position != pt)
            addPositionUnaryPremise<Wumpus>(false, pt);
    return true;
}

bool Game::deduceAndAdd(bool prefix, Property property, Point point) {
    return _knowledgeBase.deduceAndAdd(Literal<Atom>(prefix, Atom(property, point)));
}

bool Game::unknown(Property property, Point point) {
    return !check(true, property, point) && !check(false, property, point);
}

bool Game::check(bool prefix, Property property, Point point) {
    auto s = _knowledgeBase.clauseExists(Clause<Atom>(prefix, Atom(property, point)));
    return s;
}

std::vector<Point> Game::getNewNeighbours() {
    auto &&neighboursProposed = _board.getNeighbours(_position);
    std::vector<Point> neighboursNew;
    foreach(neighbour, neighboursProposed)if (_visited.find(neighbour) == _visited.end())
            neighboursNew.push_back(neighbour);
    return neighboursNew;
}

void Game::step() {

    _visited.insert(_position);

    auto &&neighbours = getNewNeighbours();

    foreach(neigbour, neighbours) {
        neigbour.print();
        endline();
    }
    initialPremisesAbout(_position);

    foreach(neighbourPt, neighbours)initialPremisesAbout(neighbourPt);

    addPositionUnaryPremise<Pit>(false);
    addPositionUnaryPremise<Teleport>(false);
    addPositionUnaryPremise<Wumpus>(false);

    addPositionUnaryPremise<Glow>(at<Glow>());
    addPositionUnaryPremise<Breeze>(at<Breeze>());
    addPositionUnaryPremise<Stench>(at<Stench>());

    foreach(neighbourPt, neighbours)addUnknown(neighbourPt);

    ComparatorPricierPoint cpp;
    std::queue<Point> v;

    while (!_unknown.empty()) {

        if (hasSafe() && cpp(_unknown.top(), _safe.top())) break;

        Point pt = _unknown.top();
        _unknown.pop();
        while (!_unknown.empty() && pt == _unknown.top()) _unknown.pop();
        v.push(pt);
    }

    if (at<Glow>())
        foreach(neighbourPt, neighbours)
            if (deduceAndAdd(true, Teleport, neighbourPt)) {
                printMe("teleport move");
                moveTo(neighbourPt);
                return;
            }

    if (at<Stench>())
    {
        ntimes(v.size())
        {
            Point pt = v.back();
            v.pop();
            if (deduceAndAdd(true, Wumpus, pt)) {
                wumpusFound();
                break;
            }
            v.push(pt);
        }
    }



    while (!v.empty()) {
        Point pt = v.front();

        v.pop();

        if (deduceAndAdd(false, Pit, pt) && deduceAndAdd(false, Wumpus, pt)) {
            addSafe(pt);
            continue;
        }
        addUnknown(pt);
    }
    if (!hasSafe()) {
        while (hasUnknown()) {
            auto pt = _unknown.top();
            if (!deduceAndAdd(true, Pit, pt) && !deduceAndAdd(true, Wumpus, pt))
                break;
            _unknown.pop();
        }
    }

    move();
}

void Game::moveTo(Point const &point) {
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
    while (!_unknown.empty() && _unknown.top() == p) _unknown.pop();
    return p;
}

void Game::printVisited() {
    _board.print(_visited);
}

Point Game::nextSafe() {

    Point p(_safe.top());
    _safe.pop();
    while (!_safe.empty() && _safe.top() == p) _safe.pop();
    return p;
}

bool Game::ended() {

    std::cout << "AT: ";
    _position.print();
    endline();
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