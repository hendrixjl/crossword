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

bool test_normalize()
{
    auto answers = std::vector<answer>{
        answer{"0 0 A C STARWARS"}
    };
    auto new_answers = normalize_data(answers);
    if (answers[0].to_string() != new_answers[0].to_string()) {
        cout << "Error in ut. translate failed. (" << new_answers[0].to_string() << ") line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    answers = std::vector<answer>{
        answer{"1 2 A C STARWARS"},
        answer{"4 3 D C STARTREK"}
    };
    new_answers = normalize_data(answers);
    if ("0 0 A C STARWARS" != new_answers[0].to_string()) {
        cout << "Error in ut. translate failed. (" << new_answers[0].to_string() << ") line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if ("3 1 D C STARTREK" != new_answers[1].to_string()) {
        cout << "Error in ut. translate failed. (" << new_answers[1].to_string() << ") line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    answers = std::vector<answer>{
        answer{"-1 2 A C STARWARS"},
        answer{"4 3 D C STARTREK"}
    };
    new_answers = normalize_data(answers);
    if ("0 0 A C STARWARS" != new_answers[0].to_string()) {
        cout << "Error in ut. translate failed. (" << new_answers[0].to_string() << ") line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if ("5 1 D C STARTREK" != new_answers[1].to_string()) {
        cout << "Error in ut. translate failed. (" << new_answers[1].to_string() << ") line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    auto prop_ans = answer{"-1 -2 D I BOB"};
    auto trans_ans = prop_ans;
    auto newer_answers = std::vector<answer>{};
    normalize_data(new_answers, prop_ans, newer_answers, trans_ans);
    if ("1 2 A C STARWARS" != newer_answers[0].to_string()) {
        cout << "Error in ut. translate failed. (" << newer_answers[0].to_string() << ") line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if ("6 3 D C STARTREK" != newer_answers[1].to_string()) {
        cout << "Error in ut. translate failed. (" << newer_answers[1].to_string() << ") line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if ("0 0 D I BOB" != trans_ans.to_string()) {
        cout << "Error in ut. translate failed. (" << trans_ans.to_string() << ") line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    
    return true;
}

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
    auto start_answers = std::vector<answer>{answer{"4 4 A C AMAZIAH"}};
    
    auto mypuz = puzzle{start_answers};
    
    auto new_answers = mypuz.place(answer{"4 3 D C YEA"});
    if (!new_answers.empty()) {
        cout << "Error in ut. Adding should have failed." << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    new_answers = mypuz.place(answer{"4 2 D I YEA"});
    if (new_answers.empty()) {
        cout << "Error in ut. Adding should not have failed." << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    auto compare_to = vector<answer>{
        answer{"0 2 A C AMAZIAH"},
        answer{"0 0 D I YEA"}
    };
    
    mypuz = puzzle{new_answers};

    new_answers = mypuz.place(answer{"-4 1 A I HAPPY"});
    if (!new_answers.empty()) {
        cout << "Error in ut. Adding should have failed." << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    new_answers = mypuz.place(answer{"-4 0 A I HAPPY"});
    if (new_answers.empty()) {
        cout << "Error in ut. Adding should not have failed." << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    mypuz = puzzle{new_answers};

    compare_to = vector<answer>{
        answer{"4 2 A C AMAZIAH"},
        answer{"4 0 D I YEA"},
        answer{"0 0 A I HAPPY"}
    };
    
    
    auto delta = delta_list(compare_to, new_answers);
    
    if (delta.size() != 0) {
        cout << "Error in ut. delta.size()=" << delta.size() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    return true;
}

bool test_puzzle_find_places()
{
    auto start_answers = std::vector<answer>{answer{"4 4 A C AMAZIAH"},
        answer{"4 2 D C YEA"},
        answer{"6 3 D C MAGOG"}};
    
    auto mypuz = puzzle{start_answers};
    
    auto new_answers = mypuz.find_places("HAPPY");
    if (new_answers.empty()) {
        cout << "Error in ut. should not have failed." << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    auto compare_to = std::vector<answer>{
        answer{"4 -2 D I HAPPY"}, answer{"9 3 D I HAPPY"}, answer{"10 4 D I HAPPY"},
        answer{"0 2 A I HAPPY"}, answer{"10 4 A I HAPPY"}
    };
    
    if (!is_permutation(new_answers.cbegin(), new_answers.cend(), compare_to.cbegin())) {
        cout << "Expected permuation equal.\n";
        cout << " Was:\n";
        for (const auto& e : new_answers) {
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
    if (!test_normalize()) {
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
    auto rev = mypuz.reverse_answers();
    auto delta = delta_list(start_answers, rev);
    if (!delta.empty()) {
        cout << "Reversing failed.\n";
        for (const auto d : delta) {
            cout << d.to_string() << endl;
        }
        return false;
    }
    
    if (!test_puzzle_place()) {
        return false;
    }
    
    if (!test_puzzle_find_places()) {
        return false;
    }
    
    return true;
}

