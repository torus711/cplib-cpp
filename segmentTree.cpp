// セグメント木のユーティリティ
inline int chl( const int k )
{
	return k * 2 + 1;
}

inline int chr( const int k )
{
	return k * 2 + 2;
}

inline int mid( const int l, const int r )
{
	return ( l + r ) / 2;
}

#define LEFT( k, l, r ) chl( k ), l, mid( l, r )
#define RIGHT( k, l, r ) chr( k ), mid( l, r ), r


// セグメント木による Range Minimum Query
class RangeMinimumQuery
{
private:
	const int N;
	vector<LL> data;

public:
	RangeMinimumQuery( const vector<LL> &src ) : N( src.size() ), data( N * 4 )
	{
		REP( i, 0, N )
		{
			update( i, src[i] );
		}

		return;
	}

	void update( const int p, const LL x )
	{
		return update( p, x, 0, 0, N );
	}

	int minimum( const int a, const int b ) const
	{
		return minimum( a, b, 0, 0, N );
	}
private:
	void update( const int p, const LL x, const int k, const int l, const int r )
	{
		if ( p < l || r <= p )
		{
			return;
		}
		if ( l + 1 == r && p == l )
		{
			data[k] = x;
			return;
		}

		update( p, x, chl( k ), l, mid( l, r ) );
		update( p, x, chr( k ), mid( l, r ), r );
		data[k] = min( data[ chl( k ) ], data[ chr( k ) ] );
	
		return;
	}

	int minimum( const int a, const int b, const int k, const int l, const int r ) const
	{
		if ( b <= l || r <= a )
		{
			return LLONG_MAX;
		}
		else if ( a <= l && r <= b )
		{
			return data[k];
		}
		else
		{
			return min(	minimum( a, b, chl( k ), l, mid( l, r ) ), minimum( a, b, chr( k ), mid( l, r ), r ) );
		}
	}
};
// RangeMinimumQuery( VI src )
// update( pos, x )
// minimum( [ a, b ) )

// セグメント木による Range Maximum Query
class RangeMaximumQuery
{
private:
	const int N;
	vector<int> data;

public:
	RangeMaximumQuery( const vector<int> &src ) : N( src.size() ), data( N * 4 )
	{
		REP( i, 0, N )
		{
			update( i, src[i] );
		}

		return;
	}

	void update( const int p, const int x )
	{
		return update( p, x, 0, 0, N );
	}

	int maximum( const int a, const int b ) const
	{
		return maximum( a, b, 0, 0, N );
	}
private:
	void update( const int p, const int x, const int k, const int l, const int r )
	{
		if ( p < l || r <= p )
		{
			return;
		}
		if ( l + 1 == r && p == l )
		{
			data[k] = x;
			return;
		}

		update( p, x, chl( k ), l, mid( l, r ) );
		update( p, x, chr( k ), mid( l, r ), r );
		data[k] = max( data[ chl( k ) ], data[ chr( k ) ] );
	
		return;
	}

	int maximum( const int a, const int b, const int k, const int l, const int r ) const
	{
		if ( b <= l || r <= a )
		{
			return INT_MIN;
		}
		else if ( a <= l && r <= b )
		{
			return data[k];
		}
		else
		{
			return max( maximum( a, b, chl( k ), l, mid( l, r ) ), maximum( a, b, chr( k ), mid( l, r ), r ) );
		}
	}
};
// RangeMaximumQuery( VI src )
// update( pos, x )
// maximum( [ a, b ) )

// セグメント木による Range (Minimum|Maximum) Query
template < typename T = int, typename F = std::less< T >,
		 T identity = std::is_same< F, std::less< T > >::value ? std::numeric_limits< T >::max() : std::numeric_limits< T >::min(),
		 typename = typename std::enable_if< std::is_same< F, std::less< T > >::value || std::is_same< F, std::greater< T > >::value >::type >
class RangeMQuery
{
private:
	const int N;
	std::vector< T > data;

	F func;

public:
	RangeMQuery( const int n ) :
		N( n ), data( N * 4 )
	{
		return;
	}
	
	RangeMQuery( const std::vector< T > &src ) :
		N( src.size() ), data( N * 4 )
	{
		init( src, 0, 0, N );
		return;
	}

	void update( const int p, const T x )
	{
		return update( p, x, 0, 0, N );
	}

	T query( const int a, const int b ) const
	{
		return query( a, b, 0, 0, N );
	}

private:
	void init( const vector< T > &src, const int k, const int l, const int r )
	{
		if ( l + 1 == r )
		{
			data[k] = src[l];
			return;
		}

		init( src, LEFT( k, l, r ) );
		init( src, RIGHT( k, l, r ) );
		data[k] = std::min( data[ chl( k ) ], data[ chr( k ) ], func );

		return;
	}

	void update( const int p, const T x, const int k, const int l, const int r )
	{
		if ( p < l || r <= p )
		{
			return;
		}
		if ( l + 1 == r && p == l )
		{
			data[k] = x;
			return;
		}

		update( p, x, chl( k ), l, mid( l, r ) );
		update( p, x, chr( k ), mid( l, r ), r );
		data[k] = std::min( data[ chl( k ) ], data[ chr( k ) ], func );
	
		return;
	}

	T query( const int a, const int b, const int k, const int l, const int r ) const
	{
		if ( b <= l || r <= a )
		{
			return identity;
		}
		else if ( a <= l && r <= b )
		{
			return data[k];
		}
		else
		{
			return std::min( query( a, b, chl( k ), l, mid( l, r ) ), query( a, b, chr( k ), mid( l, r ), r ), func );
		}
	}
};
// RangeMQuery< comp = less, identity element = inf >( N )
// RangeMQuery< comp = less, identity element = inf >( VT src )
// update( pos, x )
// query( [ a, b ) )

// segment tree, range add, sum
class SegmentTreeRAS
{
private:
	const int N;
	vector<int> data_overall, data_partial;

public:
	SegmentTreeRAS( const vector<int> &src ) : N( src.size() ), data_overall( N * 4, 0 ), data_partial( N * 4, 0 )
	{
		REP( i, 0, N )
		{
			add( i, i + 1, src[i] );
		}

		return;
	}

	void add( const int a, const int b, const int x )
	{
		return add( a, b, x, 0, 0, N );
	}

	int sum( const int a, const int b )
	{
		return sum( a, b, 0, 0, N );
	}

private:
	void add( const int a, const int b, const int x, const int k, const int l, const int r )
	{
		if ( b <= l || r <= a )
		{
			return;
		}
		else if ( a <= l && r <= b )
		{
			data_overall[k] += x;
		}
		else
		{
			data_partial[k] += (LL)( min( r, b ) - max( l, a ) ) * x;
			add( a, b, x, chl( k ), l, mid( l, r ) );
			add( a, b, x, chr( k ), mid( l, r ), r );
		}
	}

	int sum( const int a, const int b, const int k, const int l, const int r )
	{
		if ( b <= l || r <= a )
		{
			return 0;
		}
		else if ( a <= l && r <= b )
		{
			return data_partial[k] + (LL)( r - l ) * data_overall[k];
		}
		else
		{
			int res = (LL)( min( r, b ) - max( l, a ) ) * data_overall[k];
			res += sum( a, b, chl( k ), l, mid( l, r ) );
			res += sum( a, b, chr( k ), mid( l, r ), r );
			return res;
		}
	}
};
// SegmentTreeRAS( VI src )
// add( [ a, b ), x )
// sum( [ a, b ) )

// Starry Sky 木
// 区間加算と minimum query
class StarrySkyTree
{
	const int N;
	vector<long long> dat_add, dat_min;

public:
	StarrySkyTree( const vector< long long > &src ) :
		N( src.size() ), dat_add( N * 4 * 2 ), dat_min( N * 4 * 2 )
	{
		REP( i, 0, N )
		{
			add( i, i + 1, src[i] );
		}
		return;
	}

	void add( const int a, const int b, const long long x )
	{
		return add( a, b, x, 0, 0, N );
	}

	void add( const int a, const int b, const long long x, int k, const int l, const int r )
	{
		if ( b <= l || r <= a )
		{
			return;
		}
		else if ( a <= l && r <= b )
		{
			dat_add[k] += x;
			while ( k )
			{
				k = ( k - 1 ) / 2;
				dat_min[k] = min( dat_min[ chl( k ) ] + dat_add[ chl( k ) ], dat_min[ chr( k ) ] + dat_add[ chr( k ) ] );
			}
		}
		else
		{
			add( a, b, x, chl( k ), l, mid( l, r ) );
			add( a, b, x, chr( k ), mid( l, r ), r );
		}
		return;
	}

	long long minimum( const int a, const int b )
	{
		return minimum( a, b, 0, 0, N );
	}

	long long minimum( const int a, const int b, const int k, const int l, const int r )
	{
		if ( b <= l || r <= a )
		{
			return LLONG_MAX;
		}
		else if ( a <= l && r <= b )
		{
			return dat_min[k] + dat_add[k];
		}
		else
		{
			const long long left = minimum( a, b, chl( k ), l, mid( l, r ) );
			const long long right = minimum( a, b, chr( k ), mid( l, r ), r );
			return min( left, right ) + dat_add[k];
		}
	}
};
// StarrySkyTree( vector<LL> src )
// add( [ a, b ), x )
// minimum( [ a, b ) )

// 値の配列と確率の配列から全体の期待値を求める何か
namespace SegmentTree
{
	template < typename T >	
	class ExpectedValueArray 
	{
	private:
		const int N;
		std::vector< T > array;
		std::vector< double > probs, expects;

	public:
		ExpectedValueArray( const int n ) : N( n ), array( N ), probs( N ), expects( N * 4 )
		{
			return;
		}

		ExpectedValueArray( const std::vector< T > &as, const std::vector< double > &ps ) :
			N( as.size() ), array( as ), probs( ps ), expects( N * 4 )
		{
			for ( int i = 0; i < N; ++i )
			{
				update( i, 0, 0, N );
			}
			return;
		}

		void update_array( const int p, const int x )
		{
			array[p] = x;
			update( p, 0, 0, N );
			return;
		}

		void update_prob( const int p, const double x )
		{
			probs[p] = x;
			update( p, 0, 0, N );
			return;
		}

		double expected_value() const
		{
			return expects[0];
		}

	private:
		void update( const int p, const int k, const int l, const int r )
		{
			if ( p < l || r <= p )
			{
				return;
			}

			if ( l + 1 == r && p == l )
			{
				expects[k] = array[p] * probs[p];
				return;
			}

			update( p, chl( k ), l, mid( l, r ) );
			update( p, chr( k ), mid( l, r ), r );

			expects[k] = expects[ chl( k ) ] + expects[ chr( k ) ];

			return;
		}
	};
}
// ExpectedValueArray( N )
// ExpectedValueArray( as, ps )
// update_arrah( pos, value )
// update_prob( pos, value )
// expected_value()

namespace SegmentTree
{
	constexpr int MOD = 1000000007;
	class RangeAdd_GetValue 
	{
		const int N;
		vector<long long> dat_add;

	public:
		RangeAdd_GetValue( const int n ) : N( n ), dat_add( N * 4 )
		{
		}

		void add( const int a, const int b, const int x )
		{
			return add( a, b, x, 0, 0, N );
		}

		void add( const int a, const int b, const int x, int k, const int l, const int r )
		{
			if ( b <= l || r <= a )
			{
				return;
			}
			else if ( a <= l && r <= b )
			{
				( dat_add[k] += x ) %= MOD;
			}
			else
			{
				add( a, b, x, chl( k ), l, mid( l, r ) );
				add( a, b, x, chr( k ), mid( l, r ), r );
			}
			return;
		}

		int value( const int p )
		{
			return value( p, 0, 0, N );
		}

		int value( const int p, const int k, const int l, const int r )
		{
			if ( l == p && l + 1 == r )
			{
				return dat_add[k];
			}
			else if ( l <= p && p < r )
			{
				return ( ( dat_add[k] + value( p, chl( k ), l, mid( l, r ) ) ) % MOD + value( p, chr( k ), mid( l, r ), r ) ) % MOD;
			}
			else
			{
				return 0;
			}
		}
	};
}
// rangeadd_getvalue( N )
// add( [ a, b ), x )
// value( pos )

// RangeTree
// 区間内にある値未満 or 超過の要素数を数える
// 初期化 O( N log N )
// クエリ O( N log^2 N )
namespace SegmentTree
{
	template < typename T >
	class RangeTree
	{
		const int N;
		std::vector< std::vector< T > > data;

	public:
		RangeTree( const std::vector< T > &src ) : N( src.size() ), data( 4 * N )
		{
			init( src, 0, 0, N );
			return;
		}

		void init( const std::vector< T > &src, const int k, const int l, const int r )
		{
			if ( l + 1 == r )
			{
				data[k].push_back( src[l] );
				return;
			}

			init( src, LEFT( k, l, r  ) );
			init( src, RIGHT( k, l, r ) );

			std::merge(
					data[ chl( k ) ].begin(), data[ chl( k ) ].end(),
					data[ chr( k ) ].begin(), data[ chr( k ) ].end(),
					std::back_inserter( data[k] ) );

			return;
		}

		int count( const T x, const int a, const int b, bool less = true )
		{
			return count( x, a, b, less, 0, 0, N );
		}

		int count( const T x, const int a, const int b, bool less, const int k, const int l, const int r )
		{
			if ( r <= a || b <= l )
			{
				return 0;
			}
			else if ( a <= l && r <= b )
			{
				if ( less )
				{
					return lower_bound( data[k].begin(), data[k].end(), x ) - data[k].begin();
				}
				else
				{
					return data[k].end() - upper_bound( data[k].begin(), data[k].end(), x );
				}
			}
			else
			{
				return count( x, a, b, less, LEFT( k, l, r ) ) + count( x, a, b, less, RIGHT( k, l, r ) );
			}
		}
	};
}
// RangeTree( src )
// count( value, a, b, less flag )
