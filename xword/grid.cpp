//
//  grid.cpp
//  xword
//
//  Created by Jonathan Hendrix on 12/14/15.
//  Copyright © 2015 Jonathan Hendrix. All rights reserved.
//

#include "grid.h"
#include <iostream>

using namespace std;


bool test_grid_can_place()
{
    auto word = std::string{"BOB"};
    auto slice = std::string{" A B B C B"};
    // only slice position 3 and 9 should succede.
    for (auto slice_loc = size_t{}; slice_loc<slice.size(); ++slice_loc) {
        if ((slice_loc == 3) || (slice_loc == 9)) {
            if (!can_place(word, slice, slice_loc)) {
                cout << "Error in ut. slice_loc=" << slice_loc << " line=" << __LINE__ << " file=" << __FILE__ << endl;
                return false;
            }
        }
        else {
            if (can_place(word, slice, slice_loc)) {
                cout << "Error in ut. slice_loc=" << slice_loc << " line=" << __LINE__ << " file=" << __FILE__ << endl;
                return false;
            }
        }
    }
    return true;
}

bool test_look_before()
{
    auto word = std::string{"BOB"};
    auto slice = std::string{"I "};
    auto res = look_before(word, slice);
    if (!res.empty()) {
        cout << "Error in ut. Found spots for BOB in I " << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    slice = std::string{"O "};
    res = look_before(word, slice);
    if (res.empty()) {
        cout << "Error in ut. Didn't find a spot for BOB in O " << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if (res.size() != 1) {
        cout << "Error in ut. Too many spots for BOB in O " << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if (res[0] != -1) {
        cout << "Error in ut. BOB in O not at -1 " << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    slice = std::string{"B "};
    res = look_before(word, slice);
    if (res.empty()) {
        cout << "Error in ut. Didn't find a spot for BOB in B " << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if (res.size() != 1) {
        cout << "Error in ut. Too many spots for BOB in B " << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if (res[0] != -2) {
        cout << "Error in ut. BOB in B not at -2 " << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    return true;
}

bool test_find_places()
{
    auto word = std::string{"BOB"};
    auto slice = std::string{"B A B B C B"};
    auto places = find_places(word, slice);
    if (places.size() != 3) {
        cout << "Error in ut. places.size()=" << places.size() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if (places[0] != -2) {
        cout << "Error in ut. places[0]=" << places[0] << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if (places[1] != 4) {
        cout << "Error in ut. places[0]=" << places[0] << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    if (places[2] != 10) {
        cout << "Error in ut. places[1]=" << places[1] << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    return true;
}

bool test_grid_find_places()
{
    auto mygrid = grid{};
    mygrid.resize(make_pair(10,10));
    mygrid.overlay(answer{"HELLO", make_pair(0,0), direction::ACROSS});
    
    auto word = std::string{"HOE"};
    auto ans = mygrid.find_places(word);
    
    if (ans.empty()) {
        cout << "Error in ut. grid couldn't place " << word << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    if (ans.size()  != 3) {
        cout << "Error in ut. wrong count on placing " << word << " size=" << ans.size() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    if (ans[0].to_string() != "0 0 D I HOE") {
        cout << "Error in ut. ans[0]=" << ans[0].to_string() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    if (ans[1].to_string() != "1 -2 D I HOE") {
        cout << "Error in ut. ans[1]=" << ans[1].to_string() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    if (ans[2].to_string() != "4 -1 D I HOE") {
        cout << "Error in ut. ans[2]=" << ans[2].to_string() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    mygrid = grid{};
    mygrid.resize(make_pair(10,10));
    mygrid.overlay(answer{"HELLO", make_pair(0,0), direction::DOWN});
    
    word = std::string{"POSH"};
    ans = mygrid.find_places(word);
    
    if (ans.empty()) {
        cout << "Error in ut. grid couldn't place " << word << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    if (ans.size()  != 3) {
        cout << "Error in ut. wrong count on placing " << word << " size=" << ans.size() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    if (ans[0].to_string() != "0 -3 D I POSH") { // Todo: If new word is formed, shouldn't it be subsumed?
        cout << "Error in ut. ans[0]=" << ans[0].to_string() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    if (ans[1].to_string() != "-3 0 D I POSH") {
        cout << "Error in ut. ans[1]=" << ans[1].to_string() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    if (ans[2].to_string() != "-1 4 D I POSH") {
        cout << "Error in ut. ans[2]=" << ans[2].to_string() << " line=" << __LINE__ << " file=" << __FILE__ << endl;
        return false;
    }
    
    return true;
}

bool test_can_place()
{
    auto mygrid = grid{};
    mygrid.resize(make_pair(10,10));
    mygrid.overlay(answer{"HELLO", make_pair(0,0), direction::ACROSS});
    
    auto ans = answer{"HOE", make_pair(0,0), direction::DOWN};
    if (!mygrid.can_place(ans)) {
        cout << "Error in ut. couldn't place answer=" << ans.to_string() << " line=" << __LINE__ <<
        " file=" << __FILE__ << endl;
    }
    ans = answer{"HOE", make_pair(1,0), direction::DOWN};
    if (mygrid.can_place(ans)) {
        cout << "Error in ut. could place answer=" << ans.to_string() << " line=" << __LINE__ <<
        " file=" << __FILE__ << endl;
    }
    ans = answer{"HOE", make_pair(1,-2), direction::DOWN};
    if (!mygrid.can_place(ans)) {
        cout << "Error in ut. couldn't place answer=" << ans.to_string() << " line=" << __LINE__ <<
        " file=" << __FILE__ << endl;
    }
    ans = answer{"HOE", make_pair(2,-1), direction::DOWN};
    if (!mygrid.can_place(ans)) {
        cout << "Error in ut. couldn't place answer=" << ans.to_string() << " line=" << __LINE__ <<
        " file=" << __FILE__ << endl;
    }
    
    mygrid = grid{};
    mygrid.resize(make_pair(10,10));
    mygrid.overlay(answer{"HELLO", make_pair(0,0), direction::DOWN});
    
    ans = answer{"HOE", make_pair(0,0), direction::ACROSS};
    if (!mygrid.can_place(ans)) {
        cout << "Error in ut. couldn't place answer=" << ans.to_string() << " line=" << __LINE__ <<
        " file=" << __FILE__ << endl;
    }
    ans = answer{"HOE", make_pair(0,1), direction::ACROSS};
    if (mygrid.can_place(ans)) {
        cout << "Error in ut. could place answer=" << ans.to_string() << " line=" << __LINE__ <<
        " file=" << __FILE__ << endl;
    }
    ans = answer{"HOE", make_pair(-2,1), direction::ACROSS};
    if (!mygrid.can_place(ans)) {
        cout << "Error in ut. couldn't place answer=" << ans.to_string() << " line=" << __LINE__ <<
        " file=" << __FILE__ << endl;
    }
    ans = answer{"HOE", make_pair(-1,2), direction::ACROSS};
    if (!mygrid.can_place(ans)) {
        cout << "Error in ut. couldn't place answer=" << ans.to_string() << " line=" << __LINE__ <<
        " file=" << __FILE__ << endl;
    }
    return true;
}

bool grid::ut()
{
    if (!test_look_before()) {
        return false;
    }
    if (!test_grid_can_place()) {
        return false;
    }
    if (!test_find_places()) {
        return false;
    }
    if (!test_can_place()) {
        return false;
    }
    if (!test_grid_find_places()) {
        return false;
    }
    return true;
}