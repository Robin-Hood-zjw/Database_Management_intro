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
}  // namespace bustub

#endif
