//
//  puzzle.hpp
//  xword
//
//  Created by Jonathan Hendrix on 12/19/15.
//  Copyright © 2015 Jonathan Hendrix. All rights reserved.
//

#ifndef puzzle_hpp
#define puzzle_hpp

#include "answer.h"
#include "grid.h"
#include <vector>
#include <algorithm>
#include <utility>
#include <fstream>
#include <exception>
#include <istream>

#include <iostream>

class puzzle
{
public:
    static bool ut();
    
    explicit puzzle(const std::vector<answer> answers)
    : grid_{answers}, answers_(answers)
    {
    }
    
    std::vector<std::string> render() const {
        return grid_.render();
    }
    
    void render(std::ostream& out) const {
        auto s = render();
        for (const auto& se : s) {
            out << se << std::endl;
        }
    }
    
    // Return the resultant answer list if the input
    // answer is added. If the list is empty, then the
    // input answer conflicted with the current state of
    // the puzzle. The resultant answer list will be
    // normalized so that the upper left is (0,0)
    std::vector<answer> place(const answer& ans) const
    {
        std::vector<answer> result;
        if (grid_.can_place(ans))
        {
            auto newpuzzle = puzzle{answers_};
            newpuzzle.overlay(ans);
            newpuzzle.fetch_across(result);
            newpuzzle.fetch_down(result);
            mark_new_answers(answers_, result);
        }
        return result;
    }
    
    std::vector<answer> find_places(const std::string& word) const
    {
        return grid_.find_places(word);
        // ToDo: handle translation in the place function
//        auto result = grid_.find_places(word);
//        for (auto r : result) {
//            std::cout << r.to_string() << std::endl;
//        }
//        auto extremes = find_upper_left_limits(result);
//        std::cout << std::get<0>(extremes) << " " << std::get<1>(extremes) << std::endl;
//        return ::translate(-std::get<0>(extremes), -std::get<1>(extremes), result);
    }
    
private:
    grid grid_;
    std::vector<answer> answers_;
    
    void overlay(const answer& answer)
    {
        grid_.overlay(answer);
    }
    
    void overlay(const std::vector<answer> answers)
    {
        for (const auto& answer : answers)
        {
            overlay(answer);
        }
    }
    
    void fetch_across(std::vector<answer>& ans) const
    {
        const auto rows = grid_.get_rows();
        const auto last_row = grid_.rows();
        for (auto down = int{0}; down < last_row; ++down) {
            add_words_found(down, rows[down], direction::ACROSS, ans);
        }
    }
    
    void fetch_down(std::vector<answer>& ans) const
    {
        const auto cols = grid_.get_columns();
        const auto last_col = grid_.columns();
        for (auto across = int{0}; across < last_col; ++across) {
            add_words_found(across, cols[across], direction::DOWN, ans);
        }
    }
};

inline puzzle load_puzzle(const std::string& filename)
{
    std::ifstream in{filename};
    if (!in) {
        throw std::invalid_argument(std::string{"Could not open: " + filename});
    }
    
    std::vector<answer> ans;
    auto line = std::string{};
    
    while (std::getline(in, line)) {
        if (!line.empty() && !(line[0] == '#')) {
            ans.push_back(answer{line});
        }
    }
    return puzzle{ans};
}

#endif /* puzzle_hpp */
