//
// Created by ivan on 5/6/15.
//

#include <string>
#include <vector>

class Point {
public:
    Point(const int &x, int T &

    y) :

    x(x), y(y) { }

    const int x;
    const int y;

    bool operator<(const Point &other) {
        if (x < other.x) return true;
        if (other.x < x) return false;
        return y < other.y;
    }
};

class FieldData {
    bool isPit;
    bool isBreeze;

    bool isWumpus;
    bool isStench;

    bool isTeleport;
    bool isGlow;
};

class Board {
private:
    std::vector _fields;

public:
    Board(std::string path) {

    }
};

enum Atom { Wumpus, Teleport, Pit };

class Literal {
private:
    Atom _label;
    bool _prefix;
    Point _point;
public:

    Literal(Atom &_label, bool _prefix, Point const &_point) : _label(_label), _prefix(_prefix), _point(_point) { }

    Literal(const Literal &other) : _label(other._label), _prefix(other._prefix), _point(other._point) {
    };

    inline void togglePrefix() { _prefix ^= 1; }

    Literal getNegated() const {
        Literal copied(*this);
        copied.togglePrefix();
        return copied;
    }

    bool operator<(const Literal &other) {
        if (_point < other._point) return true;
        if (other._point < _point) return false;
        if (_label < other._label) return true;
        if (other._label < _label) return false;
        return _prefix < other._prefix;
    }
};

#include <set>
#include <ostream>
#include <iostream>


#define existsInContainer(container, value) ((container).find(value) != (container).end())


class Clause {
private:
    bool _isTautology = false;

    std::set<Literal> _literalSet;


    Clause() { }

public:


    static Clause Create(Atom &_label, bool _prefix, Point const &_point) {
        return Clause::Create(Literal(_label, _prefix, _point));
    }

    template<typename... Args>
    static Clause Create(Atom &_label, bool _prefix, Point const &_point, Args... args) {
        auto clause = Create(args...);
        clause.addLiteral(Literal(_label, _prefix, _point));
        return clause;
    }


    static Clause Create(Literal l) {
        Clause clause;
        clause.addLiteral(l);
        return clause;
    }

    template<typename... Args>
    static Clause Create(Literal first, Args... args) {
        auto clause = Create(args...);
        clause.addLiteral(first);
        return clause;
    }

    void addLiteral(const Literal &literal) {
        if (existsInContainer(_literalSet, literal.getNegated()))
            _isTautology = true;
        _literalSet.insert(literal);
    }

    bool operator<(const Clause &other) {
        auto it1 = this->_literalSet.begin();
        auto it2 = other._literalSet.begin();
        for (; it1 != this->_literalSet.end() && it2 != other._literalSet.end(); it1++, it2++) {
            if (*it1 < *it2) return true;
            if (*it2 < *it1) return false;
        }
        return false;
    }


    bool isTautology() {
        return _isTautology;
    }

    bool hasLessLiteralsThan(const Clause &other) const {
        return this->_literalSet.size() < other._literalSet.size();
    }
};


class KnowledgeBase {
private:
    std::set<Clause> _clauseSet;
public:
    KnowledgeBase() { }

    void addClause(const Clause &clause) {
        auto it = _clauseSet.find(clause);

        if (it != _clauseSet.end() && it->hasLessLiteralsThan(clause))
            return;

        _clauseSet.insert(clause);
    }

};


class RefutationResolution {

};

enum Property { Glow, Breeze, Stench, Teleport, Pit, Wumpus, Safe };

#define print(str) std::cout<<str<<std::endl

class Game {
private:
    Board board;
    Point position;
    KnowledgeBase knowledgeBase;
public:
    Game(std::string path) : board(path), position(1, 1) {
    }

    void run() {
        while (!ended()) {
            addPositionUnaryPresumption<Atom::Pit, false>();
            addPositionUnaryPresumption<Atom::Teleport, false>();
            addPositionUnaryPresumption<Atom::Wumpus, false>();

            if (at<Glow>()) addNeighbouringPresumption<Atom::Teleport, false>();
            if (at<Breeze>()) addNeighbouringPresumption<Atom::Pit, false>();
            if (at<Stench>()) addNeighbouringPresumption<Atom::Wumpus, false>();

            if (at<Safe>()) updateSafe();

        }
    }

    void updateSafe() {
        board.addSafe(position);
    }


    template<Atom atom, bool prefix>
    void addPositionUnaryPresumption() {
        knowledgeBase.addClause(Clause::Create(atom, prefix, position));
    }

    template<Atom atom, bool prefix>
    void addNeighbouringPresumption() {
        for (auto &neighbour : board.getNeighbours(position))
            knowledgeBase.addClause(Clause::Create(atom, prefix, neighbour));
    }


    template<Property property>
    bool at() {
        board.is<property>(position);
    }

    template<>
    bool at<Safe>() {
        return !at<Breeze>() && !at<Stench>();
    }


    bool ended() {
        if (at<Teleport>()) {
            print("Pikard se spasio, pronasao je teleporter :)");
            return true;
        }
        if (at<Wumpus>()) {
            print("Pikarda je ubio Wumpus :(, tko god to bio ?!");
            return true;
        }
        if (at<Pit>()) {
            print("Pikard je pao u jamu i umro :(");
            return true;
        }
        return false;
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;

    Game game(argv[1]);
    game.run();
}