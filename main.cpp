#include "main.h"

#include "restaurant.cpp"

int main(int argc, char* argv[]) {
    restaurant* r = new restaurant();
    string fileName = "reg_test.txt";

    // initialize table with ID, name and age
    for (int i = 1; i <= MAXSIZE; i++) {
        r->recentTable = r->insert(r->recentTable, i, "", 0);
    }

    simulate(fileName, r);

    /*table* current = r->recentTable->next;
    string currentName = current->name;
    int count = 0;
    do {
        cout << current->ID << " " << current->name << " " << current->next->ID << endl;
        if (currentName == "") currentName = current->name;
        current = current->next;
    } while (currentName != current->name);*/

    delete r;

    return 0;
}
