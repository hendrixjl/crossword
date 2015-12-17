//
//  grid.h
//  xword
//
//  Created by Jonathan Hendrix on 12/8/15.
//  Copyright © 2015 Jonathan Hendrix. All rights reserved.
//

#ifndef grid_h
#define grid_h

#include <vector>
#include <string>
#include <algorithm>

#include <iostream>

#include "answer.h"

// Is there a two-or-more letter word in s starting at across?
inline bool word_starting(const int across, const int last, const std::string& s)
{
    return (across != (last-1)) &&
           (s[across] != ' ') &&
           (s[across+1] != ' ');
}

// return the word starting at across.
inline std::pair<int,std::string> get_word(int across, int last, const std::string& s)
{
    auto res = std::string{};
    auto len = int{0};
    while ((s[across+len] != ' ') && (across + len < last))
    {
        res += s[across+len];
        ++len;
    }
    return std::make_pair(len, res);
}

// add all the words you can find in s to answers.
inline void add_words_found(const int down, const std::string& s,
                            direction dir, std::vector<answer>& answers)
{
    auto across = int{0};
    auto last = static_cast<int>(s.size());
    while (across < last)
    {
        auto last_word_len = 0;
        if (word_starting(across, last, s))
        {
            auto res = get_word(across, last, s);
            answer myanswer = (dir == direction::ACROSS) ?
                answer{res.second, std::make_pair(across, down), dir} :
                answer{res.second, std::make_pair(down, across), dir};
            answers.push_back(myanswer);
            last_word_len = res.first;
        }
        across += last_word_len + 1;
    }
}

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
                std::cout << "conflicts with across=" << across << " down=" << down << " - " << grid_[down][across] << std::endl;
                return true; // conflict!
            }
        }
        return false;
    }

private:
    std::vector<std::string> grid_;

};


inline void mark_new_answers(const std::vector<answer>& oldanswers,
                             std::vector<answer>& result)
{
    for (auto& a : result) {
        auto it = find_if(oldanswers.cbegin(), oldanswers.cend(),
                       [&a](const auto& oa) {
                           return oa.matches(a);
                       });
        if (it != oldanswers.cend()) {
            if (it->get_status() == word_status::COMPLETE)
            {
                a.mark_complete();
            }
        }
    }
}

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

#endif /* grid_h */
