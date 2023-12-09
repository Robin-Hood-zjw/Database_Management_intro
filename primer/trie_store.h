#ifndef TRIE_STORE_H
#define TRIE_STORE_H

#include <optional>
#include <memory>
#include <string_view>
#include "common/value_guard.h"

namespace bustub {

template <class T>
class TrieStore {
public:
  auto Get(std::string_view key) -> std::optional<ValueGuard<T>>;
  void Put(std::string_view key, T value);
  void Remove(std::string_view key);

private:
  // Add necessary data members or locks here

  // TrieStore implementation details, such as the root and any synchronization mechanisms, should be here.
};

// Explicit instantiation of template functions

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<uint32_t>>;
template void TrieStore::Put(std::string_view key, uint32_t value);

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<std::string>>;
template void TrieStore::Put(std::string_view key, std::string value);

using Integer = std::unique_ptr<uint32_t>;

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<Integer>>;
template void TrieStore::Put(std::string_view key, Integer value);

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<MoveBlocked>>;
template void TrieStore::Put(std::string_view key, MoveBlocked value);

}  // namespace bustub

#endif  // TRIE_STORE_H
