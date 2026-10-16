// #include "template.cpp";

// BEGIN SNIPLATE inf 
// {{ abbr: declare INF }}
// {{ class: Utility }}
// {{ pattern: constexpr\s*\sINF\s }}
constexpr auto INF = LIM<{{cursor}}>::max() / 2;

// END SNIPLATE

// BEGIN SNIPLATE modint 
// {{ abbr: mod int 998244353 }}
// {{ class: Utility }}
// {{ pattern: using\smint }}
// Published at https://github.com/torus711/cplib-cpp/util.cpp
#include <atcoder/modint>
using namespace atcoder;
using MINT = modint998244353;
// using MINT = modint1000000007;
// using MINT = static_modint< 1000000009 >;
// using MINT0 = dynamic_modint< 0 >;
inline istream& operator>>( istream &s, MINT &t ){ int a; s >> a; t = a; return s; };
inline istream& operator>>( istream &s, vector< MINT > &v ){ for ( MINT &a : v ){ s >> a; } return s; }
inline ostream& operator<<( ostream &s, const MINT &t ){ s << t.val(); return s; }
// ==========================================

// END SNIPLATE

namespace grid4
{
// BEGIN SNIPLATE dxdy4 
// {{ abbr: adjucent direction on grid }}
// {{ class: Utility }}
// {{ pattern: constexpr\sint\sdy }}
constexpr int dy[] = { 0, 1, 0, -1, 0 };
constexpr int dx[] = { 1, 0, -1, 0, 0 };
constexpr string DIR = "RDLUC";

// END SNIPLATE
}

// BEGIN SNIPLATE transpose 
// {{ abbr: transpose vector< string >
// {{ class: Utility }}
// {{ pattern: std::vector<\sstd::string\s>\stranspose }}
// Published at https://github.com/torus711/cplib-cpp/util.cpp
std::vector< std::string > transpose( std::vector< std::string > &board )
{
	const int H = board.size(), W = board[0].size();

	std::vector< string > res( W, std::string( H, ' ' ) );
	for ( int i = 0; i < H; ++i )
	{
		for ( int j = 0; j < W; ++j )
		{
			res[j][i] = board[i][j];
		}
	}

	return std::move( res );
}
// ==========================================

// END SNIPLATE
