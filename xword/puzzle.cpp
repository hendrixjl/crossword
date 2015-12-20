//
//  puzzle.cpp
//  xword
//
//  Created by Jonathan Hendrix on 12/19/15.
//  Copyright © 2015 Jonathan Hendrix. All rights reserved.
//

#include "puzzle.h"
#include "answer.h"

#include <vector>
#include <iostream>

using namespace std;

std::vector<answer> delta_list(std::vector<answer> one, std::vector<answer> two)
{
    auto result = std::vector<answer>{};
    for (const auto& ans : one) {
        if (two.cend() == find(two.cbegin(), two.cend(), ans)) {
            result.push_back(ans);
        }
    }
    
    for (const auto& ans : two) {
        if (one.cend() == find(one.cbegin(), one.cend(), ans)) {
            result.push_back(ans);
        }
    }
    return result;
}

bool test_puzzle_place()
{
    auto start_answers = std::vector<answer>{answer{"4 4 A C AMAZIAH"},
        answer{"4 2 D C YEA"},
        answer{"6 3 D C MAGOG"}};
    
    auto mypuz = puzzle{start_answers};
//    mypuz.render(cout);

    auto new_answers = mypuz.place(answer{"2 2 D I HAPPY"});
    if (!new_answers.empty()) {
        cout << "Error in ut. Adding should have failed." << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    new_answers = mypuz.place(answer{"0 2 A I HAPPY"});
    if (new_answers.empty()) {
        cout << "Error in ut. Adding should not have failed." << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
//    cout << endl;
//    for (const auto a : new_answers) {
//        cout << a.to_string() << endl;
//    }
    
    auto delta = delta_list(start_answers, new_answers);
    
    if (delta.size() != 1) {
        cout << "Error in ut. delta.size()=" << delta.size() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    if (delta[0].to_string() != "0 2 A I HAPPY") {
        cout << "Error in ut. delta.size()=" << delta.size() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    new_answers = mypuz.place(answer{"7 4 D I ZOO"});
    
    if (new_answers.empty()) {
        cout << "Error in ut. Adding should not have failed." << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
//    cout << endl;
//    for (const auto a : new_answers) {
//        cout << a.to_string() << endl;
//    }
    
    delta = delta_list(start_answers, new_answers);
    
    auto compare_to = std::vector<answer>{
        answer{"7 4 D I ZOO"}, answer{"6 5 A I GO"}, answer{"6 6 A I OO"}};
    
    if (!is_permutation(delta.cbegin(), delta.cend(), compare_to.cbegin())) {
        cout << "Expected permuation equal.\n";
        cout << " Was:\n";
        for (const auto& e : delta) {
            cout << e.to_string() << ", ";
        }
        cout << "\n Expected:\n";
        for (const auto& e : compare_to) {
            cout << e.to_string() << ", ";
        }
        return false;
    }
    return true;
}


bool puzzle::ut()
{
    
    auto start_answers = std::vector<answer>{answer{"2 4 A C AMAZIAH"},
        answer{"1 7 A C AZOG"},
        answer{"8 8 A C BRAVE"},
        answer{"2 2 D C YEA"},
        answer{"4 3 D C MAGOG"},
        answer{"8 4 D I HOREB"},
        answer{"11 8 D C VERILY"}};
    
    auto mypuz = puzzle{start_answers};
    auto rev = std::vector<answer>{};
    mypuz.fetch_across(rev);
    mypuz.fetch_down(rev);
    mark_new_answers(start_answers, rev);
    
    //    cout << "Reverse list.\n";
    //    for (const auto& ans : rev) {
    //        cout << ans.to_string() << '\n';
    //    }
    //
    auto delta = delta_list(start_answers, rev);
    if (!delta.empty()) {
        cout << "Reversing failed.\n";
        return false;
    }
    
    if (!test_puzzle_place()) {
        return false;
    }
    
    return true;
}

