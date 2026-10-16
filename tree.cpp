// BEGIN SNIPLATE treediam 
// {{ abbr: calculate diameter of a tree }}
// {{ class: Graph, Tree }}
// {{ pattern: class\sTreeDiamiter }}
// 木の直径またはその端点を求める
// DFS 二回
// O( |V| )
// This class is published at https://github.com/torus711/cplib-cpp/tree.cpp
class TreeDiameter
{
private:
	const int N;
	std::vector< std::vector< std::pair< int, long long > > > G;
	std::vector< long long > distance;

public:

	TreeDiameter( const int n ) : N( n ), G( N ), distance( N, -1 ) {};

	void connect( const int u, const int v, const int d = 1 )
	{
		G[u].emplace_back( v, d );
		G[v].emplace_back( u, d );

		return;
	}

	long long diameter( const int r = 0 )
	{
		dfs( r, 0, -1 );
		const int u = max_element( distance.begin(), distance.end() ) - distance.begin();
		dfs( u, 0, -1 );
		return *max_element( distance.begin(), distance.end() );
	}

	pair<int,int> endpoints( const int r = 0 )
	{
		dfs( r, 0, -1 );
		const int u = max_element( distance.begin(), distance.end() ) - distance.begin();
		dfs( u, 0, -1 );
		const int v = max_element( distance.begin(), distance.end() ) - distance.begin();
		return make_pair( u, v );
	}

private:
	void dfs( const int u, const long long depth, const int p )
	{
		distance[u] = depth;

		for ( const auto &e : G[u] )
		{
			const int v = e.first;
			const long long d = e.second;

			if ( v == p )
			{
				continue;
			}
			dfs( v, depth + d, u );
		}

		return;
	}
};
// TreeDiameter( N )
// connect( a, b, cost )
// diameter() := 直径を返す
// endpoints() := 直径の端点の pair を返す

// END SNIPLATE

// BEGIN SNIPLATE  lca
// {{ abbr: calculate Lowest Common Ancestor of a tree }}
// {{ class: Graph, Tree }}
// {{ pattern: class\sLowestCommonAncestor }}
// 木の根を適当に固定して、最近共通祖先を求める
// 二点間距離もいける
// 前処理 O( N log N )
// クエリ O( log N )
// This class is published at https://github.com/torus711/cplib-cpp/tree.cpp
class LowestCommonAncestor
{
private:
	const int N;
	std::vector< VPII > G;

	bool constructed = false;
	std::vector<int> depth_, distances_;
	std::vector< std::vector<int> > parent_;

public:
	LowestCommonAncestor( const int n ) : N( n ), G( N ), depth_( N ), distances_( N ), parent_( N, std::vector<int>( 31, -1 ) )
	{
		return;
	}

	void connect( const int u, const int v, const int d = 1 )
	{
		G[u].EB( v, d );
		G[v].EB( u, d );
		constructed = false;

		return;
	}

	int depth( const int u ) const
	{
		return depth_[u];
	}

	int solve( int u, int v )
	{
		if ( !constructed )
		{
			dfs( 0, -1, 0, 0 );
			doubling();
			constructed = true;
		}

		if ( depth_[u] < depth_[v] )
		{
			swap( u, v );
		}

		{ // align depths
			int diff = depth_[u] - depth_[v];
			for ( int i = 30; 0 <= i; --i )
			{
				if ( diff & 1 << i )
				{
					u = parent_[u][i];
				}
			}
		}

		if ( u == v )
		{
			return u;
		}

		for ( int i = 30; 0 <= i; --i )
		{
			if ( parent_[u][i] != parent_[v][i] )
			{
				u = parent_[u][i];
				v = parent_[v][i];
			}
		}

		return parent_[u][0];
	}

	int distance( const int u, const int v )
	{
		const int w = solve( u, v );
		return distances_[u] + distances_[v] - 2 * distances_[w];
	}

private:
	void dfs( const int u, const int prev, const int d, const int dist )
	{
		depth_[u] = d;
		parent_[u][0] = prev;
		distances_[u] = dist;

		for ( const PII &e : G[u] )
		{
			const int v = e.fst;
			if ( v == prev )
			{
				continue;
			}

			dfs( v, u, d + 1, dist + e.snd );
		}

		return;
	}

	void doubling()
	{
		for ( int i = 0; i < 30; ++i )
		{
			for ( int u = 0; u < N; ++u )
			{
				if ( parent_[u][i] != -1 )
				{
					parent_[u][ i + 1 ] = parent_[ parent_[u][i] ][i];
				}
			}
		}
		return;
	}
};
// LowestCommonAncestor( N )
// connect( u, v [, dist = 1 ] )
// solve( u, v )
// depth( u )
// distance( u, v )

// END SNIPLATE
