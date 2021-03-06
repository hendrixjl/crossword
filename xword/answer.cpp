//
//  answer.cpp
//  xword
//
//  Created by Jonathan Hendrix on 12/9/15.
//  Copyright © 2015 Jonathan Hendrix. All rights reserved.
//

#include "answer.h"
#include <string>
#include <iostream>

using namespace std;

bool answer::ut()
{
    auto a1 = string{"1 2 A I HOWDY"};
    auto an = answer{a1};
    if (a1 != an.to_string()) {
        cout << "Error parsing " << a1 << endl;
        return false;
    }
    
    auto an_c = an.coordinate();
    if ((an_c.first != 1) || (an_c.second != 2)) {
        cout << "Error on first coordinate. an_c.first=" << an_c.first << endl;
        cout << "Error on first coordinate. an_c.second=" << an_c.second << endl;
        return false;
    }
    auto an_lc = an.last_coordinate();
    if ((an_lc.first != 5) || (an_lc.second != 2)) {
        cout << "Error on last coordinate. an_lc.first=" << an_lc.first << endl;
        cout << "Error on last coordinate. an_lc.second=" << an_lc.second << endl;
        return false;
    }
    auto letter = an.letters();
    auto last_letter = letter.at(letter.size()-1);
    if (get<0>(last_letter) != an_lc.first) {
        cout << "error. get<0>(last_letter)=" << get<0>(last_letter) << " an_lc.first=" << an_lc.first << endl;
        return false;
    }
    if (get<1>(last_letter) != an_lc.second) {
        cout << "error. get<1>(last_letter)=" << get<1>(last_letter) << " an_lc.second=" << an_lc.second << endl;
        return false;
    }
    
    a1 = string{"10 21 D C MOSES"};
    an = answer{a1};
    if (a1 != an.to_string()) {
        cout << "Error parsing " << a1 << ": got " << an.to_string() << endl;
        return false;
    }

    auto answers = vector<answer>{};
    answers.push_back(an);
    auto limits = find_limits(answers);
    auto ul_limit = get<UpperLeft>(limits);
    if ((get<0>(ul_limit) != 10) && (get<1>(ul_limit) != 21)) {
        cout << "Error. 0=" << get<0>(ul_limit) << " 1=" << get<1>(ul_limit) << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }

    auto an1 = answer{"-5 -10 A C JOE"};
    answers.push_back(an1);
    ul_limit = get<UpperLeft>(find_limits(answers));
    if ((get<0>(ul_limit) != -5) && (get<1>(ul_limit) != -10)) {
        cout << "Error. 0=" << get<0>(ul_limit) << " 1=" << get<1>(ul_limit) << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    auto an2 = an1.translate(5, 10);
    auto coord = an2.coordinate();
    if ((get<0>(coord) != 0) && (get<1>(coord) != 0)) {
        cout << "Error. 0=" << get<0>(coord) << " 1=" << get<1>(coord) << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    auto tanswers = ::translate(5, 10, answers);
    ul_limit = get<UpperLeft>(find_limits(tanswers));
    if ((get<0>(ul_limit) != 0) && (get<1>(ul_limit) != 0)) {
        cout << "Error. 0=" << get<0>(ul_limit) << " 1=" << get<1>(ul_limit) << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }

    
    return true;
}
