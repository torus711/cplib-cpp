#include "./../_.cpp"

/************/
/* 最短経路 */
/************/


// BEGIN SNIPLATE dijkstra 
// {{ abbr: Dijkstra's algorithm }}
// {{ class: Algorithm, Graph }}
// {{ pattern: class\sDijkstra }}
// Dijstra 法による単一始点最短経路問題ソルバ
#include <vector>
#include <queue>
#include <utility>
#include <limits>
#include <type_traits>
namespace ShortestPath
{
	template < typename DIST_TYPE = int, typename = typename std::enable_if< std::is_arithmetic< DIST_TYPE >::value >::type >
	class Dijkstra
	{
	private:
		const int V;
		std::vector< std::vector< std::pair< int, DIST_TYPE > > > G;

	public:
		static DIST_TYPE inf()
		{
			return std::numeric_limits< DIST_TYPE >::max();
		}

		Dijkstra( const int v ) : V( v ), G( V )
		{
			return;
		}

		void connect( const int u, const int v, const DIST_TYPE d )
		{
			G[u].emplace_back( v, d );
			return;
		}

		std::vector< DIST_TYPE > solve( const int s, const DIST_TYPE d0 = 0 )
		{
			std::vector< DIST_TYPE > distances( V, inf() );
			distances[s] = d0;

			std::priority_queue<
				std::pair< DIST_TYPE, int >,
				std::vector< std::pair< DIST_TYPE, int > >,
				std::greater< std::pair< DIST_TYPE, int > > > que;
			que.emplace( d0, s );

			while ( !que.empty() )
			{
				const DIST_TYPE dist = que.top().first;
				const int u = que.top().second;
				que.pop();

				if ( distances[u] < dist )
				{
					continue;
				}

				for ( auto &e : G[u] )
				{
					const int v = e.first;
					const DIST_TYPE d = e.second;

					if ( distances[u] + d < distances[v] )
					{
						que.emplace( distances[v] = distances[u] + d, v );
					}
				}
			}

			return distances;
		}
	};
}
// ShortestPath::Dijkstra( |V| )
// connect( from, to, cost )
// solve( s, d0 = 0 )

// END SNIPLATE

// BEGIN SNIPLATE floydwarshall
// {{ abbr: Floyd-Warshall }}
// {{ class: Algorithm, Graph }}
// {{ pattern: class\sFloydWarshall }}
template < typename T >
class FloydWarshall
{
	int N;
	std::vector< std::vector< T > > G;
	bool solved = true;

public:
	FloydWarshall( const int n ) :
		N( n ), G( N, std::vector( N, std::numeric_limits< T >::max() / 4 ) )
	{
		for ( int i = 0; i < N; ++i )
		{
			G[i][i] = 0;
		}
		return;
	}

	void connect( const int u, const int v, const T c )
	{
		G[u][v] = std::min( G[u][v], c );
		solved = false;
		return;
	}

	void solve()
	{
		for ( int k = 0; k < N; ++k )
		{
			for ( int i = 0; i < N; ++i )
			{
				for ( int j = 0; j < N; ++j )
				{
					G[i][j] = std::min( G[i][j], G[i][k] + G[k][j] );
				}
			}
		}

		solved = true;

		return;
	}

	T distance( const int u, const int v )
	{
		if ( !solved )
		{
			solve();
		}

		return G[u][v];
	}

	void update( const int u, const int v, const T c )
	{
		G[u][v] = std::min( G[u][v], c );
		for ( int i = 0; i < N; ++i )
		{
			for ( int j = 0; j < N; ++j )
			{
				G[i][j] = std::min( G[i][j], G[i][u] + c + G[v][j] );
				G[i][j] = std::min( G[i][j], G[i][v] + c + G[u][j] );
			}
		}

		return;
	}
};

// END SNIPLATE

/**************/
/* 最小全域木 */
/**************/

// BEGIN SNIPLATE prim 
// {{ abbr: Prims's algorithm }}
// {{ class: Algorithm, Graph }}
// {{ pattern: class\sPrim }}
// Prim 法による MST
#include <vector>
#include <utility>
#include <queue>
#include <limits>
#include <type_traits>
namespace MinimumSpanningTree
{
	template < typename COST_TYPE = int, typename = typename std::enable_if< std::is_arithmetic< COST_TYPE >::value >::type >
	class Prim
	{
		using Edge = std::pair< COST_TYPE, int >;
		const COST_TYPE INF = std::numeric_limits< COST_TYPE >::max() / 2;

		const int N;
		std::vector< std::vector< Edge > > G;

	public:
		Prim( const int n ) : N( n ), G( N )
		{
			return;
		}

		void connect( const int u, const int v, COST_TYPE c )
		{
			G[u].emplace_back( c, v );
			G[v].emplace_back( c, u );

			return;
		}

		COST_TYPE solve()
		{
			std::vector< COST_TYPE > costs( N, INF );
			costs[0] = 0;

			std::priority_queue< Edge, std::vector< Edge >, greater< Edge > > que;
			que.emplace( 0, 0 );

			COST_TYPE result = 0;

			while ( !que.empty() )
			{
				const COST_TYPE cost = que.top().first;
				const int u = que.top().second;
				que.pop();

				if ( costs[u] < cost )
				{
					continue;
				}

				result += cost;

				for ( Edge &e : G[u] )
				{
					const COST_TYPE c = e.first;
					const int v = e.second;

					if ( c < costs[v] )
					{
						que.emplace( costs[v] = c, v );
					}
				}
			}

			return result;
		}
	};
}
// MinimumSpanningTree::Prim( |V| )
// connect( u, v, cost )
// solve()

// END SNIPLATE


/******************/
/* 一般的なグラフ */
/******************/

// BEGIN SNIPLATE isbipartite 
// {{ abbr: dicision whether graph G is bipartite }}
// {{ class: Algorithm, Graph }}
// {{ pattern: class\sIsBipartite }}
// 二部グラフ判定
// O( |V| + |E| )
// This class is published at https://github.com/torus711/cplib-cpp/graph.cpp
class IsBipartite
{
private:
	const int N;

	vector< vector<int> > G;
	vector<int> colors;

public:
	IsBipartite( const int n ) : N( n ), G( n ), colors( N ) {};

	void connect( const int u, const int v )
	{
		G[u].push_back( v );
		G[v].push_back( u );
	}

	bool solve()
	{
		fill( colors.begin(), colors.end(), -1 );

		for ( int u = 0; u < N; ++u )
		{
			if ( colors[u] == -1 && !paint( u, 1 ) )
			{
				return false;
			}
		}

		return true;
	}

	bool color( const int v ) const
	{
		return colors[v];
	}

private:
	bool paint( const int u, const int c )
	{
		if ( colors[u] != -1 )
		{
			return colors[u] == c;
		}

		colors[u] = c;

		for ( const int v : G[u] )
		{
			if ( !paint( v, 1 - c ) )
			{
				return false;
			}
		}

		return true;
	}
};
// IsBipartite( N )
// connect( a, b )
// solve()
// color( v ) // required to be solved

// END SNIPLATE

// BEGIN SNIPLATE toposort 
// {{ abbr: Topological Sort for Graph }}
// {{ class: Algorithm, Graph }}
// {{ pattern: class\sTopologicalSort }}
// トポロジカルソート
// O( |V| + |E| )
class TopologicalSort
{
private:
	const int N;
	vector< vector<int> > G;

	vector<int> color, result;

public:
	TopologicalSort( const int n ) : N( n ), G( N ) {};

	void connect( const int u, const int v )
	{
		G[u].push_back( v );
		
		return;
	}

	vector<int> solve()
	{
		color = vector<int>( N );
		result.clear();

		for ( int v = 0; v < N; ++v )
		{
			if ( !color[v] && !dfs( v ) )
			{
				return vector<int>();
			}
		}

		reverse( result.begin(), result.end() );
		return result;
	}

private:
	bool dfs( const int u )
	{
		color[u] = 1;

		for ( const int v : G[u] )
		{
			switch ( color[v] )
			{
				case 0:
					if ( !dfs( v ) )
					{
						return false;
					}
					break;
				case 1:
					return false;
				case 2:
					continue;
			}
		}

		color[u] = 2;
		result.push_back( u );

		return true;
	}
};
// TopologicalSort( |V| )
// connect( u, v )
// solve() := ソート後の頂点順序を格納した vector / DAG でない場合は空の vector

// END SNIPLATE

// BEGIN SNIPLATE scc 
// {{ abbr: Strongly Connected Component for a Graph }}
// {{ class: Algorithm, Graph }}
// {{ pattern: class\sStronglyConnectedComponents }}
// 強連結成分分解 / Strongly Connected Component
// O( |V| + |E| )
// This class is published at https://github.com/torus711/cplib-cpp/graph.cpp
class StronglyConnectedComponents
{
private:
	const int N;

	vector< vector<int> > G, Gr;

	vector<int> vs;
	vector<bool> used;
	vector<int> component_of;

public:
	StronglyConnectedComponents( const int n ) : N( n ), G( N ), Gr( N ), used( N ), component_of( N ) {};

	void connect( const int from, const int to )
	{
		G[ from ].push_back( to );
		Gr[ to ].push_back( from );

		return;
	}

	int solve()
	{
		fill( used.begin(), used.end(), false );
		vs.clear();
		for ( int v = 0; v < N; v++ )
		{
			if ( !used[v] )
			{
				dfs( v );
			}
		}

		fill( used.begin(), used.end(), false );
		reverse( vs.begin(), vs.end() );
		int k = 0;
		for ( const int v : vs )
		{
			if ( !used[v] )
			{
				rdfs( v, k++ );
			}
		}
		return k;
	}

	vector<int> componentNumbers()
	{
		return component_of;
	}

private:
	void dfs( const int u )
	{
		used[u] = true;
		for ( const int v : G[u] )
		{
			if ( !used[v] )
			{
				dfs( v );
			}
		}
		vs.push_back( u );

		return;
	}

	void rdfs( const int u, const int k )
	{
		used[u] = true;
		component_of[u] = k;
		for ( const int v : Gr[u] )
		{
			if ( !used[v] )
			{
				rdfs( v, k );
			}
		}

		return;
	}
};
// connect( from, to )
// solve() : 連結成分の数が返る
// componentNumbers() : 頂点番号→連結成分の番号 の表が返る
// 連結成分の番号の大小は、トポロジカル順序の大小と一致

// END SNIPLATE

// 最大独立集合
// O( n * 1.466^n )
int independence_set( vector< vector< int > > &G, const int u = 0, long long vanished = 0 )
{
	const int N = G.size();
	if ( u == N )
	{
		return 0;
	}

	if ( vanished & 1LL << u )
	{
		return independence_set( G, u + 1, vanished );
	}

	int res = 0, degree = 0;
	long long nex_vanished = vanished;

	// use u
	for ( const auto &&v : G[u] )
	{
		nex_vanished |= 1LL << v;
		degree += !( vanished & 1LL << v );
	}
	nex_vanished |= 1LL << u;
	res = max( res, 1 + independence_set( G, u + 1, nex_vanished ) );

	if ( degree <= 1 )
	{
		return res;
	}

	// no use u
	res = max( res, independence_set( G, u + 1, vanished | 1LL << u ) );
	return res;
}
