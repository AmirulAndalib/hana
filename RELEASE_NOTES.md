Release notes for Hana 1.6.1
============================

- Official support for Xcode 6, 7 and 8, 9, 10 and LLVM Clang 3.5, 3.6, 3.7,
  and 3.8 has has been dropped. The library should still work with these
  compilers, however they are not being tested regularly anymore, so they are
  not officially supported.
- The `hana::traits::result_of` trait has been removed. Since `std::result_of`
  has been removed from the Standard in C++20, users should move away from it.
