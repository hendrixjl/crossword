//
//  main.cpp
//  xword
//
//  Created by Jonathan Hendrix on 12/7/15.
//  Copyright © 2015 Jonathan Hendrix. All rights reserved.
//

#include "answer.h"
#include "grid.h"
#include "puzzle.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
//#include <cstdlib>
using namespace std;

int main(int argc, const char * argv[]) {
    
    if (!answer::ut()) {
        return false;
    }

    
    if (!puzzle::ut()) {
        cout << "puzzle::ut() failed\n";
        return 1;
    }

    ifstream in{"words.txt"};
    if (!in) {
        cout << "could not open gdata.txt\n";
        return 1;
    }
    
    vector<answer> ans;
    auto line = string{};
    
    while (getline(in, line)) {
        if (!line.empty() && !(line[0] == '#')) {
            ans.push_back(answer{line});
        }
    }
    
    for (const auto& a : ans) {
        cout << a.to_string() << endl;
    }

    auto mypuz = puzzle{ans};
    
    auto grid = mypuz.render();
    
       for (const auto& l : grid) {
           cout << l << endl;
       }

    return 0;
}
