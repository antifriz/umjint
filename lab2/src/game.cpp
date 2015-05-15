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
    seen.insert(_position);
    _wumpusFound = false;
}

/*void Game::initialPremisesAbout(Point p) {
    // todo: if not seen as adjacent

    createRuleForNeighbours<true, Stench, true, Wumpus, Op::Or>(p);
    createRuleForNeighbours<false, Stench, false, Wumpus, Op::And>(p);
    createRuleForNeighbours<true, Breeze, true, Pit, Op::Or>(p);
    createRuleForNeighbours<false, Breeze, false, Pit, Op::And>(p);
    createRuleForNeighbours<true, Glow, true, Teleport, Op::Or>(p);
    createRuleForNeighbours<false, Glow, false, Teleport, Op::And>(p);
}*/

void Game::run() {

    while (!ended()) {
        step();
    }
}


bool Game::deduceAndAdd(bool prefix, Property property, Point point) {
    return _knowledgeBase.deduceAndAdd(Literal(prefix, Atom(property, point)));
}
std::vector<Point> Game:: getNewNeighbours() {
    auto &&neighboursProposed = _board.getNeighbours(_position);
    std::vector<Point> neighboursNew;
    foreach(neighbour, neighboursProposed)
    if (seen.find(neighbour) == seen.end()){
        neighboursNew.push_back(neighbour);
        seen.insert(neighbour);
    }
    return neighboursNew;
}

void Game::step() {

    visited.insert(_position);

    auto &&neighbours = getNewNeighbours();

    addPositionUnaryPremise<false,Pit>();
    addPositionUnaryPremise<false,Teleport>();
    addPositionUnaryPremise<false,Wumpus>();

    addNeighbouringPresumption<Teleport>(at<Glow>());
    addNeighbouringPresumption<Pit>(at<Breeze>());
    addNeighbouringPresumption<Wumpus>( at<Stench>());

    if (at<Glow>())
        foreach(neighbourPt, neighbours)
            if (deduceAndAdd(true, Teleport, neighbourPt)) {
                printMe("teleport move");
                moveTo(neighbourPt);
                return;
            }

    foreach(neighbourPt, neighbours)addUnknown(neighbourPt);

    auto tmpUnknown(_unknown);
    while(!_unknown.empty())_unknown.pop();



    while (!tmpUnknown.empty()) {
        auto pt = tmpUnknown.top();
        tmpUnknown.pop();

        if (deduceAndAdd(false, Pit, pt) && deduceAndAdd(false, Wumpus, pt)) {
            addSafe(pt);
        }
        else if (!deduceAndAdd(true, Pit, pt) && !deduceAndAdd(true, Wumpus, pt))
            addUnknown(pt);
    }


    move();
}

void Game::moveTo(Point const &point) {
    _position = point;

}

void Game::move() {
    printMe("safe");
    printSet(_safe);
    printMe("unknown");
    printSet(_unknown);
    endline();endline();

    if (hasSafe()) {
        printMe("safe move");
        moveTo(nextSafe());

    } else if (hasUnknown()) {
        printMe("unknown move");

        moveTo(nextUnknown());

    } else {
        printMe("predah se");
        printVisited();
        exit(0);
    }

    printVisited();

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
    _board.print(visited,_position);
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
