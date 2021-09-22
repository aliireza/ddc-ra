/*
 *  Authors:
 *    Alireza Farshin <farshin@kth.se>
 *    Amir Roozbeh    <amirrsk@kth.se>
 *
 */

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace Gecode;

typedef struct Tile {
    bool occupied;
    int x;
    int y;
    Tile(int _x, int _y) {
        x = _x;
        y = _y;
        occupied = false;
    }
    Tile(int _x, int _y, bool _oc) {
        x = _x;
        y = _y;
        occupied = _oc;
    }
} Tile;

/* Topology definition */
int col = 7;
int row = 8;
int requiredCores = 4;

std::vector<std::vector<Tile*>> mesh;//(row, std::vector<Tile*>(col));


/* Constraint Solver class */
class CPUAlloc : public Space {


public:

    /* Processor Constants */

    int CPUx = col;
    int CPUy = row;
    int nCPUPool = CPUx * CPUy;



    /* Application Constants */
    int appCores = requiredCores;

    /* Variables */

    IntVarArray CoresX;
    IntVarArray CoresY;
    IntVar distance;

    /* Constructor */
    CPUAlloc(void)
        : CoresX(*this, appCores, 0, CPUx - 1),
          CoresY(*this, appCores, 0, CPUy - 1),
          distance(*this, 0, nCPUPool * appCores)
    {

        /* Constraints */

        /* Fill-in Pre-defined Fields */
        for (int i = 0; i < CPUx; i++) {
            for (int j = 0; j < CPUy; j++) {
                if (int preDefinedValue = mesh[j][i]->occupied) {
                    for (int k = 0; k < appCores; k++) {
                        rel(*this, (
                                ( (CoresX[k] == i) && (CoresY[k] != j) ) ||
                                ( (CoresX[k] != i) && (CoresY[k] == j) ) ||
                                ( (CoresX[k] != i) && (CoresY[k] != j) )
                            ) );
                    }
                }
            }
        }


        int nPairs = 0; /* Calculate number of pairs */
        /* Either X or Y should be different */
        for (int i = 0; i < appCores; i++) {
            for (int j = i + 1; j < appCores; j++) {
                rel(*this, ( (CoresX[i] != CoresX[j]) || (CoresY[i] != CoresY[j]) ) );
                nPairs++;
            }
        }
        /* Calculate distance between every pair */
        IntVarArray dis(*this, nPairs, 0, nCPUPool * appCores);
        int k = 0;
        for (int i = 0; i < appCores; i++) {
            for (int j = i + 1; j < appCores; j++) {
                /* Assume that we have a torus */
                rel(*this, (min(CPUx - abs(CoresX[i] - CoresX[j]), abs(CoresX[i] - CoresX[j])) + min(CPUy - abs(CoresY[i] - CoresY[j]), abs(CoresY[i] - CoresY[j]))) == dis[k]);
                k++;
            }
        }

        /* Calculate neighboring distance */
        linear(*this, dis, IRT_EQ, distance);

        // rel(*this, distance <= (appCores -1)*(appCores -1) );

        /* Symmetry breaking */
        for(int i=0; i< appCores -1; i++) {
            rel(*this, (CoresX[i] >= CoresX[i+1]) || (CoresY[i] >= CoresY[i+1]));
        }
        //rel(*this, (CoresX[0] >= CoresX[1]) || (CoresY[0] >= CoresY[1]));


        /* Branching */
        branch(*this, CoresX, INT_VAR_NONE(), INT_VAL_MAX());
        branch(*this, CoresY, INT_VAR_ACTION_MAX(), INT_VAL_MAX());

    }


    /* Cost Function - Minimize Price and inherently Maximize Penalty*/
    // virtual IntVar cost(void) const
    // {
    //   return distance;
    // }

    /* Cloning Constructor */
    CPUAlloc(CPUAlloc& S)
        : Space(S) {
        CoresX.update(*this, S.CoresX);
        CoresY.update(*this, S.CoresY);
        distance.update(*this, S.distance);
    }

    /* Copying */
    virtual Space*
    copy(void) {
        return new CPUAlloc(*this);
    }

    void constrain(const Space &_s) {
        const CPUAlloc& s = static_cast<const CPUAlloc&>(_s);

        int current_distance = 0;
        for (int i = 0; i < s.appCores; i++) {
            for (int j = i + 1; j < s.appCores; j++) {
                current_distance += (std::min(s.CPUx - std::abs(s.CoresX[i].val() - s.CoresX[j].val()), std::abs(s.CoresX[i].val() - s.CoresX[j].val())) + std::min(s.CPUy - std::abs(s.CoresY[i].val() - s.CoresY[j].val()), std::abs(s.CoresY[i].val() - s.CoresY[j].val())));
            }
        }

        rel(*this, distance < current_distance);
    }

    /* Printing */
    virtual void
    print(void) const {
        std::cout << "Solution: " << std::endl;
        for (int i = 0; i < appCores; i++) {
            std::cout << CoresX[i] << " " << CoresY[i] << "\n";
        }
        std::cout << "Distance: " << distance << "\n";
    }
};

/* Main */
int main(int argc, char* argv[]) {


    if (argc != 4) {
        printf("Wrong Input! Enter mesh dimensions and the number of cores!\n");
        printf("Enter: %s <row> <column> <#cores>\n", argv[0]);
        exit(1);
    }

    sscanf (argv[1], "%d", &row);
    if (row <= 2) {
        printf("Wrong row! It should be larger than 2!\n");
        exit(1);
    }


    sscanf (argv[2], "%d", &col);
    if (col <= 2) {
        printf("Wrong column! It should be larger than 2!\n");
        exit(1);
    }

    sscanf (argv[3], "%d", &requiredCores);
    if (requiredCores < 2) {
        printf("Wrong number of cores! It should be larger than 2!\n");
        exit(1);
    }

    //mesh.resize(row, std::vector<Tile*>(col));


    std::srand(std::time(nullptr)); // use current time as seed for random generator
    /* Create Mesh */
    for (int i = 0; i < row; i++) {
        std::vector<Tile*> v;
        for (int j = 0; j < col; j++) {
            //Tile *n = new Tile(i, j, (bool)(std::rand() % 2));
            //Tile *n = new Tile(i,j,0);

            //Tile *n = new Tile(j, i, (bool)(std::rand() % 2));
            Tile *n = new Tile(j, i, 0);
            v.push_back(n);
            //mesh[j][i] = n;
        }
        mesh.push_back(v);
    }

    std::cout << "Size: " << mesh.size() * mesh[0].size() << std::endl;
    /* Print Mesh */
    std::cout << "Pre-solving Mesh" << std::endl;
    for (int i = row - 1; i >= 0; i--) {
        for (int j = 0; j < col; j++) {
            std::cout << mesh[i][j]->occupied << " ";
        }
        std::cout << std::endl;
    }

    clock_t begin = clock();
    /* Solve CPU allocation problem */
    CPUAlloc* problem = new CPUAlloc();
    BAB<CPUAlloc> e(problem);
    delete problem;
    std::vector<CPUAlloc*> sol;
    CPUAlloc* optimal_solution;
    /* Save solutions */
    while (CPUAlloc* s = e.next()) {
        //sol.push_back(s);
        optimal_solution = s;
    }

    /* Print the optimal solution */
    if (optimal_solution != nullptr) {
        std::cout << "Solution <x,y>: " << std::endl;
        for (int i = 0; i < optimal_solution->appCores; i++) {
            std::cout << "<" << optimal_solution->CoresX[i].val() << "," << optimal_solution->CoresY[i].val() << ">" << std::endl;
            mesh[optimal_solution->CoresY[i].val()][optimal_solution->CoresX[i].val()]->occupied = 1;
        }
        std::cout << "Distance: " << optimal_solution->distance << std::endl;

        /* Erase solutions */
        for (auto itr = sol.begin();
                itr != sol.end(); itr++) {
            delete (*itr);
            sol.erase(itr);
        }
    } else {
        std::cout << "No Solution!" << std::endl;
    }

    clock_t end = clock();
    double duration = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Time: " << duration << std::endl;

    std::cout << "Post-solving Mesh" << std::endl;
    /* Print Mesh */
    for (int i = row - 1; i >= 0; i--) {
        for (int j = 0; j < col; j++) {
            std::cout << mesh[i][j]->occupied << " ";
        }
        std::cout << std::endl;
    }

    /* Delete mesh */
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            delete mesh[i][j];
        }
    }
    mesh.clear();
    return 0;
}
