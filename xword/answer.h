//
//  answer.h
//  xword
//
//  Created by Jonathan Hendrix on 12/7/15.
//  Copyright © 2015 Jonathan Hendrix. All rights reserved.
//

#ifndef answer_h
#define answer_h

#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>

enum class direction
{
    ACROSS,
    DOWN
};

enum class word_status
{
    IN_WORK,
    COMPLETE
};

class answer
{
public:
    static bool ut();
    
    answer(const std::string& w, std::pair<int,int> c, direction d)
    : coord_{c}, dir{d}, status{word_status::IN_WORK}, word{w} {
    }
    
    explicit answer(const std::string& s) {
        std::istringstream in(s);
        auto a = char{};
        auto b = char{};
        in >> coord_.first >> coord_.second >> a >> b >> word;
        dir = (a == 'A') ? direction::ACROSS : direction::DOWN;
        status = (b == 'I') ? word_status::IN_WORK : word_status::COMPLETE;
    }
    
    void mark_complete() {
        status = word_status::COMPLETE;
    }
    
    std::string to_string() const {
        std::ostringstream out;
        out << coord_.first << ' ' << coord_.second << ' ';
        out << ((dir == direction::ACROSS) ? 'A' : 'D') << ' ';
        out << ((status == word_status::IN_WORK) ? 'I' : 'C') << ' ';
        out << word;
        
        return out.str();
    }
    
    std::pair<int,int> coordinate() const {
        return coord_;
    }
    
    std::pair<int,int> last_coordinate() const {
        auto len = static_cast<int>(word.size());
        const auto adelta = (dir == direction::ACROSS) ? len-1 : 0;
        const auto ddelta = (dir == direction::DOWN) ? len-1 : 0;
        return std::make_pair(
            coord_.first + adelta, coord_.second + ddelta);
    }
    
    std::vector<std::tuple<int,int,char>> letters() const {
        auto result = std::vector<std::tuple<int,int,char>>{};
        const auto a = (dir == direction::ACROSS);
        const auto d = (dir == direction::DOWN);
        for (auto i=int{}; i<word.size(); ++i) {
            auto point = std::make_tuple(coord_.first+i*a,
                                         coord_.second+i*d,
                                         word[i]);
            result.push_back(point);
        }
        return result;
    }
    
    word_status get_status() const {
        return status;
    }
    
    bool matches(const answer& a) const {
        return (coord_ == a.coord_) &&
        (dir == a.dir) &&
        (word == a.word);
    }
    
    bool operator==(const answer& a) const {
        return (coord_ == a.coord_) &&
               (dir == a.dir) &&
               (status == a.status) &&
               (word == a.word);
    }
    
    bool operator!=(const answer& a) const {
        return !(*this == a);
    }
    
private:
    std::pair<int,int> coord_;
    direction dir;
    word_status status=word_status::IN_WORK;
    std::string word;
};

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


#endif /* answer_h */
