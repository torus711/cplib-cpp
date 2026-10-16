#include "./../_forLib.cpp"

// BEGIN SNIPLATE binary_method
// {{ abbr: binary method }}
// {{ class: Algorithm }}
// {{ pattern: Num binary_method }}
// This function published at https://github.com/torus711/cplib-cpp/algorithm.cpp
template < typename Num, typename F >
Num binary_method( const F &f, Num ok, Num ng )
{
	while ( 1 < abs( ok - ng ) )
	{
		const Num mid = ( ok + ng ) / 2;
		( f( mid ) ? ok : ng ) = mid;
	}
	return ok;
}

// END SNIPLATE

// BEGIN SNIPLATE partialsum
// {{ abbr: Partial Sum}}
// {{ class: Algorithm }}
// {{ pattern: class\sPsum }}
// This struct published at https://github.com/torus711/cplib-cpp/algorithm.cpp
template < typename T >
struct Psum
{
	std::vector< T > psum;

	Psum( const std::vector< T > &A ) : psum( 1 )
	{
		std::partial_sum( std::begin( A ), std::end( A ),
				std::back_inserter( psum ) );
		return;
	}

	T operator[]( const int i ) const
	{
		return psum[ i + 1 ];
	}

	T sum( const int l, const int r ) const
	{
		return psum[r] - psum[l];
	}
};

// END SNIPLATE

// BEGIN SNIPLATE partialsum2d
// {{ abbr: Partial Sum for 2D }}
// {{ class: Algorithm }}
// {{ pattern: class\sPartialSum2d }}
// 累積和による区間 Sum クエリ
// 前処理 O( HW )
// クエリ O( 1 )
// This class published at https://github.com/torus711/cplib-cpp/algorithm.cpp
class PartialSum2d
{
private:
	const int H, W;
	vector< vector< long long > > src, csum;
	bool isSolved = false;

public:
	PartialSum2d( const int h, const int w ) : H( h ), W( w ), src( H, vector< long long >( W ) ) {}
	PartialSum2d( const vector< vector< long long > > &s ) : H( s.size() ), W( s[0].size() ), src( s ) {}
	PartialSum2d( const vector< vector< int > > &s ) : H( s.size() ), W( s[0].size() ), src( H, vector< long long >( W, 0 ) )
	{
		for ( int i = 0; i < H; ++i )
		{
			for ( int j = 0; j < W; ++ j )
			{
				src[i][j] = s[i][j];
			}
		}
		return;
	}


	PartialSum2d( const vector< string > &s ) : H( s.size() ), W( s[0].size() ), src( H, vector< long long >( W, 0 ) )
	{
		for ( int i = 0; i < H; ++i )
		{
			for ( int j = 0; j < W; ++ j )
			{
				src[i][j] = s[i][j] - '0';
			}
		}
		return;
	}

	int get( const int y, const int x ) const
	{
		return src[y][x];
	}

	void set( const int y, const int x, const long long v )
	{
		src[y][x] = v;
		isSolved = false;
		
		return;
	}

	long long sum( const int y1, const int x1, const int y2, const int x2 )
	{
		if ( !isSolved )
		{
			solve();
		}
		return csum[ y2 + 1 ][ x2 + 1 ] - csum[ y2 + 1 ][ x1 ] - csum[ y1 ][ x2 + 1 ] + csum[ y1 ][ x1 ];
	}

private:
	void solve()
	{
		csum.clear();
		csum.resize( H + 1, vector< long long >( W + 1 ) );

		for ( int i = 0; i < H; ++i )
		{
			partial_sum( src[i].begin(), src[i].end(), csum[ i + 1 ].begin() + 1 );
		}

		for ( int i = 0; i < H; ++i )
		{
			for ( int j = 0; j <= W; ++j )
			{
				csum[ i + 1 ][j] += csum[i][j];
			}
		}

		isSolved = true;
		return;
	}
};
// PartialSum2d( H, W )
// PartialSum2d( SRC )
// consecutiveSums2d( vector< string > SRC )
// get( y, x )
// set( y, x, value )
// sum( y1, x1, y2, x2 ) ←それぞれの座標軸に対して閉区間

// END SNIPLATE

// BEGIN SNIPLATE imos2d 
// {{ abbr: Imos's method for 2D }}
// {{ class: Algorithm }}
// {{ pattern: class\sImos2d }}
// いもす法による区間加算クエリ
// 加算 O( 1 )
// 集計 O( HW )
// This class published at https://github.com/torus711/cplib-cpp/algorithm.cpp
class Imos2d
{
private:
	const int H, W;
	vector< vector<int> > board, csum;
	bool solved;

public:
	Imos2d( const int h, const int w ) : H( h ), W( w ), board( H + 1, vector<int>( W + 1 ) ) {}

	void add( const int y1, const int x1, const int y2, const int x2, const int value )
	{
		board[ y1 ][ x1 ] += value;
		board[ y1 ][ x2 ] -= value;
		board[ y2 ][ x1 ] -= value;
		board[ y2 ][ x2 ] += value;

		solved = false;

		return;
	}

	int getValue( const int y, const int x )
	{
		if ( !solved )
		{
			solve();
		}
		return csum[y][x];
	}

private:
	void solve()
	{
		csum = board;

		for ( auto &row : csum )
		{
			partial_sum( row.begin(), row.end(), row.begin() );
		}

		for ( int i = 0; i < H; i++ )
		{
			for ( int j = 0; j <= W; j++ )
			{
				csum[ i + 1 ][j] += csum[i][j];
			}
		}

		solved = true;

		return;
	}
};
// imos2d( H, W )
// add( y1, x1, y2, x2, value ) : 半開区間
// getvalue( y, x )

// END SNIPLATE

// BEGIN SNIPLATE runlength
// {{ abbr: Run Length Encode }}
// {{ class: Algorithm }}
// {{ pattern: run_length_encode }}
// This function published at https://github.com/torus711/cplib-cpp/algorithm.cpp
template < typename ForwardIterator >
auto run_length_encode( ForwardIterator first, const ForwardIterator last )
{
	using T = decltype( *first );
	std::vector< std::pair< T, int > > result;
	while ( first != last )
	{
		const auto it = std::find_if_not( first, last, std::bind( std::equal_to< T >(), std::placeholders::_1, *first ) );
		result.emplace_back( *first, std::distance( first, it ) );
		first = it;
	}
	return result;
}

// END SNIPLATE

// 計数ソート
template < typename Iterator, typename F >
void counting_sort( Iterator first, Iterator last, F f, int d = 1000000 )
{
    std::vector<int> c( d );
 
    for ( auto it = first; it != last; ++it )
    {
        ++c[ f( *it ) ];
    }
    std::partial_sum( c.begin(), c.end(), c.begin() );
 
    std::vector< typename std::iterator_traits<Iterator>::value_type > result( last - first );
	while ( first != last )
    {
        --c[ f( *--last ) ];
        result[ c[ f( *last ) ] ] = *last;
    }
     
    std::copy( result.begin(), result.end(), first );
 
    return;
}
 
// 基数ソート
template < typename Iterator >
void radix_sort( Iterator first, Iterator last )
{
	for ( int d = 0, lg = 0; d < 8; ++d, lg += 8 )
    {
        counting_sort( first, last, [=]( long long a ){ return ( a >> lg ) & 255; }, 256 );
    }
 
    return;
}


// BEGIN SNIPLATE fft 
// {{ abbr: Convolution by Descrete Fourier Transform}}
// {{ class: Algorithm }}
// {{ pattern: namespace\sfast_fourier_transform }}
// 畳み込み
#include <complex>
namespace fast_fourier_transform 
{
	using Complex = std::complex< double >;
	constexpr double PI = acos( -1 );

	void fft( std::vector< Complex > &F, const bool rev = false )
	{
		const int N = F.size();
		if ( N == 1 )
		{
			return;
		}

		std::vector< Complex > f0( N / 2 ), f1( N / 2 );
		for ( int i = 0; i < N / 2; ++i )
		{
			f0[i] = F[ i * 2 + 0 ];
			f1[i] = F[ i * 2 + 1 ];
		}
		fft( f0, rev );
		fft( f1, rev );

		const Complex zeta = std::polar( 1., 2 * PI / N * ( rev ? -1 : 1 ) );
		Complex pzeta( 1, 0 );

		for ( int i = 0; i < N; ++i )
		{
			F[i] = f0[ i % ( N / 2 ) ] + pzeta * f1[ i % ( N / 2 ) ];
			pzeta *= zeta;
		}

		return;
	}

	template < typename T >
	std::vector< double > convolution( const std::vector< T > &A, const std::vector< T > &B )
	{
		int N = 1;
		for ( ; N < int( A.size() + B.size() + 1 ); N <<= 1 );

		std::vector< Complex > a( A.begin(), A.end() ), b( B.begin(), B.end() );
		a.resize( N );
		b.resize( N );

		fft( a );
		fft( b );

		std::vector< Complex > c;
		c.reserve( N );
		std::transform( a.begin(), a.end(), b.begin(), back_inserter( c ), std::multiplies< Complex >() );

		fft( c, true );

		std::vector< double > res;
		res.reserve( N );
		std::transform( c.begin(), c.end(), back_inserter( res ), [=]( const Complex &comp ){ return comp.real() / N; } );
		return res;
	}
}

// END SNIPLATE

/********/
/* 乱数 */
/********/

// BEGIN SNIPLATE xor128 
// {{ abbr: Random Number Generator by XOR Shift }}
// {{ class: Algorithm, Random }}
// {{ pattern: inline\sunsigned\slong\sxor128() }}
// XOR Shift
inline unsigned long xor128()
{
    static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    unsigned long t;
    t = ( x^(x<<11) );
	x=y; y=z; z=w;
	return ( w = ( w^(w>>19) )^( t^(t>>8) ) );
}

// END SNIPLATE

// [ 0, 1 ] の乱数
inline double rand01()
{
	return 1. * xor128() / ULONG_MAX;
}

// 線形合同法
class LCGs
{
private:
	long long x_;
	const long long a_, b_, m_;

public:
	LCGs( const long long x, const long long a, const long long b, const long long m ) : x_( x ), a_( a ), b_( b ), m_( m ) {}

	long long operator()()
	{
		const long long result = x_;
		x_ = ( a_ * x_ + b_ ) % m_;
		return result;
	}
};
