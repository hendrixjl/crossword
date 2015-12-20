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

bool test_puzzle_can_place()
{
    auto start_answers = std::vector<answer>{answer{"2 4 A C AMAZIAH"}};
    auto mypuz = puzzle{start_answers};
    
    if (mypuz.can_place("food"))
    {
        cout << "Food shouldn't fit.\n";
        return false;
    }
    
    if (!mypuz.can_place("ICECYCLE"))
    {
        cout << "Should be able to place ICECYCLE.\n";
        return false;
    }

    return true;
}

bool puzzle::ut()
{
    if (!test_puzzle_can_place())
    {
        cout << "test_can_place() failed.\n";
        return false;
    }
    
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
    
    auto new_answers1 = mypuz.place(answer{"7 12 A I HAPPY"});
    if (!new_answers1.empty()) {
        cout << "Adding conflict failed.\n";
        return false;
    }
    
    auto new_answers2 = mypuz.place(answer{"7 13 A I HAPPY"});
    //        cout << "After adding happy list.\n";
    //        for (const auto& ans : new_answers2) {
    //            cout << ans.to_string() << '\n';
    //        }
    
    delta = delta_list(start_answers, new_answers2);
    //        cout << "After adding happy delta list.\n";
    //        for (const auto& ans : delta) {
    //            cout << ans.to_string() << '\n';
    //        }
    if ((delta.size() != 1) || (delta[0].to_string() != "7 13 A I HAPPY")) {
        cout << "Adding Happy failed.\n";
        return false;
    }
    
    
    return true;
}

