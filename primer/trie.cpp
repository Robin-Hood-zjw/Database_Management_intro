#include "trie.h"
#include "common/exception.h"

using namespace std;

namespace bustub {
  template <class T>
  auto Trie::Get(std::string_view key) const -> const T * {
    
    auto ptr = GetRootPointer();
    for (size_t i = 0; i < key.size(); ++i) {
      char token = key[i];
      if (!ptr || !ptr->GetChildren().count(token)) return nullptr;
      ptr = ptr->GetChildren().at(token);
    }

    auto res = dynamic_cast<const TreeNode<T> *> (ptr.get());

    return !res || !res->GetLeafStatus() ? nullptr : res->GetValuePtr();

    // traverse the trie to find the node corresponding to the key.
    // -- If the node doesn't exist, return nullptr.
    // After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`.
    // If dynamic_cast returns `nullptr`, it means the type of the value is mismatched, return nullptr.
    // Otherwise, return the value.
  }

  template <class T>
  auto Trie::Put(std::string_view key, T value) const -> Trie {
    // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
    // You should walk through the trie and create new nodes if necessary.
    // If the node corresponding to the key already exists, you should create a new `TrieNodeWithValue`.
    auto ptr = GetRootPointer();
    auto val_ptr = make_shared<T>(std::move(value));

    if (key.size() == 0) {
      return Trie(make_shared<const TrieNode<T>> (ptr->GetChildren(), val_ptr));
    }

    vector<shared_ptr<const TrieNode>> vec (key.size() + 1, nullptr);
    vec[0] = this->GetRootPointer();

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
        make_shared<const TrieNode<T>>> (val_ptr) :
        make_shared<const TrieNode<T>>> (ptr->GetChildren(), val_ptr);
      }
    }

    for (size_t i = key.size(); i >= 0; --i) {
      char token = key[i];
      auto target = vec[i];
      if (target) {
        vec[i] = vec[i]->Clone();
        const_cast<TrieNode*>(vec[i].get())->GetChildren().at(token) = vec[i + 1];
      } else {
        map<char, shared_ptr<const TrieNode>> hash {{token, vec[i + 1]}};
        vec[i] = make_shared<const TrieNode>(hash);
      }
    }


    return Trie(vec[0]);
  }

  auto Trie::Remove(std::string_view key) const -> Trie {
    // traverse the trie and remove nodes if necessary.
    // If the node doesn't contain a value any more, you should convert it to `TrieNode`.
    // If a node doesn't have children any more, you should remove it.
    if (!this->GetRootPointer()) return *this;

    auto it_cut = key.begin() - 1;
    std::shared_ptr<const TrieNode> root = this->GetRootPointer();

    for (auto it = key.begin(); it < key.end(); it++) {
      if (!root->GetChildren().count(*it)) return *this;

      root = root->children_.at(*it);
      if (it != key.end() - 1) {
        if (!root->is_value_node_ && root->GetChildren().size() == 1) {
          if (it_cut == key.end()) it_cut = it;
        } else {
          it_cut = key.end();
        }
      } else {
        if (root->children_.empty()) {
          if (it_cut == key.end()) {
            it_cut = it;
          }
        } else {
          it_cut = key.end();
        }
      }
    }

    std::shared_ptr<TrieNode> new_node = nullptr;
    std::shared_ptr<TrieNode> new_node_prev = nullptr;
    std::shared_ptr<const TrieNode> old_node = nullptr;
    std::shared_ptr<const TrieNode> new_root = nullptr;

    for (auto it = key.begin() - 1; it < key.end(); it++) {
      if (it_cut == it) {
        if (it_cut != key.begin() - 1) {
          new_node_prev->children_.erase(*it);
        }
        break;
      }

      if (it == key.begin() - 1) {
        old_node = this->root_;
      } else if (old_node) {
        old_node = old_node->children_.count(*it) > 0 ? old_node->children_.at(*it) : nullptr;
      }

      if (it == key.end() - 1) {
        new_node = old_node ? new_node = std::make_shared<TrieNode>(old_node->children_) : std::make_shared<TrieNode>();
      } else {
        new_node = old_node ? old_node->Clone() : std::make_shared<TrieNode>();
      }

      if (it == key.begin() - 1) {
        new_root = new_node;
        new_node_prev = new_node;
      } else {
        new_node_prev->children_[*it] = new_node;
        new_node_prev = new_node;
      }
    }

    return Trie(new_root);
  }

  std::shared_ptr<const TrieNode> new_root = nullptr;
  std::shared_ptr<TrieNode> new_node = nullptr;
  std::shared_ptr<TrieNode> new_node_prev = nullptr;
  std::shared_ptr<const TrieNode> old_node = nullptr;
  }

  // Below are explicit instantiation of template functions.
  //
  // Generally people would write the implementation of template classes and functions in the header file. However, we
  // separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
  // implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
  // by the linker.

  template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
  template auto Trie::Get(std::string_view key) const -> const uint32_t *;

  template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
  template auto Trie::Get(std::string_view key) const -> const uint64_t *;

  template auto Trie::Put(std::string_view key, string value) const -> Trie;
  template auto Trie::Get(std::string_view key) const -> const string *;

  // If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

  using Integer = std::unique_ptr<uint32_t>;

  template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
  template auto Trie::Get(std::string_view key) const -> const Integer *;

  template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
  template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
