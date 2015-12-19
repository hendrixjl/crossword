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

class puzzle
{
public:
    static bool ut();
    
    explicit puzzle(const std::vector<answer> answers)
    : answers_(answers)
    {
        auto extremes = find_limits(answers);
        grid_.resize(extremes);
        overlay(answers);
    }
    
    std::vector<std::string> render() const {
        return grid_.render();
    }
    
    //    std::vector<answer> retrieve_answers() const
    //    {
    //        std::vector<answer> res;
    //        fetch_across(res);
    //        fetch_down(res);
    //        return res;
    //    }
    
    // Return the resultant answer list if the input
    // answer is added. If the list is empty, then the
    // input answer conflicted with the current state of
    // the puzzle
    std::vector<answer> place(const answer& ans) const
    {
        std::vector<answer> result;
        if (!grid_.conflict(ans))
        {
            auto newpuzzle = puzzle{answers_};
            newpuzzle.overlay(ans);
            newpuzzle.fetch_across(result);
            newpuzzle.fetch_down(result);
            mark_new_answers(answers_, result);
        }
        return result;
    }
    
private:
    grid grid_;
    std::vector<answer> answers_;
    
    std::pair<int,int> find_limits(const std::vector<answer> answers) const
    {
        auto biggest_a = int{};
        auto biggest_d = int{};
        for (const auto& answer : answers)
        {
            if (answer.last_coordinate().first > biggest_a) {
                biggest_a = answer.last_coordinate().first;
            }
            if (answer.last_coordinate().second > biggest_d) {
                biggest_d = answer.last_coordinate().second;
            }
        }
        return std::make_pair(biggest_a, biggest_d);
    }
    
    void resize(std::pair<int,int> extremes)
    {
        grid_.resize(extremes);
    }
    
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



#endif /* puzzle_hpp */
