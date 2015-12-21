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

/**
 * Can the word be placed in the given position of the slice
 * (a row or column of the grid)?
 * there must at least be one actual letter match
 */
inline bool can_place(const std::string& word,
               const std::string& slice, std::size_t slice_loc)
{
    auto one_exact_match = bool{false};
    const auto slice_to_match = slice.substr(slice_loc);
    auto stop_at_len = (word.size() > slice_to_match.size()) ? slice_to_match.size() : word.size();
    for (auto i = std::size_t{0}; i<stop_at_len; ++i) {
        auto exact_match = (slice_to_match[i] == word[i]);
        auto blank_match = (slice_to_match[i] == ' ');
        if ((!exact_match) && (!blank_match)) {
            return false;
        }
        one_exact_match = one_exact_match || exact_match;
    }
    return one_exact_match;
}

/**
 * try to fit pieces of the word before the slice (a row or column of
 * the grid)?
 */
inline std::vector<int> look_before(const std::string& word, const std::string& slice)
{
    auto res = std::vector<int>{};
    for (auto i=std::size_t{1}; i<word.size(); ++i) {
        if (can_place(word.substr(i), slice, 0)) {
            res.push_back(-static_cast<int>(i));
        }
    }
    return res;
}

/**
 * Where will the answer fit along the slice (a row or column of
 * the grid)?
 */
inline std::vector<int> find_places(const std::string& word, const std::string& slice)
{
    auto temp = slice;
    auto result = look_before(word, temp);
    for (auto slice_loc = std::size_t{0}; slice_loc < slice.size(); ++slice_loc) {
        if (can_place(word, slice, slice_loc)) {
            result.push_back(static_cast<int>(slice_loc));
        }
    }
    return result;
}

// handy abstraction of an m-by-n char grid.
class grid
{
public:
    static bool ut();
    
    grid(std::pair<int,int> extremes)
    {
        const auto num = static_cast<std::size_t>(extremes.first)+1;
        const auto line = std::string(num, ' ');

        grid_.resize(extremes.second+1);
        for (auto& gridline : grid_)
        {
            gridline = line;
        }
    }
    
    grid(const std::vector<answer>& answers)
    : grid(std::get<LowerRight>(find_limits(answers)))
    {
        for (const auto& answer : answers) {
            overlay(answer);
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
    
    // return grid columns as a vector of strings
    std::vector<std::string> get_columns() const {
        auto res = std::vector<std::string>{};
        const auto cols = columns();
        for (auto across = int{0}; across < cols; ++across) {
            res.push_back(get_column(across));
        }
        return res;
    }
    
    // return grid rows as a vector of strings
    std::vector<std::string> get_rows() const {
        auto res = std::vector<std::string>{};
        const auto irows = rows();
        for (auto down = int{0}; down < irows; ++down) {
            res.push_back(get_row(down));
        }
        return res;
    }

    std::vector<std::string> render() const {
        return grid_;
    }
    
    bool can_place(const answer& ans) const
    {
        if (ans.get_dir() == direction::ACROSS) {
            return can_place_across(ans);
        }
        else {
            return can_place_down(ans);
        }
    }
    
    std::vector<answer> find_places(const std::string& word) const
    {
        auto res = std::vector<answer>{};
        find_places_across(res, word);
        find_places_down(res, word);
        return res;
    }
    
private:
    std::vector<std::string> grid_;
    
    
    // return grid columns as a string
    std::string get_column(int across) const {
        if ((across < 0) || across > columns()) {
            return std::string{}; // no good
        }
        const auto rws = rows();
        auto s = std::string{};
        for (auto down = int{0}; down < rws; ++down) {
            s += grid_[down][across];
        }
        return s;
    }
    
    // return grid row as strings
    std::string get_row(int down) const {
        if ((down < 0) || down > rows()) {
            return std::string{}; // no good
        }
        return grid_[down];
    }

    
    bool can_place_down(const answer& ans) const
    {
        const auto across = std::get<0>(ans.coordinate());
        auto down = std::get<1>(ans.coordinate());
        if ((across < 0) || (across > columns()) || (down > rows())) {
            return false; // no good
        }
        auto word = ans.get_word();
        if (down < 0) {
            word = word.substr(-down);
            down = 0;
        }
        auto slice = get_column(across);
        return ::can_place(word, slice, down);
    }

    bool can_place_across(const answer& ans) const
    {
        auto across = std::get<0>(ans.coordinate());
        const auto down = std::get<1>(ans.coordinate());
        if ((down < 0) || (down > rows()) || (across > columns())) {
            return false; // no good
        }
        auto word = ans.get_word();
        if (across < 0) {
            word = word.substr(-across);
            across = 0;
        }
        auto slice = get_row(down);
        return ::can_place(word, slice, across);
    }
    
    void find_places_across(std::vector<answer>& res, const std::string& word) const
    {
        const auto columns = get_columns();
        for (auto i = std::size_t{}; i<columns.size(); ++i) {
            auto dplaces = ::find_places(word, columns[i]);
            for (auto dplace : dplaces) {
                const auto coord = std::make_pair(static_cast<int>(i),
                                                  static_cast<int>(dplace));
                res.emplace_back(answer{word, coord, direction::DOWN});
            }
        }
    }
    
    void find_places_down(std::vector<answer>& res, const std::string& word) const
    {
        const auto rows = get_rows();
        for (auto i = std::size_t{}; i<rows.size(); ++i) {
            auto aplaces = ::find_places(word, rows[i]);
            for (auto aplace : aplaces) {
                const auto coord = std::make_pair(static_cast<int>(aplace),
                                                  static_cast<int>(i));
                res.emplace_back(answer{word, coord, direction::ACROSS});
            }
        }
    }
};


#endif /* grid_h */
