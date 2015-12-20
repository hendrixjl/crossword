//
//  main.cpp
//  xword
//
//  Created by Jonathan Hendrix on 12/7/15.
//  Copyright © 2015 Jonathan Hendrix. All rights reserved.
//

#include "answer.h"
#include "grid.h"
//#include "puzzle.h"
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

    if (!grid::ut()) {
        cout << "grid::uit() failed\n";
        return 1;
    }
    
//    if (!puzzle::ut()) {
//        cout << "puzzle::ut() failed\n";
//        return 1;
//    }
    
//    try {
//    
//        auto mypuz = load_puzzle("words.txt");
//
//        auto grid = mypuz.render();
//        
//        for (const auto& l : grid) {
//            cout << l << endl;
//        }
//
//    }
//    catch (exception& e) {
//        cout << "Exception! " << e.what() << endl;
//    }
    return 0;
}
