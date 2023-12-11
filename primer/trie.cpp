#include "trie.h"
#include "common/exception.h"

using namespace std;

namespace bustub {
    template <class T>
    auto Trie<T>::Get(std::string_view key) const -> const T* {
        auto ptr = GetRoot();
        for (size_t i = 0; i < key.size(); ++i) {
            char token = key[i];
            if (!ptr || !ptr->GetChildren().count(token)) return nullptr;
            ptr = ptr->GetChildren().at(token);
        }

        auto res = dynamic_cast<const TrieNodeWithValue<T>*>(ptr.get());
        return !res || !res->is_leaf_node_ ? nullptr : res->GetValuePtr();
    }

    template <class T>
    auto Trie<T>::Put(std::string_view key, T value) const -> Trie {
        auto ptr = GetRoot();
        auto val_ptr = make_shared<T>(move(value));

        if (key.size() == 0) {
            return Trie(make_shared<const TrieNode<T>>(ptr->GetChildren(), val_ptr));
        }

        vector<shared_ptr<const TrieNode<T>>> vec(key.size() + 1, nullptr);
        vec[0] = this->GetRoot();

        for (size_t i = 0; i < key.size(); ++i) {
            char token = key[i];
            if (ptr && ptr->GetChildren().count(token) > 0) {
                ptr = ptr->GetChildren().at(token);
            } else {
                ptr = nullptr;
            }
            vec[i + 1] = ptr;

            if (i == key.size() - 1) {
                vec[i + 1] = !ptr ?
                    make_shared<const TrieNode<T>>(val_ptr) :
                    make_shared<const TrieNode<T>>(ptr->GetChildren(), val_ptr);
            }
        }

        for (size_t i = key.size(); i > 0; --i) {
            char token = key[i - 1];
            auto target = vec[i];
            if (target) {
                vec[i] = vec[i]->Clone();
                const_cast<TrieNode<T>*>(vec[i].get())->GetChildren().at(token) = vec[i - 1];
            } else {
                map<char, shared_ptr<const TrieNode<T>>> hash {{token, vec[i - 1]}};
                vec[i - 1] = make_shared<const TrieNode<T>>(hash);
            }
        }

        return Trie(vec[0]);
    }

    template <class T>
    auto Trie<T>::Remove(std::string_view key) const -> Trie {
        if (!this->GetRoot()) return *this;

        auto cut_itr = key.begin() - 1;
        shared_ptr<const TrieNode<T>> root = this->GetRoot();

        for (auto it = key.begin(); it < key.end(); ++it) {
            if (!root->GetChildren().count(*it)) return *this;

            root = root->GetChildren().at(*it);
            if (it != key.end() - 1) {
                if (!root->is_leaf_node_ && root->GetChildren().size() == 1) {
                    if (cut_itr == key.end()) cut_itr = it;
                } else {
                    cut_itr = key.end();
                }
            } else {
                if (root->GetChildren().empty()) {
                    if (cut_itr == key.end()) cut_itr = it;
                } else {
                    cut_itr = key.end();
                }
            }
        }

        shared_ptr<TrieNode<T>> new_node = nullptr;
        shared_ptr<TrieNode<T>> new_node_prev = nullptr;
        shared_ptr<const TrieNode<T>> old_node = nullptr;
        shared_ptr<const TrieNode<T>> new_root = nullptr;

        for (auto it = key.begin() - 1; it < key.end(); ++it) {
            if (cut_itr == it) {
                if (cut_itr != key.begin() - 1) new_node_prev->GetChildren().erase(*it);
                break;
            }

            if (it == key.begin() - 1) {
                old_node = this->GetRoot();
            } else if (old_node) {
                old_node = old_node->GetChildren().count(*it) > 0 ? old_node->GetChildren().at(*it) : nullptr;
            }

            if (it == key.end() - 1) {
                new_node = old_node ? make_shared<TrieNode<T>>(old_node->GetChildren()) : make_shared<TrieNode<T>>();
            } else {
                new_node = old_node ? old_node->Clone() : make_shared<TrieNode<T>>();
            }

            if (it == key.begin() - 1) {
                new_root = new_node;
                new_node_prev = new_node;
            } else {
                new_node_prev->GetChildren().at(*it) = new_node;
                new_node_prev = new_node;
            }
        }

        return Trie(new_root);
    }

    // Explicit instantiations for template functions
    template auto Trie<uint32_t>::Put(std::string_view key, uint32_t value) const -> Trie;
    template auto Trie<uint32_t>::Get(std::string_view key) const -> const uint32_t *;

    template auto Trie<uint64_t>::Put(std::string_view key, uint64_t value) const -> Trie;
    template auto Trie<uint64_t>::Get(std::string_view key) const -> const uint64_t *;

    template auto Trie<string>::Put(std::string_view key, string value) const -> Trie;
    template auto Trie<string>::Get(std::string_view key) const -> const string *;

    using Integer = std::unique_ptr<uint32_t>;

    template auto Trie<Integer>::Put(std::string_view key, Integer value) const -> Trie;
    template auto Trie<Integer>::Get(std::string_view key) const -> const Integer *;

    // Assuming MoveBlocked is a valid type, replace it with the actual type in your code.
    // template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
    // template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
