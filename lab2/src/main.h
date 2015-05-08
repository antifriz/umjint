//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H


#define existsInContainer(container, value) ((container).find(value) != (container).end())
#define printMe(str) std::cout<<str<<std::endl
#define endline() std::cout<<std::endl
#define foreach(item, container) for(auto &item: container)
#define foreachCpy(item, container) for(auto item: container)
#define ntimesVar(var, n) for(int (var) = 0; (var)<n;(var)++)
#define ntimesi(n) ntimesVar(i,n)
#define ntimesj(n) ntimesVar(j,n)
#define ntimesk(n) ntimesVar(k,n)
#define ntimesx(n) ntimesVar(x,n)
#define ntimesy(n) ntimesVar(y,n)
#define ntimesz(n) ntimesVar(z,n)
#define ntimes(n) ntimesi(n)
#define iterate(container) for(auto it = (container).begin();it!=(container).end();it++)
#define iterate2(container) for(auto it2 = (container).begin();it2!=(container).end();it2++)

#define printVec(container) printMe("container{");foreach(item,container){printMe(item);}printMe("}")
enum Property { Glow, Breeze, Stench, Teleport, Pit, Wumpus };


#endif //PROJECT_MAIN_H
