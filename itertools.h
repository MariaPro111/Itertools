#pragma once

#include <iostream>

template <class Iterator1>
class IteratorRange1 {
public:
    IteratorRange1(Iterator1 begin, Iterator1 end) : begin_(begin), end_(end), cur_(begin) {
    }

    auto begin() const {  // NOLINT
        IteratorRange1 it = *this;
        it.cur_ = begin_;
        return it;
    }

    auto end() const {  // NOLINT
        IteratorRange1 it = *this;
        it.cur_ = end_;
        return it;
    }
    auto operator++() {
        ++cur_;
        return *this;
    }
    bool operator!=(const auto other) const {
        return cur_ != other.cur_;
    }
    auto operator*() const {
        return *cur_;
    }

private:
    Iterator1 begin_;
    Iterator1 end_;
    Iterator1 cur_;
};

class IteratorRange {
public:
    IteratorRange(int64_t begin, int64_t end, int64_t cur, int64_t step)
        : begin_(begin), end_(end), cur_(cur), step_(step) {
    }

    auto begin() const {  // NOLINT
        IteratorRange it = *this;
        it.cur_ = begin_;
        return it;
    }

    auto end() const {  // NOLINT
        IteratorRange it = *this;
        it.cur_ = end_;
        return it;
    }
    auto operator++() {
        cur_ += step_;
        return *this;
    }
    bool operator!=(const auto other) const {
        return cur_ != other.cur_;
    }
    auto operator*() {
        return cur_;
    }

private:
    int64_t begin_;
    int64_t end_;
    int64_t cur_;
    int64_t step_;
};

template <class Iterator>
class IteratorGroup {
public:
    // template <class Iterator1>
    IteratorGroup(Iterator begin, Iterator end) {
        begin_ = begin;
        end_ = end;
        Iterator it = begin_;
        while (it != end && *it == *begin_) {
            ++it;
        }
        cur_start_ = begin_;
        cur_end_ = it;
    }

    auto begin() const {  // NOLINT
        Iterator it = begin_;
        while (it != end_ && *it == *begin_) {
            ++it;
        }
        IteratorGroup iterator = *this;
        iterator.cur_start_ = begin_;
        iterator.cur_end_ = it;
        return iterator;
    }

    auto end() const {  
        IteratorGroup iterator = *this;
        iterator.cur_end_ = end_;
        iterator.cur_start_ = end_;
        return iterator;
    }
    auto operator++() {
        if (cur_end_ == end_) {
            cur_start_ = end_;
            return *this;
        }
        Iterator start = cur_end_;
        Iterator end = ++cur_end_;
        while (end != end_ && *end == *start) {
            ++end;
        }
        cur_start_ = start;
        cur_end_ = end;
        return *this;
    }
    bool operator!=(const auto other) const {
        return cur_start_ != other.cur_start_ || cur_end_ != other.cur_end_;
    }
    auto operator*() {
        IteratorRange1<Iterator> it(cur_start_, cur_end_);
        return it;
    }

    // private:
    Iterator begin_;
    Iterator end_;
    Iterator cur_start_;
    Iterator cur_end_;
    // IteratorRange1<Iterator> cur_;
};

template <class T>
auto Group(const T& vec) {
    IteratorGroup<decltype(std::end(vec))> iter(std::begin(vec), std::end(vec));
    return iter;
}

auto Range(int64_t range) {
    int64_t start = 0;
    int64_t cur = 0;
    int64_t end = range;

    IteratorRange iter(start, end, cur, 1);
    return iter;
}

auto Range(int64_t start, int64_t end, int64_t step) {
    int64_t cur = start;
    end = ((end - start) / step + 1) * step + start;
    IteratorRange iter(start, end, cur, step);
    return iter;
}

auto Range(int64_t start, int64_t end) {
    int64_t cur = start;
    IteratorRange iter(start, end, cur, 1);
    return iter;
}

template <class Iterator1, class Iterator2>
class IteratorZip {
public:
    IteratorZip(Iterator1 begin1, Iterator2 begin2, Iterator1 end1, Iterator2 end2)
        : begin1_(begin1), begin2_(begin2), end1_(end1), end2_(end2), cur1_(begin1), cur2_(begin2) {
        SetEnd();
    }

    void SetEnd() {
        Iterator1 it1 = begin1_;
        Iterator2 it2 = begin2_;
        while (it1 != end1_ && it2 != end2_) {
            ++it1;
            ++it2;
        }
        end1_ = it1;
        end2_ = it2;
    };

    auto begin() const {         
        IteratorZip it = *this;  
        it.cur1_ = begin1_;
        it.cur2_ = begin2_;
        return it;
    }

    auto end() const {           
        IteratorZip it = *this;  
        it.cur1_ = end1_;
        it.cur2_ = end2_;
        return it;
    }
    auto operator++() {
        ++cur1_;
        ++cur2_;
        return *this;
    }
    auto operator*() {
        return std::make_pair(*cur1_, *cur2_);
    }
    bool operator!=(const auto other) const {
        return cur1_ != other.cur1_ || cur2_ != other.cur2_;
    }

private:
    Iterator1 begin1_, end1_;
    Iterator2 begin2_, end2_;
    Iterator1 cur1_;
    Iterator2 cur2_;
};

template <class T1, class T2>
auto Zip(const T1& vec1, const T2& vec2) {
    IteratorZip<decltype(std::begin(vec1)), decltype(std::begin(vec2))> iter(
        std::begin(vec1), std::begin(vec2), std::end(vec1), std::end(vec2));
    return iter;
}

