#ifndef TRIE_H
#define TRIE_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <cstddef>
#include <string_view>
#include "common/exception.h"

namespace bustub {
    template <class T>
    class TrieNode {
        private:
            std::map<char, std::shared_ptr<const TrieNode>> children_;

        public:
            TrieNode Clone() const; // create a new node
            std::map<char, std::shared_ptr<const TrieNode>> GetChildren() const;
            explicit TrieNode(const std::map<char, std::shared_ptr<const TrieNode>>& children);
    };

    template <class T>
    class TrieNodeWithValue : public TrieNode<T> {
        private:
            bool is_leaf_node_;
            std::shared_ptr<T> value_;

        public:
            using TrieNode<T>::TrieNode;  // Inherit constructors
            const T& GetValue() const;
            bool GetLeafStatus() const;
            TrieNodeWithValue Clone() const;
            std::shared_ptr<T> GetValuePtr() const;
            TrieNodeWithValue(std::shared_ptr<T> value);
            TrieNodeWithValue(const std::map<char, std::shared_ptr<const TrieNode<T>>>& children, std::shared_ptr<T> value);
    };

    template <class T>
    class Trie {
        private:
            std::shared_ptr<const TrieNode<T>> root_;

        public:
            Trie();
            Trie Clone() const;
            std::shared_ptr<const TrieNode<T>> GetRoot() const;
            explicit Trie(const std::shared_ptr<const TrieNode<T>>& root);

            template <class U>
            const U* Get(std::string_view key) const;

            template <class U>
            Trie Put(std::string_view key, U value) const;

            Trie Remove(std::string_view key) const;
    };



    // the implementations of the above function headers
    template <class T>
    TrieNode<T> TrieNode<T>::Clone() const {
        return TrieNode<T>(children_);
    }

    template <class T>
    std::map<char, std::shared_ptr<const TrieNode<T>>> TrieNode<T>::GetChildren() const {
        return children_;
    }

    template <class T>
    TrieNode<T>::TrieNode(const std::map<char, std::shared_ptr<const TrieNode>>& children)
        : children_(children) {}

    template <class T>
    const T& TrieNodeWithValue<T>::GetValue() const {
        return *value_;
    }

    template <class T>
    bool TrieNodeWithValue<T>::GetLeafStatus() const {
        return is_leaf_node_;
    }

    template <class T>
    TrieNodeWithValue<T> TrieNodeWithValue<T>::Clone() const {
        return TrieNodeWithValue<T>(this->GetChildren(), value_);
    }

    template <class T>
    std::shared_ptr<T> TrieNodeWithValue<T>::GetValuePtr() const {
        return value_;
    }

    template <class T>
    TrieNodeWithValue<T>::TrieNodeWithValue(std::shared_ptr<T> value)
        : is_leaf_node_(true), value_(value) {}

    template <class T>
    TrieNodeWithValue<T>::TrieNodeWithValue(const std::map<char, std::shared_ptr<const TrieNode<T>>>& children,
                                            std::shared_ptr<T> value)
        : TrieNode<T>(children), is_leaf_node_(true), value_(value) {}

    template <class T>
    Trie<T>::Trie() : root_(nullptr) {}

    template <class T>
    Trie<T> Trie<T>::Clone() const {
        return Trie<T>(root_);
    }

    template <class T>
    std::shared_ptr<const TrieNode<T>> Trie<T>::GetRoot() const {
        return root_;
    }

    template <class T>
    Trie<T>::Trie(const std::shared_ptr<const TrieNode<T>>& root) : root_(root) {}
}

#endif