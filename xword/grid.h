//
//  grid.h
//  xword
//
//  Created by Jonathan Hendrix on 12/8/15.
//  Copyright © 2015 Jonathan Hendrix. All rights reserved.
//

#ifndef grid_h
#define grid_h

#include "answer.h"

#include <vector>
#include <string>
#include <utility>

// handy abstraction of an m-by-n char grid.
class grid
{
public:
    
    void resize(std::pair<int,int> extremes)
    {
        const auto num = static_cast<std::size_t>(extremes.first)+1;
        const auto line = std::string(num, ' ');

        grid_.resize(extremes.second+1);
        for (auto& gridline : grid_)
        {
            gridline = line;
        }
    }

    
    void overlay(const answer& ans)
    {
        auto letters = ans.letters();
        for (const auto c : letters) {
            const auto across = std::get<0>(c);
            const auto down = std::get<1>(c);
            const auto letter = std::get<2>(c);
            grid_.at(down).at(across) = letter;
        }
    }
    
    int rows() const {
        return static_cast<int>(grid_.size());
    }
    
    int columns() const {
        return static_cast<int>(grid_[0].size());
    }
    
    std::vector<std::string> render() const {
        return grid_;
    }
    
    // return grid columns as a vector of strings
    std::vector<std::string> get_columns() const {
        auto res = std::vector<std::string>{};
        const auto cols = columns();
        const auto rws = rows();
        for (auto across = int{0}; across < cols; ++across) {
            auto s = std::string{};
            for (auto down = int{0}; down < rws; ++down) {
                s += grid_[down][across];
            }
            res.push_back(s);
        }
        return res;
    }
    
    // return grid rows as a vector of strings
    std::vector<std::string> get_rows() const {
        auto res = std::vector<std::string>{};
        const auto irows = rows();
        for (auto down = int{0}; down < irows; ++down) {
            res.push_back(grid_[down]);
        }
        return res;
    }

    /**
     * See if there is a conflict between the proposed answer
     * and the present state of the grid
     */
    bool conflict(const answer& ans) const
    {
        const auto letters = ans.letters();
        for (const auto letter : letters) {
            const auto across = std::get<0>(letter);
            const auto down = std::get<1>(letter);
            const bool fits = (grid_[down][across] == std::get<2>(letter)) ||
            (grid_[down][across] == ' ');
            if (!fits) {
                return true; // conflict!
            }
        }
        return false;
    }

private:
    std::vector<std::string> grid_;

};


#endif /* grid_h */
