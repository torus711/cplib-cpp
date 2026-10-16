// BEGIN SNIPLATE template 
// {{ abbr: my template }}
// {{ class: util }}
// {{ pattern: ^int\smain\(\) }}
// Template is published at https://github.com/torus711/cplib-cpp/template.cpp
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>
#include <iterator>
#include <ranges>
#include <limits>
#include <numeric>
#include <utility>
#include <type_traits>
#include <cmath>
#include <cassert>
#include <cstdio>

using namespace std;
using namespace placeholders;

using LL = long long;
using ULL = unsigned long long;
using VI = std::vector< int >;
using VVI = std::vector< vector< int > >;
using VLL = std::vector< long long >;
using VVLL = std::vector< vector< long long > >;
template < typename T = int > using VT = std::vector< T >;
template < typename T = int > using VVT = std::vector< VT< T > >;
using VS = std::vector< std::string >;
using ISS = std::istringstream;
using OSS = std::ostringstream;
using PII = std::pair< int, int >;
using PLL = std::pair< long long, long long >;
using VPII = std::vector< std::pair< int, int > >;
using VPLL = std::vector< std::pair< long long, long long > >;
template < typename T = int > using WG = std::vector< std::vector< std::pair< int, T > > >;
template < typename T = int > using LIM = std::numeric_limits< T >;
template < typename T = int > using OSI = std::ostream_iterator< T >;

template < typename T > inline std::istream& operator>>( std::istream &s, std::vector< T > &v ){ for ( T &t : v ) { s >> t; } return s; }
template < typename T > inline std::ostream& operator<<( std::ostream &s, const std::vector< T > &v ){ for ( int i = 0; i < int( v.size() ); ++i ){ s << ( " " + !i ) << v[i]; } return s; }

void in_impl(){ return; }
template < typename T, typename... TS > void in_impl( T &head, TS &... tail ){ cin >> head; in_impl( tail ... ); }
#define IN( T, ... ) T __VA_ARGS__; in_impl( __VA_ARGS__ );

template < typename T, typename V >
auto make_vector( const int n, const V &v ) { return std::vector< T >( n, v ); }
template < typename T, typename... TS >
auto make_vector( const int n, TS... ts ) { return std::vector( n, make_vector< T >( std::forward< TS >( ts )... ) ); }
template < typename T >
auto make_vector0( const int n ) { return vector< T >( n ); }
template < typename T, typename... TS >
auto make_vector0( const int n, TS... ts ) { return vector( n, make_vector0< T >( std::forward< TS >( ts )... ) ); }

template < typename T > inline T fromString( const std::string &s ) { T res; std::istringstream iss( s ); iss >> res; return res; }
template < typename T > inline std::string toString( const T &a ) { std::ostringstream oss; oss << a; return oss.str(); }

#define NUMBERED( name, number ) NUMBERED2( name, number )
#define NUMBERED2( name, number ) name ## _ ## number
#define REP1( n ) REP2( NUMBERED( REP_COUNTER, __LINE__ ), n )
#define REP2( i, n ) REP3( i, 0, n )
#define REP3( i, m, n ) for ( int i = ( int )( m ); i < ( int )( n ); ++i )
#define GET_REP( a, b, c, F, ... ) F
#define REP( ... ) GET_REP( __VA_ARGS__, REP3, REP2, REP1 )( __VA_ARGS__ )
#define FOR( e, c ) for ( auto &&e : c )
template < typename C, typename F > void EACH( C &c, F &&f ) { std::for_each( std::begin( c ), std::end( c ), std::forward< F >( f ) ); }
#define ALL( c ) std::begin( c ), std::end( c )
#define AALL( a ) ( std::remove_all_extents< decltype( a ) >::type * )a, ( std::remove_all_extents< decltype( a ) >::type * )a + sizeof( a ) / sizeof( std::remove_all_extents< decltype( a ) >::type )

#define MAP_PRED( c ) std::transform( std::begin( c ), std::end( c ), std::begin( c ), std::bind( std::minus< int >(), _1, 1 ) );

#define SZ( v ) ( (int)( v ).size() )

template < typename T > inline bool chmin( T &a, const T &b ){ if ( b < a ) { a = b; return true; } return false; }
template < typename T > inline bool chmax( T &a, const T &b ){ if ( a < b ) { a = b; return true; } return false; }

class YESNO { const bool upper = false; bool cond;
	public: YESNO() : upper( true ) { return; }
	YESNO( bool c ) : upper( false ), cond( c ) { return; }
	YESNO &operator()( const bool c ) { cond = c; return *this; }
	friend std::ostream& operator<<( std::ostream &, const YESNO & ); };
inline std::ostream& operator<<( std::ostream &s, const YESNO &yesno ) { s << ( yesno.upper ? ( yesno.cond ? "YES" : "NO" ) : ( yesno.cond ? "Yes" : "No" ) ); return s; }

constexpr char LF = '\n';
constexpr auto FL = std::flush< char, char_traits< char > >;

#define PB push_back
#define PF push_front
#define PPB pop_back
#define PPF pop_front
#define EM emplace
#define EB emplace_back
#define BI back_inserter

#define MP make_pair
#define fst first
#define snd second

#define DUMP( x ) cerr << #x << " = " << ( x ) << endl

//      Λ   Λ＿_
//   ／(*ﾟーﾟ)／＼  < Template ends here!
// ／| ￣U U￣|＼／
//   |      |／

int main()
{
	cin.tie( nullptr );
	ios::sync_with_stdio( false );
	cout << setprecision( 12 ) << fixed;

	// {{ cursor }}

	return 0;
}
// END SNIPLATE
