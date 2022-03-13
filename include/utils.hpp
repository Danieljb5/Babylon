#pragma once

#include <list>
#include <cstdint>

#include <SFML/Graphics.hpp>

namespace ut
{
    template <typename T>
    class Vector2
    {
    public:
        Vector2()
        {
            x = 0;
            y = 0;
        }

        Vector2(T x, T y)
        {
            this->x = x;
            this->y = y;
        }

        sf::Vector2f get_vec()
        {
            return sf::Vector2f((float)x, (float)y);
        }

        friend Vector2 operator+(Vector2 lhs, const Vector2 &rhs)
        {
            lhs += rhs;
            return lhs;
        }

        friend Vector2 operator-(Vector2 lhs, const Vector2 &rhs)
        {
            lhs -= rhs;
            return lhs;
        }

        friend Vector2 operator*(Vector2 lhs, const T &rhs)
        {
            lhs *= rhs;
            return lhs;
        }

        friend Vector2 operator/(Vector2 lhs, const T &rhs)
        {
            lhs /= rhs;
            return lhs;
        }

        friend Vector2 operator*(Vector2 lhs, const Vector2 &rhs)
        {
            lhs *= rhs;
            return lhs;
        }

        friend Vector2 operator*(const T &lhs, Vector2 rhs)
        {
            rhs *= lhs;
            return rhs;
        }

        friend Vector2 operator/(Vector2 lhs, const Vector2 &rhs)
        {
            lhs /= rhs;
            return lhs;
        }

        friend Vector2 operator/(const T &lhs, Vector2 rhs)
        {
            rhs /= lhs;
            return rhs;
        }

        Vector2& operator+=(const Vector2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }
        
        Vector2& operator-=(const Vector2& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }
        
        Vector2& operator*=(const T multiplier)
        {
            x *= multiplier;
            y *= multiplier;
            return *this;
        }
        
        Vector2& operator/=(const T divisor)
        {
            x /= divisor;
            y /= divisor;
            return *this;
        }
        
        Vector2& operator*=(const Vector2& other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }
        
        Vector2 &operator/=(const Vector2 &other)
        {
            x /= other.x;
            y /= other.y;
            return *this;
        }
        
        T x = 0, y = 0;
    };
    
    typedef Vector2<float> Vector2f;
    typedef Vector2<double> Vector2d;
    typedef Vector2<int> Vector2i;
    typedef Vector2<int8_t> Vector2i8;
    typedef Vector2<int16_t> Vector2i16;
    typedef Vector2<int32_t> Vector2i32;
    typedef Vector2<int64_t> Vector2i64;
    typedef Vector2<unsigned int> Vector2ui;
    typedef Vector2<uint8_t> Vector2ui8;
    typedef Vector2<uint16_t> Vector2ui16;
    typedef Vector2<uint32_t> Vector2ui32;
    typedef Vector2<uint64_t> Vector2ui64;

    struct rect
    {
        Vector2d position;
        Vector2d size;

        rect(const Vector2d& position = {0.0f, 0.0f}, const Vector2d& size = {1.0f, 1.0f})
        : position(position), size(size) {}

        constexpr bool contains(const Vector2d& position) const;    
        constexpr bool contains(const rect& r) const;
        constexpr bool overlaps(const rect& r) const;
    };

    namespace detail
    {
        template <typename T>
        class StaticQuadTree
        {
        public:
            StaticQuadTree(const rect& size = {{0.0f, 0.0f}, {100.0f, 100.0f}}, const size_t max_depth = 8, const size_t depth = 0)
            {
                m_max_depth = max_depth;
                m_depth = depth;
                resize(size);
            }

            void resize(const rect& area)
            {
                clear();

                m_rect = area;
                Vector2d childSize = m_rect.size * 0.5f;

                m_childrenAreas[0] = rect(m_rect.position, childSize);
                m_childrenAreas[1] = rect({m_rect.position.x + childSize.x, m_rect.position.y}, childSize);
                m_childrenAreas[2] = rect({m_rect.position.x, m_rect.position.y + childSize.y}, childSize);
                m_childrenAreas[3] = rect(m_rect.position + childSize, childSize);
            }

            void clear()
            {
                m_items.clear();

                for(int i = 0; i < 4; i++)
                {
                    if(m_children[i]) m_children[i]->clear();
                    delete m_children[i];
                    m_children[i] = nullptr;
                }
            }

            size_t size() const
            {
                size_t count = m_items.size();
                for(int i = 0; i < 4; i++) if(m_children[i]) count += m_children[i]->size();
                return count;
            }

            void insert(const T& item, const rect& itemSize)
            {
                for(int i = 0; i < 4; i++)
                {
                    if(m_childrenAreas[i].contains(itemSize))
                    {
                        if(m_depth + 1 < m_max_depth)
                        {
                            if(!m_children[i])
                            {
                                m_children[i] = new StaticQuadTree<T>(m_childrenAreas[i], m_depth + 1, m_max_depth);
                            }
                            m_children[i]->insert(item, itemSize);
                            return;
                        }
                    }
                }
                m_items.push_back({itemSize, item});
            }

            std::list<T> search(const rect& area) const
            {
                std::list<T> listItems;
                search(area, listItems);
                return listItems;
            }

            void search(const rect& area, std::list<T>& listItems) const
            {
                for(const std::pair<rect, T> item : m_items)
                {
                    if(area.overlaps(item.first)) listItems.push_back(item.second);
                }

                for(int i = 0; i < 4; i++)
                {
                    if(m_children[i])
                    {
                        if(area.contains(m_childrenAreas[i])) m_children[i]->items(listItems);
                        else if(m_childrenAreas[i].overlaps(area)) m_children[i]->search(area, listItems);
                    }
                }
            }

            void items(std::list<T>& listItems) const
            {
                for(const std::pair<rect, T>& item : m_items) listItems.push_back(item.second);
                for(int i = 0; i < 4; i++) if(m_children[i]) m_children[i]->items(listItems);
            }

            const rect& area()
            {
                return m_rect;
            }

        protected:
            size_t m_depth = 0;
            size_t m_max_depth = 8;
            rect m_rect;
            rect m_childrenAreas[4] = {{}};
            StaticQuadTree* m_children[4] = {nullptr};
            std::vector<std::pair<rect, T>> m_items;
        };
    }

    template <typename T>
    class StaticQuadTree
    {
        using QuadTreeContainer = std::list<T>;
    public:
        StaticQuadTree(const rect& size = {{0.0f, 0.0f}, {100.0f, 100.0f}}, const size_t max_depth = 8, const size_t depth = 0)
        : root(size, max_depth, depth) {}

        void resize(const rect& area)
        {
            root.resize(area);
            m_items.clear();
        }

        size_t size() const
        {
            return m_items.size();
        }

        bool empty() const
        {
            return m_items.empty();
        }

        void clear()
        {
            root.clear();
            m_items.clear();
        }

        typename QuadTreeContainer::iterator begin()
        {
            return m_items.begin();
        }

        typename QuadTreeContainer::iterator end()
        {
            return m_items.end();
        }

        typename QuadTreeContainer::iterator cbegin()
        {
            return m_items.cbegin();
        }

        typename QuadTreeContainer::iterator cend()
        {
            return m_items.cend();
        }

        void insert(const T& item, const rect& itemSize)
        {
            m_items.push_back(item);
            root.insert(std::prev(m_items.end()), itemSize);
        }

        std::list<typename QuadTreeContainer::iterator> search(const rect& area) const
        {
            std::list<typename QuadTreeContainer::iterator> listItems;
            root.search(area, listItems);
            return listItems;
        }

    protected:
        QuadTreeContainer m_items;
        detail::StaticQuadTree<typename QuadTreeContainer::iterator> root;
    };
}
