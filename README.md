# cplib-cpp
競技プログラミング用スニペット (C++)

# 説明
[@torus711](https://x.com/torus711) が個人的に整備している競プロ用 C++ スニペット．

[sniplate.vim](https://github.com/MiSawa/sniplate.vim) で貼り付ける運用をしているので，sniplate.vim のためのコメント行が入っています．
未対応のところの方が多い気もしますが……．

## 各ファイルについて
### template.cpp
いわゆるテンプレ．
説明は[昔書きました](https://torus711.hatenablog.com/entry/2023/12/04/231653)が内容が最新ではないです．

### algorithm.cpp
他のカテゴリに該当しなかったアルゴリズムたち．
- 二分法
- 累積和
    - 二次元累積和
- いもす法
    - 二次元いもす法
- Run Length Encoding
- etc.

### dataStructure.cpp
データ構造色々．
- Disjoint Set Forest とそのバリエーション
- Fenwick Tree
- etc.

### graph.cpp
グラフアルゴリズム．
- 強連結成分分解
- 二部グラフ判定
- 最大独立集合
- etc.
非常に昔に書かれたので要見直し．

### mathematics
数学っぽいやつら．

### mathematicsACL
数学っぽいやつらの [ACL](https://github.com/atcoder/ac-library) 使用版．
- 階乗
- 二項係数

### segmentTree.cpp
典型っぽいセグメント木色々．

### segmentTreeACL.cpp
ACL の `segtree`, `lazysegtree` 用の雛形．
特に後者のテンプレート引数の順序を覚えられないのでね……．

### geometry_complex.cpp
幾何色々．
永遠に未完成．

### util.cpp
ユーティリティという名の未分類．
