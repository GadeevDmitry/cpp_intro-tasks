#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "type_traits.hpp"
#include <cassert>

#define VERIFICATION_TEMPLATE_CLASS_INPUT_IT    \
template <                                      \
    class InputIt,                              \
    class = enable_if_t<                        \
            is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>>>

#define VERIFICATION_TEMPLATE_CLASS_BIDIRECT_IT \
template <                                      \
    class BidirectIt,                           \
    class = std::enable_if_t<                   \
            std::is_base_of_v<std::bidirectional_iterator_tag, typename std::iterator_traits<BidirectIt>::iterator_category>>>

//==================================================================================================

namespace my_std
{
    template <class T, class Allocator = std::allocator<T>>
    class vector
    {
    // types
    public:
        using value_type      = T;
        using allocator_type  = Allocator;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using reference       = value_type&;
        using const_reference = const value_type&;
        using pointer         = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;

        class iterator
        {
        // types
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using difference_type   = ptrdiff_t;
            using pointer           = T*;
            using reference         = T&;

        // member functions
        public:
            iterator(pointer value = nullptr):
            value_(value)
            {}

            iterator(const iterator &that):
            value_(that.value_)
            {}

            iterator &operator =(const iterator &that)
            {
                value_ = that.value_;
                return *this;
            }

            reference operator *() const
            {
                return *value_;
            }

            iterator &operator++()    { ++value_; return *this; }
            iterator &operator++(int) { value_++; return *this; }

            iterator &operator--()    { --value_; return *this; }
            iterator &operator--(int) { value_--; return *this; }

            iterator operator +(long delta) const { return iterator(value_ + delta); }
            iterator operator -(long delta) const { return iterator(value_ - delta); }

            iterator &operator +=(long delta) { value_ += delta; return *this; }
            iterator &operator -=(long delta) { value_ -= delta; return *this; }

            difference_type operator -(const iterator &that) const
            {
                return value_ - that.value_;
            }

            bool operator ==(const iterator &that) const
            {
                return value_ == that.value_;
            }

            auto operator <=>(const iterator &that) const
            {
                return value_ <=> that.value_;
            }

            value_type operator [](size_type idx) const
            {
                return value_[idx];
            }

            pointer get_ptr() const
            {
                return value_;
            }

        // member data
        private:
            pointer value_;
        };

        using const_iterator = const iterator;

    // friends

        friend std::ostream &operator <<(std::ostream &out, const vector &self)
        {
            out <<
                "vector (" << &self << ")\n" <<
                "\tsize     = " << self.size()     << "\n" <<
                "\tcapacity = " << self.capacity() << "\n";

            size_type idx = 0;
            for (iterator it = self.begin_; it != self.end_size_; ++it, ++idx)
                out << "\n# " << idx << "\n" << *it;

            return out;
        }

    // member functions
    public:
        vector():
        allocator_   (),
        begin_       (nullptr),
        end_size_    (nullptr),
        end_capacity_(nullptr)
        {}

        explicit vector(const Allocator &allocator):
        allocator_   (allocator),
        begin_       (nullptr),
        end_size_    (nullptr),
        end_capacity_(nullptr)
        {}

        explicit vector(size_type count, const_reference value, const Allocator &allocator = Allocator()):
        allocator_   (allocator),
        begin_       (std::allocator_traits<Allocator>::allocate(allocator_, count)),
        end_size_    (begin_ + count),
        end_capacity_(end_size_)
        {
            copy_construct(begin_, end_size_, value);
        }

        explicit vector(size_type count, const Allocator &allocator = Allocator()):
        vector(count, T(), allocator)
        {}

        VERIFICATION_TEMPLATE_CLASS_INPUT_IT
        explicit vector(InputIt first, InputIt last, const Allocator &allocator = Allocator()):
        allocator_   (allocator),
        begin_       (std::allocator_traits<Allocator>::allocate(allocator_, std::distance(first, last))),
        end_size_    (begin_ + std::distance(first, last)),
        end_capacity_(end_size_)
        {
            copy_construct(begin_, first, last);
        }

        vector(const vector &that):
        allocator_   (that.allocator_),
        begin_       (std::allocator_traits<Allocator>::allocate(allocator_, that.capacity())),
        end_size_    (begin_ + that.size()),
        end_capacity_(begin_ + that.capacity())
        {
            copy_construct(begin_, that.begin_, that.end_size_);
        }

        explicit vector(const vector &that, const Allocator &allocator):
        allocator_   (allocator),
        begin_       (std::allocator_traits<Allocator>::allocate(allocator_, that.capacity())),
        end_size_    (begin_ + that.size()),
        end_capacity_(begin_ + that.capacity())
        {
            copy_construct(begin_, that.begin_, that.end_size_);
        }

        vector(vector &&that):
        allocator_   (std::move(that.allocator_)),
        begin_       (that.begin_),
        end_size_    (that.end_size_),
        end_capacity_(that.end_capacity_)
        {
            that.begin_        = nullptr;
            that.end_size_     = nullptr;
            that.end_capacity_ = nullptr;
        }

        explicit vector(vector &&that, const Allocator &allocator):
        allocator_   (allocator),
        begin_       (std::allocator_traits<Allocator>::allocate(allocator_, that.capacity())),
        end_size_    (begin_ + that.size()),
        end_capacity_(begin_ + that.capacity())
        {
            move_construct(begin_, that.begin_, that.end_size_);
        }

        explicit vector(std::initializer_list<T> init_list, const Allocator &allocator = Allocator()):
        allocator_   (allocator),
        begin_       (std::allocator_traits<Allocator>::allocate(allocator_, init_list.size())),
        end_size_    (begin_ + init_list.size()),
        end_capacity_(end_size_)
        {
            copy_construct(begin_, init_list.begin(), init_list.end());
        }

        ~vector()
        {
            destroy(begin_, end_size_);
            std::allocator_traits<Allocator>::deallocate(allocator_, begin_.get_ptr(), capacity());
            begin_        = nullptr;
            end_size_     = nullptr;
            end_capacity_ = nullptr;
        }

        vector &operator =(const vector &that)
        {
            if (this != &that)
            {
                if (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value &&
                    allocator_ != that.allocator_)
                {
                    this->~vector();
 
                    allocator_    = that.allocator_;
                    begin_        = std::allocator_traits<Allocator>::allocate(allocator_, that.capacity());
                    end_size_     = begin_ + that.size();
                    end_capacity_ = begin_ + that.capacity();

                    copy_construct(begin_, that.begin_, that.end_size_);
                }
                else
                {
                    if (capacity() != that.capacity())
                    {
                        destructive_realloc(that.capacity());
                        end_size_ = begin_ + that.size();
                        copy_construct(begin_, that.begin_, that.end_size_);
                    }
                    else
                    {
                        copy_assign(begin_, that.begin_, that.begin_ + std::min(size(), that.size()));

                        if (size() > that.size())
                        {
                            destroy(begin_ + that.size(), end_size_);
                            end_size_ = begin_ + that.size();
                        }
                        else if (size() < that.size())
                        {
                            size_type old_size = size();
                            end_size_ = begin_ + that.size();
                            copy_construct(begin_ + old_size, that.begin_ + old_size, that.end_size_);
                        }
                    }
                }
            }
            return *this;
        }

        vector &operator =(vector &&that)
        {
            if (this != &that)
            {
                if (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
                    allocator_ == that.allocator_)
                {
                    std::swap(allocator_   , that.allocator_);
                    std::swap(begin_       , that.begin_);
                    std::swap(end_size_    , that.end_size_);
                    std::swap(end_capacity_, that.end_capacity_);
                }
                else
                {
                    move_assign(begin_, that.begin_, that.begin_ + std::min(size(), that.size()));

                    if (size() > that.size())
                    {
                        destroy(begin_ + that.size(), end_size_);
                        end_size_ = begin_ + that.size();
                    }
                    else if (size() < that.size())
                    {
                        size_type old_size = size();
                        end_size_ = begin_ + that.size();
                        move_construct(begin_ + old_size, that.begin_ + old_size, that.end_size_);
                    }
                }
            }
            return *this;
        }

        vector &operator =(std::initializer_list<T> init_list)
        {
            assign(init_list.begin(), init_list.end());
            return *this;
        }

        void assign(size_type count, const_reference value)
        {
            if (capacity() != count)
            {
                destructive_realloc(count);
                end_size_ = end_capacity_;
                copy_construct(begin_, end_size_, value);
            }
            else
            {
                assert(size() <= count);

                copy_assign(begin_, end_size_, value);
                if (size() < count)
                {
                    size_type old_size = size();
                    end_size_ = end_capacity_;
                    copy_construct(begin_ + old_size, end_size_, value);
                }
            }
        }

        VERIFICATION_TEMPLATE_CLASS_INPUT_IT
        void assign(InputIt first, InputIt last)
        {
            size_type count = std::distance(first, last);
            if (capacity() != count)
            {
                destructive_realloc(count);
                end_size_ = end_capacity_;
                copy_construct(begin_, first, last);
            }
            else
            {
                assert(size() <= count);

                copy_assign(begin_, first, first + size());
                if (size() < count)
                {
                    size_type old_size = size();
                    end_size_ = end_capacity_;
                    copy_construct(begin_ + old_size, first + old_size, last);
                }
            }
        }

        inline void assign(std::initializer_list<T> init_list)
        {
            assign(init_list.begin(), init_list.end());
        }

        inline allocator_type get_allocator() const
        {
            return allocator_;
        }

        reference at(size_type pos)
        {
            assert(pos < size());
            return begin_[pos];
        }

        const_reference at(size_type pos) const
        {
            assert(pos < size());
            return begin_[pos];
        }

        inline       reference operator [](size_type pos)       { return at(pos); }
        inline const_reference operator [](size_type pos) const { return at(pos); }

        inline       reference front()       { *begin_; }
        inline const_reference front() const { *begin_; }

        inline       reference back ()       { return end_size_[-1]; }
        inline const_reference back () const { return end_size_[-1]; }

        inline T       *data()       { return begin_; }
        inline const T *data() const { return begin_; }

        inline iterator        begin()       { return begin_; }
        inline const_iterator  begin() const { return begin_; }
        inline const_iterator cbegin() const { return begin_; }

        inline iterator        end()       { return end_size_; }
        inline const_iterator  end() const { return end_size_; }
        inline const_iterator cend() const { return end_size_; }

        inline bool empty() const
        {
            return size() == 0;
        }

        inline size_type size() const
        {
            return end_size_ - begin_;
        }

        inline size_type max_size() const
        {
            return std::allocator_traits<Allocator>::max_size(allocator_);
        }

        void reserve(size_type new_capacity)
        {
            if (new_capacity > capacity())
                safe_realloc(new_capacity);
        }

        inline size_type capacity() const
        {
            return end_capacity_ - begin_;
        }

        void clear()
        {
            destroy(begin_, end_size_);
            end_size_ = begin_;
        }

        iterator insert(const_iterator pos, const_reference value)
        {
            assert(pos >= begin_);
            assert(pos <= end_size_);

            iterator new_pos = single_shift_right(pos);
            std::allocator_traits<Allocator>::construct(allocator_, new_pos.get_ptr(), value);
            return new_pos;
        }

        iterator insert(const_iterator pos, T &&value)
        {
            assert(pos >= begin_);
            assert(pos <= end_size_);

            iterator new_pos = single_shift_right(pos);
            std::allocator_traits<Allocator>::construct(allocator_, new_pos.get_ptr(), std::move(value));
            return new_pos;
        }

        iterator insert(const_iterator pos, size_type count, const_reference value)
        {
            if (count == 0) return pos;

            assert(pos >= begin_);
            assert(pos <= end_size_);

            iterator new_pos = shift_right(count, pos);
            copy_construct(new_pos, new_pos + count, value);
            return new_pos;
        }

        VERIFICATION_TEMPLATE_CLASS_INPUT_IT
        iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            if (last == first) return pos;

            assert(pos >= begin_);
            assert(pos <= end_size_);

            size_type count   = std::distance(first, last);
            iterator  new_pos = shift_right(count, pos);
            copy_construct(new_pos, first, last);
            return new_pos;
        }

        inline iterator insert(const_iterator pos, std::initializer_list<T> init_list)
        {
            return insert(pos, init_list.begin(), init_list.end());
        }

        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args)
        {
            assert(pos.get_ptr());
            assert(pos >= begin_);
            assert(pos < end_size_);

            iterator new_pos = single_shift_right(pos);
            std::allocator_traits<Allocator>::construct(allocator_, new_pos.get_ptr(), std::forward<Args>(args)...);
            return new_pos;
        }

        iterator erase(const_iterator pos)
        {
            assert(pos.get_ptr());
            assert(pos >= begin_);
            assert(pos < end_size_);

            std::allocator_traits<Allocator>::destroy(allocator_, pos.get_ptr());
            return shift_left(1, pos + 1);
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            if (first == last)
                return last;

            assert(begin_ <= first);
            assert(first < last);
            assert(last <= end_size_);

            destroy(first, last);
            return shift_left(last - first, last);
        }

        void push_back(const_reference value)
        {
            ensure_free_capacity();
            std::allocator_traits<Allocator>::construct(allocator_, end_size_.get_ptr(), value);
            ++end_size_;
        }

        void push_back(T &&value)
        {
            ensure_free_capacity();
            std::allocator_traits<Allocator>::construct(allocator_, end_size_.get_ptr(), std::move(value));
            ++end_size_;
        }

        template <class... Args>
        reference emplace_back(Args&&... args)
        {
            ensure_free_capacity();
            std::allocator_traits<Allocator>::construct(allocator_, end_size_.get_ptr(), std::forward<Args>(args)...);
            ++end_size_;

            return *(end_size_ - 1);
        }

        void pop_back()
        {
            --end_size_;
            std::allocator_traits<Allocator>::destroy(allocator_, end_size_.get_ptr());
        }

        void resize(size_type count)
        {
            if (count == size()) return;

            if (count < size())
            {
                destroy(begin_ + count, end_size_);
                end_size_ = begin_ + count;
            }
            else
            {
                safe_realloc(count);

                size_type old_size = size();
                end_size_ = end_capacity_;
                copy_construct(begin_ + old_size, end_size_, T());
            }
        }

        void resize(size_type count, const_reference value)
        {
            if (count == size()) return;

            if (count < size())
            {
                destroy(begin_ + count, end_size_);
                end_size_ = begin_ + count;
            }
            else
            {
                safe_realloc(count);

                size_type old_size = size();
                end_size_ = end_capacity_;
                copy_construct(begin_ + old_size, end_size_, value);
            }
        }

    private:
        void safe_realloc(size_type new_capacity)
        {
            assert(new_capacity < max_size());

            size_type new_size = std::min(size(), new_capacity);

            T *new_begin = std::allocator_traits<Allocator>::allocate(allocator_, new_capacity);
            move_construct(new_begin, begin_, begin_ + new_size);

            destroy(begin_, end_size_);
            std::allocator_traits<Allocator>::deallocate(allocator_, begin_.get_ptr(), capacity());

            begin_        = new_begin;
            end_size_     = begin_ + new_size;
            end_capacity_ = begin_ + new_capacity;
        }

        void destructive_realloc(size_type new_capacity)
        {
            assert(new_capacity < max_size());

            destroy(begin_, end_size_);
            std::allocator_traits<Allocator>::deallocate(allocator_, begin_.get_ptr(), capacity());

            begin_        = std::allocator_traits<Allocator>::allocate(allocator_, new_capacity);
            end_size_     = begin_;
            end_capacity_ = begin_ + new_capacity;
        }

        size_type recalc_capacity() const
        {
            return (capacity() == 0) ? default_capacity : (realloc_coef * capacity());
        }

        void ensure_free_capacity()
        {
            assert(end_size_ <= end_capacity_);

            if (end_size_ == end_capacity_)
            {
                size_type new_capacity = recalc_capacity();
                safe_realloc(new_capacity);
            }
        }

        iterator single_shift_right(iterator shift_begin)
        {
            assert(shift_begin >= begin_);
            assert(shift_begin <= end_size_);

            size_type shift_pos = shift_begin - begin_;
            ensure_free_capacity();
            shift_begin = begin_ + shift_pos;

            reverse_move_construct(end_size_, end_size_ - 1, shift_begin - 1);

            ++end_size_;
            return shift_begin;
        }

        iterator shift_right(size_type shift_size, iterator shift_begin)
        {
            assert(shift_size != 0);

            if (shift_size == 1)
                return single_shift_right(shift_begin);

            assert(shift_begin >= begin_);
            assert(shift_begin <= end_size_);

            size_type shift_pos    = shift_begin - begin_;
            size_type new_size     = size() + shift_size;
            size_type new_capacity = capacity();
            while (new_size > new_capacity)
            {
                new_capacity = recalc_capacity();
            }
            safe_realloc(new_capacity);
            shift_begin = begin_ + shift_pos;

            reverse_move_construct(end_size_ + (shift_size - 1), end_size_ - 1, shift_begin - 1);

            end_size_ += shift_size;
            return shift_begin;
        }

        iterator shift_left(size_type shift_size, iterator shift_begin)
        {
            assert(shift_size != 0);

            assert(shift_begin.get_ptr());
            assert(shift_begin >= begin_ + shift_size);
            assert(shift_begin <= end_size_);

            move_construct(shift_begin - shift_size, shift_begin, end_size_);

            end_size_ -= shift_size;
            return shift_begin - shift_size;
        }

        void copy_construct(iterator begin, iterator end, const_reference value)
        {
            if (begin == end)
                return;

            assert(begin.get_ptr());
            assert(end  .get_ptr());

            assert(begin_ <= begin);
            assert(begin < end);
            assert(end <= end_size_);

            for (; begin < end; ++begin)
                std::allocator_traits<Allocator>::construct(
                    allocator_, begin.get_ptr(), value);
        }

        VERIFICATION_TEMPLATE_CLASS_INPUT_IT
        void copy_construct(iterator dst_begin, InputIt src_begin, InputIt src_end)
        {
            if (src_begin == src_end)
                return;

            assert(dst_begin.get_ptr());

            assert(dst_begin >= begin_);
            assert(dst_begin < end_size_);

            for (; src_begin != src_end; ++src_begin, ++dst_begin)
                std::allocator_traits<Allocator>::construct(
                    allocator_, dst_begin.get_ptr(), *src_begin);
        }

        VERIFICATION_TEMPLATE_CLASS_INPUT_IT
        void move_construct(iterator dst_begin, InputIt src_begin, InputIt src_end)
        {
            if (src_begin == src_end)
                return;

            assert(dst_begin.get_ptr());

            for (; src_begin != src_end; ++src_begin, ++dst_begin)
                std::allocator_traits<Allocator>::construct(
                    allocator_, dst_begin.get_ptr(), std::move(*src_begin));
        }

        VERIFICATION_TEMPLATE_CLASS_BIDIRECT_IT
        void reverse_move_construct(iterator dst_begin, BidirectIt src_begin, BidirectIt src_end)
        {
            if (src_begin == src_end)
                return;

            assert(dst_begin.get_ptr());

            for (; src_begin != src_end; --src_begin, --dst_begin)
                std::allocator_traits<Allocator>::construct(
                    allocator_, dst_begin.get_ptr(), std::move(*src_begin));
        }

        void destroy(iterator begin, iterator end)
        {
            if (begin == end)
                return;

            assert(begin.get_ptr());
            assert(end  .get_ptr());

            assert(begin_ <= begin);
            assert(begin < end);
            assert(end <= end_size_);

            for (; begin < end; ++begin)
                std::allocator_traits<Allocator>::destroy(
                    allocator_, begin.get_ptr());
        }

    // static functions
    private:
        static void copy_assign(iterator begin, iterator end, const_reference value)
        {
            if (begin == end)
                return;

            assert(begin.get_ptr());
            assert(end  .get_ptr());

            for (; begin < end; ++begin)
                *begin = value;
        }

        VERIFICATION_TEMPLATE_CLASS_INPUT_IT
        static void copy_assign(iterator dst_begin, InputIt src_begin, InputIt src_end)
        {
            if (src_begin == src_end)
                return;

            assert(dst_begin.get_ptr());

            for (; src_begin != src_end; ++src_begin, ++dst_begin)
                *dst_begin = *src_begin;
        }

        VERIFICATION_TEMPLATE_CLASS_INPUT_IT
        static void move_assign(iterator dst_begin, InputIt src_begin, InputIt src_end)
        {
            if (src_begin == src_end)
                return;

            for (; src_begin != src_end; ++src_begin, ++dst_begin)
                *dst_begin = std::move(*src_begin);
        }

    // static data
    private:
        static const size_type default_capacity = 4;
        static const size_type realloc_coef     = 2;

    // member data
    private:
        Allocator allocator_;
        iterator  begin_;
        iterator  end_size_;
        iterator  end_capacity_;
    };
}

//==================================================================================================

#undef VERIFICATION_TEMPLATE_CLASS_INPUT_IT
#undef VERIFICATION_TEMPLATE_CLASS_BIDIRECT_IT

#endif // VECTOR_HPP
