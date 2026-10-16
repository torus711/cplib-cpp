#include "./../_forLib.cpp"

// BEGIN SNIPLATE MinPQue
// {{ abbr: Priority queue respect to min}}

// {{ pattern: using\minpque$ }}
template < typename T >
using minpque = std::priority_queue< T, std::vector< T >, std::greater< T > >;

// END SNIPLATE

// BEGIN SNIPLATE DisjointSetForest 
// {{ abbr: Disjoint Set Forest}}
// {{ class: DataStructure }}
// {{ pattern: class\DisjointSetForest$ }}
// This class published at https://github.com/torus711/cplib-cpp/DataStructure.cpp
class DisjointSetForest
{
protected:
	const int N;

	mutable std::vector<int> parent;
	mutable std::vector<int> rank;

	int groups_;
	std::vector< int > sizes;

public:
	DisjointSetForest( int n ) : N( n ), parent( N ), rank( N, 0 ), groups_( N ), sizes( N, 1 )
	{
		std::iota( parent.begin(), parent.end(), 0 );

		return;
	}

	int find( int x ) const
	{
		if ( parent[x] == x )
		{
			return x;
		}
		return parent[x] = find( parent[x] );
	}

	bool same( int x, int y ) const
	{
		return find( x ) == find( y );
	}

	bool unite( int x, int y )
	{
		x = find( x );
		y = find( y );

		if ( x == y )
		{
			return false;
		}

		if ( rank[x] < rank[y] )
		{
			parent[x] = y;
			sizes[y] += sizes[x];
		}
		else
		{
			parent[y] = x;
			sizes[x] += sizes[y];

			if ( rank[x] == rank[y] )
			{
				++rank[x];
			}
		}

		--groups_;
		return true;
	}

	int numGroups() const
	{
		return groups_;
	}

	std::vector< std::vector< int > > groups() const
	{
		std::vector< std::vector< int > > result( N );
		for ( int i = 0; i < N; ++i )
		{
			result[ find( i ) ].push_back( i );
		}
		std::ranges::remove_if( result, &std::vector< int >::empty );
		return result;
	}

	int groupSize( const int x )
	{
		return sizes[ find( x ) ];
	}
};
// DisjointSetForest( N )
// find( x )
// same( x, y )
// unite( x, y )
// groups()
// groupSize( x )
// ==========================================

// END SNIPLATE

// BEGIN SNIPLATE DisjointSetForestAssociated
// {{ abbr: Disjoint Set Forest with Assosicated Value}}
// {{ require: DisjointSetForest }}
// {{ class: DataStructure }}
// {{ pattern: class\DisjointSetForestAssociated$ }}
// This class published at https://github.com/torus711/cplib-cpp/DataStructure.cpp
template < typename T >
class DisjointSetForestAssociated : public DisjointSetForest
{
	std::vector< T > assoc;
	const function< void( T&, T& ) > merge;
// 	const decltype( []( T&, T& ){} ) &&merge;

public:
	DisjointSetForestAssociated( const int n, const auto &&f ) :
		DisjointSetForest( n ), assoc( n ), merge( f )
	{
		return;
	}

	bool unite( int x, int y )
	{
		x = find( x );
		y = find( y );

		if ( x == y )
		{
			return false;
		}

		if ( rank[x] < rank[y] )
		{
			parent[x] = y;
			sizes[y] += sizes[x];
			merge( assoc[y], assoc[x] );
		}
		else
		{
			parent[y] = x;
			sizes[x] += sizes[y];
			merge( assoc[x], assoc[y] );

			if ( rank[x] == rank[y] )
			{
				++rank[x];
			}
		}

		--groups_;
		return true;
	}

	void set( const int x, const T &v )
	{
		assoc[ find( x ) ] = v;
		return;
	}

	T get( const int x )
	{
		return assoc[ find( x ) ];
	}
};
// DisjointSetForestAssociated( n, void( T&, T& ) )
// set( i, T )
// get( i )
// ==========================================

// END SNIPLATE

// BEGIN SNIPLATE DisjointSetForest2d
// {{ abbr: Disjoint Set Forest}}
// {{ require: DisjointSetForest }}
// {{ class: DataStructure }}
// {{ pattern: class\DisjointSetForest$ }}
// This class published at https://github.com/torus711/cplib-cpp/DataStructure.cpp
class DisjointSetForest2d : private DisjointSetForest
{
	const int H, W;

	inline int index( const int y, const int x ) const
	{
		return y * W + x;
	}

public:
	DisjointSetForest2d( const int h, const int w ) :
		DisjointSetForest( h * w ), H( h ), W( w )
	{
		return;
	}

	using base = DisjointSetForest;

	int find( const int y, const int x )
	{
		return base::find( index( y, x ) );
	}

	bool same( const int y1, const int x1, const int y2, const int x2 )
	{
		return base::same( index( y1, x1 ), index( y2, x2 ) );
	}

	bool unite( const int y1, const int x1, const int y2, const int x2 )
	{
		return base::unite( index( y1, x1 ), index( y2, x2 ) );
	}

	int groups() const
	{
		return base::groups();
	}

	int groupSize( const int y, const int x )
	{
		return base::groupSize( index( y, x ) );
	}
};
// find( y, x )
// same( y1, x1, y2, x2 )
// unite( y1, x1, y2, x2 )
// groups()
// groupSize( y, x )
// ==========================================

// END SNIPLATE

// BEGIN SNIPLATE fenwick 
// {{ abbr: Fenwick Tree }}
// {{ class: DataStructure }}
// {{ pattern: class\sFenwickTree$ }}
// Fenwick Tree ( Binary Indexed Tree )
// This class published at https://github.com/torus711/cplib-cpp/DataStructure.cpp
template < typename T >
class FenwickTree 
{
private:
	const int N;
	std::vector< T > data;

public:
	FenwickTree( const int n ) : N( n ), data( N + 1, 0 ) {}

	T sum( int i ) const
	{
		++i;
		T res = 0;
		while ( 0 < i )
		{
			res += data[i];
			i -= i & -i;
		}
		return res;
	}

	T sum( const int l, const int r ) const
	{
		return l ? sum( r - 1 ) - sum( l - 1 ) : sum( r - 1 );
	}

	void add( int i, const T x )
	{
		++i;
		while ( i <= N )
		{
			data[i] += x;
			i += i & -i;
		}
		return;
	}

	void change( const int i, const T x )
	{
		add( i, x - ( sum( i ) - ( i ? sum( i - 1 ) : 0 ) ) );
		return;
	}

	int lower_bound( T v ) const
	{
		int x = 0;
		for ( int k = 1 << ( 31 - __builtin_clz( N ) ); k; k >>= 1 )
		{
			if ( x + k <= N && data[ x + k ] < v )
			{
				x += k;
				v -= data[x];
			}
		}
		return x;
	}
};
// FenwickTree( array size )
// int sum( 0-indexed pos )
// int sum( [ l, r )
// int add( pos, value )
// int change( pos, value )
// int lower_bound( value )
// ==========================================

// END SNIPLATE

// BEGIN SNIPLATE fenwick2d 
// {{ abbr: Fenwick Tree for 2D }}
// {{ class: DataStructure }}
// {{ pattern: class\sFenwickTree2d$ }}
// This class published at https://github.com/torus711/cplib-cpp/DataStructure.cpp
class FenwickTree2d
{
private:
	const int H, W;
	vector< vector<int> > data;

public:
	FenwickTree2d( const int h, const int w ) : H( h + 1 ), W( w + 1 ), data( H + 1, vector<int>( W + 1, 0 ) ) {}

	void add( int y, int x, const int w )
	{
		++y, ++x;
		for ( ; y <= H; y += y & -y )
		{
			for ( ; x <= W; x += x & -x )
			{
				data[y][x] += w;
			}
		}

		return;
	}

	int sum( int y, int x )
	{
		++y, ++x;
		int res = 0;
		for ( ; 0 < y; y -= y & -y )
		{
			for ( ; 0 < x; x -= x & -x )
			{
				res += data[y][x];
			}
		}

		return res;
	}

	int sum( const int y1, const int x1, const int y2, const int x2 )
	{
		return sum( y2, x2 ) - sum( y2, x1 - 1 ) - sum( y1 - 1, x2 ) + sum( y1 - 1, x1 - 1 );
	}
};
// binaryindexedtree2d( H, W )
// add( y, x, value )
// sum( y, x )
// sum( y1, x1, y2, x2 ) : ( y1 <= y2 , x1 <= x2 )
// ==========================================

// END SNIPLATE

// BEGIN SNIPLATE dendpque 
// {{ abbr: Double Ended Priority Queue }}
// {{ class: DataStructure }}
// {{ pattern: class\sDoubleEndedPriorityQueue$ }}
// Double Ended Priority Queue
// amortized O( log n )
// include : <vector> <functional>
template < typename T >
class DoubleEndedPriorityQueue
{
	priority_queue<T> maxHeap, delMax;
	priority_queue< T, vector<T>, greater<T> > minHeap, delMin;

public:
	void push( const T &v )
	{
		maxHeap.push( v );
		minHeap.push( v );
		return;
	}

	const T& min()
	{
		adjust();
		return minHeap.top();
	}

	const T& max()
	{
		adjust();
		return maxHeap.top();
	}

	void popmin()
	{
		delMax.push( minHeap.top() );
		minHeap.pop();
		return;
	}

	void popmax()
	{
		delMin.push( maxHeap.top() );
		maxHeap.pop();
		return;
	}
	
private:
	void adjust()
	{
		while ( !maxHeap.empty() && !delMax.empty() && maxHeap.top() == delMax.top() )
		{
			maxHeap.pop();
			delMax.pop();
		}
		while ( !minHeap.empty() && !delMin.empty() && minHeap.top() == delMin.top() )
		{
			minHeap.pop();
			delMin.pop();
		}
		return;
	}
};
// DoubleEndedPriorityQueue<T>
// push( v )
// min(), max(), popmin(), popmax()

// END SNIPLATE

// BEGIN SNIPLATE rangmqueryst 
// {{ abbr: Range Maximum Query by Sparse Table }}
// {{ class: DataStructure, SparseTable }}
namespace SparseTable
{
	// Sparse Table による RMQ
	// 構築 O( N log N ), クエリ O( 1 )
	template < typename T > class RangeMaximumQuery
	{
	private:
		mutable std::vector< int > clzs;

		const int N;
		const std::vector< T > src;

		std::vector< std::vector< T > > dp;

	public:
		RangeMaximumQuery( const std::vector< T > &s ) :
			clzs( N, -1 ),
			N( s.size() ),
			src( s ), dp( N, std::vector< T >( 32 - clz( N ) ) )
		{
			for ( int i = 0; i < N; ++i )
			{
				dp[i][0] = src[i];
			}

			for ( int j = 1; ( 1 << j ) <= N; ++j )
			{
				const int L = ( 1 << j );
				for ( int i = 0; i + L <= N; ++i )
				{
					dp[i][j] = max( dp[i][ j - 1 ], dp[ i + L / 2 ][ j - 1 ] );
				}
			}

			return;
		}

		T maximum( const int l, const int r ) const
		{
			const int L = r - l;
			const int k = 31 - clz( L );
			return max( dp[l][k], dp[ r - ( 1 << k ) ][k] );
		}

	private:
		int clz( const int n ) const
		{
			return clzs[n] == -1 ? ( clzs[n] = __builtin_clz( n ) ) : clzs[n];
		}
	};
	// RangeMaximumQuery( src )
	// minimum( [ l, r ) )
}

// END SNIPLATE

#include <random>
 
namespace RBST
{
	template < typename T >
	class set
	{
	private:
		// 乱数生成器
		std::mt19937 rng;
		std::uniform_int_distribution< int > rng_n;
		int rand( const int a, const int b )
		{
			rng_n.param( std::uniform_int_distribution< int >::param_type( a, b ) );
			return rng_n( rng );
		}
 
		// ノード
		struct Node
		{
			T value;
			Node *left = nullptr, *right = nullptr;
 
			unsigned int size_ = 1;
 
			Node( const T &v ) : value( v )
			{
				return;
			}
 
			~Node()
			{
				delete left;
				delete right;
				return;
			};
		};
 
		// 本体
		Node *root = nullptr;
 
	public:
		~set()
		{
			delete root;
		}
 
		unsigned int size() const
		{
			return size( root );
		}
 
		unsigned int size( Node * const t ) const
		{
			return t == nullptr ? 0 : t->size_;
		}
 
		Node *update( Node * const t )
		{
			t->size_ = size( t->left ) + size( t->right ) + 1;
			return t;
		}
 
		bool find( const T &v )
		{
			return find( root, v );
		}
 
		bool find( Node * const t, const T &v )
		{
			if ( t == nullptr )
			{
				return false;
			}
			if ( t->value == v )
			{
				return true;
			}
			return find( ( v < t->value ? t->left : t->right ), v );
		}
 
		Node *merge( Node *l, Node *r )
		{
			if ( l == nullptr || r == nullptr )
			{
				return l == nullptr ? r : l;
			}
 
			const int n = size( l ), m = size( r );
			if ( rand( 0, n + m ) < n )
			{
				l->right = merge( l->right, r );
				return update( l );
			}
			else
			{
				r->left = merge( l, r->left );
				return update( r );
			}
		}
 
		std::pair< Node*, Node* > split_by_index( Node *t, const unsigned int k )
		{
			if ( t == nullptr )
			{
				return std::make_pair( nullptr, nullptr );
			}
 
			if ( k <= size( t->left ) )
			{
				auto ts = split( t->left, k );
				t->left = ts.second;
				return std::make_pair( ts.first, update( t ) );
			}
			else
			{
				auto ts = split( t->right, k - size( t->left ) - 1 );
				t->right = ts.first;
				return std::make_pair( update( t ), ts.second );
			}
		}
 
		std::pair< Node*, Node* > split_by_value( Node *t, const T &v )
		{
			// v 以下の値をもつノードからなる部分木と，それ以外に分割する
 
			if ( t == nullptr )
			{
				return std::make_pair( nullptr, nullptr );
			}
 
			if ( t->value <= v )
			{
				auto ts = split_by_value( t->right, v );
				t->right = ts.first;
				return std::make_pair( update( t ), ts.second );
			}
			else
			{
				auto ts = split_by_value( t->left, v );
				t->left = ts.second;
				return std::make_pair( ts.first, update( t ) );
			}
		}
 
		bool insert( const T &v )
		{
			if ( find( v ) )
			{
				return false;
			}
 
			auto ts = split_by_value( root, v );
			root = merge( merge( ts.first, new Node( v ) ), ts.second );
 
			return true;
		}
 
		bool erase( const T &v )
		{
			if ( !find( v ) )
			{
				return false;
			}
 
			auto l = split_by_value( root, v - 1 );
			auto r = split_by_value( l.second, v );
 
			delete r.first;
 
			root = merge( l.first, r.second );
 
			return true;
		}
 
		T operator[]( const unsigned int k ) const
		{
			return nth_value( root , k );
		}
 
	private:
		T nth_value( Node * const t, const unsigned k ) const
		{
			if ( size( t->left ) == k )
			{
				return t->value;
			}
			else if ( k <= size( t->left ) )
			{
				return nth_value( t->left, k );
			}
			else
			{
				return nth_value( t->right, k - size( t->left ) - 1 );
			}
		}
 
	public:
		// テスト用
		void dump() const
		{
			return dump( root );
		}
 
		void dump( Node * const t ) const
		{
			if ( t == nullptr )
			{
				return;
			}
 
			dump( t->left );
			cerr << "set element : " << t->value << endl;
			dump( t->right );
 
			return;
		}
	};
}
