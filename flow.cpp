// 最大流（ Ford-Fulkerson 法 ） O( F |E| )
class FordFulkerson
{
private:
	struct Edge
	{
		int to, cap, rev;
		Edge( int t, int c, int r ) : to( t ), cap( c ), rev( r ) {}
	};

	vector< vector<Edge> > G;
	vector<bool> used;

public:
	FordFulkerson( const int v ) : G( v ), used( v )
	{
		return;
	}

	void connect( int from, int to, int cap )
	{
		G[ from ].push_back( Edge( to, cap, G[ to ].size() ) );
		G[ to ].push_back( Edge( from, 0, G[ from ].size() - 1 ) );
		return;
	}

	int solve( int s, int t )
	{
		int res = 0;
		while ( true )
		{
			fill( used.begin(), used.end(), false );
			int f = dfs( s, t );

			if ( f == 0 )
			{
				break;
			}

			res += f;
		}
		return res;
	}

private:
	int dfs( int v, int t, int f = INT_MAX )
	{
		if ( v == t )
		{
			return f;
		}

		used[v] = true;
		for ( int i = 0; i < (int)G[v].size(); ++i )
		{
			Edge &e = G[v][i];
			if ( !used[ e.to ] && 0 < e.cap )
			{
				int d = dfs( e.to, t, min( f, e.cap ) );
				if ( 0 < d )
				{
					e.cap -= d;
					G[ e.to ][ e.rev ].cap += d;
					return d;
				}
			}
		}
		return 0;
	}
};
// FordFulkerson( |V| )
// connect( from, to, cap )
// solve( s, t )

// BEGIN SNIPLATE dinic 
// {{ abbr: Maxflow by Dinic's Algorithm }}
// {{ class: Graph, Flow }}
// {{ pattern: class\sDinic }}
// 最大流（ Dinic 法 ） O( |E||V|^2 )
// This class is published ad https://github.com/torus711/cplib-cpp/flow.cpp
class Dinic
{
	struct Edge
	{
		int to, cap, rev;
		Edge( int t, int c, int r ) : to( t ), cap( c ), rev( r ) {}
	};

	vector< vector<Edge> > G;
	vector<int> distance, done;

public:
	Dinic( int V ) : G( V ), distance( V ), done( V )
	{
		return;
	}

	void connect( int from, int to, int cost )
	{
		G[ from ].push_back( Edge( to, cost, G[ to ].size() ) );
		G[ to ].push_back( Edge( from, 0, G[ from ].size() - 1 ) );
		return;
	}

	int solve( int s, int t )
	{
		int res = 0;
		while ( true )
		{
			bfs( s );
			if ( distance[t] < 0 )
			{
				return res;
			}

			fill( done.begin(), done.end(), 0 );
			for ( int f; ( f = dfs( s, t, numeric_limits<int>::max() ) ) > 0; res += f );
		}
	}

private:
	void bfs( int s )
	{
		fill( distance.begin(), distance.end(), -1 );
		distance[s] = 0;
		
		queue<int> que;
		que.push( s );
		while ( !que.empty() )
		{
			int v = que.front();
			que.pop();

			for ( int i = 0; i < (int)G[v].size(); ++i )
			{
				Edge &e = G[v][i];
				if ( e.cap > 0 && distance[ e.to ] < 0 )
				{
					distance[ e.to ] = distance[v] + 1;
					que.push( e.to );
				}
			}
		}

		return;
	}

	int dfs( int v, int t, int f )
	{
		if ( v == t )
		{
			return f;
		}

		for ( int &i = done[v]; i < (int)G[v].size(); ++i )
		{
			Edge &e = G[v][i];
			if ( e.cap > 0 && distance[v] < distance[ e.to ] )
			{
				int d = dfs( e.to, t, min( f, e.cap ) );
				if ( d > 0 )
				{
					e.cap -= d;
					G[ e.to ][ e.rev ].cap += d;
					return d;
				}
			}
		}

		return 0;
	}
};
// Dinic( |V| )
// connect( from, to, cap )
// solve( s, t )

// END SNIPLATE

// BEGIN SNIPLATE bmatching 
// {{ abbr: Bipartite Matching }}
// {{ class: Graph, Flow }}
// {{ pattern: class\sBipartiteMatching }}
// 二部グラフ最大マッチング O( |V||E| )
// This class is published ad https://github.com/torus711/cplib-cpp/flow.cpp
class BipartiteMatching
{
private:
	const int V;
	vector< vector<int> > G;
	vector<int> match;
	vector<bool> used;

public:
	BipartiteMatching( int v ) : V( v ), G( V ), match( V, -1 ), used( V )
	{
		return;
	}

	void connect( int u, int v )
	{
		G[u].push_back( v );
		G[v].push_back( u );
		return;
	}

	int solve()
	{
		int res = 0;
		fill( match.begin(), match.end(), -1 );

		for ( int v = 0; v < V; ++v )
		{
			if ( match[v] == -1 )
			{
				fill( used.begin(), used.end(), false );
				if ( dfs( v ) )
				{
					res++;
				}
			}
		}

		return res;
	}

private:
	int dfs( int v )
	{
		used[v] = true;

		for ( int i = 0; i < (int)G[v].size(); ++i )
		{
			int u = G[v][i], w = match[u];
			if ( w == -1 || ( !used[w] && dfs( w ) ) )
			{
				match[v] = u;
				match[u] = v;
				return true;
			}
		}

		return false;
	}
};
// BipartiteMatching( |V| )
// connect( u, v )
// solve()

// END SNIPLATE

// BEGIN SNIPLATE mcflow 
// {{ abbr: Minimum Cost Flow by Primal-Dual }}
// {{ class: Graph, Flow }}
// {{ pattern: class\sBipartiteMatching }}
// 最小費用流 O( F |E| log |V| )
// This class is published ad https://github.com/torus711/cplib-cpp/flow.cpp
class MinimumCostFlow
{
private:
	struct Edge
	{
		int to, cap, cost, rev;
		Edge( int t, int c, int d, int r ) : to( t ), cap( c ), cost( d ), rev( r ) {}
	};

	const int V;
	vector< vector<Edge> > G;

public:
	MinimumCostFlow( int v ) : V( v ), G( V ) {};

	void connect( int from, int to, int cap, int cost )
	{
		G[ from ].push_back( Edge( to, cap, cost, G[ to ].size() ) );
		G[ to ].push_back( Edge( from, 0, -cost, G[ from ].size() - 1 ) );
		return;
	}

	int solve( int s, int t, int f )
	{
		int res = 0;
		vector<int> h( V, 0 ), prevv( V ), preve( V );

		while ( 0 < f )
		{
			vector<int> distance( V, INT_MAX );
			distance[s] = 0;

			priority_queue< pair<int,int>, vector< pair<int,int> >, greater< pair<int,int> > > que;
			que.push( make_pair( 0, s ) );
			
			while ( !que.empty() )
			{
				int d = que.top().first;
				int v = que.top().second;
				que.pop();

				if ( distance[v] < d )
				{
					continue;
				}

				for ( int i = 0; i < (int)G[v].size(); ++i )
				{
					Edge &e = G[v][i];
					if ( 0 < e.cap && distance[v] + e.cost + h[v] - h[ e.to ] < distance[ e.to ] )
					{
						distance[ e.to ] = distance[v] + e.cost + h[v] - h[ e.to ];
						prevv[ e.to ] = v;
						preve[ e.to ] = i;
						que.push( make_pair( distance[ e.to ], e.to ) );
					}
				}
			}

			if ( distance[t] == INT_MAX )
			{
				return -1;
			}

			for ( int i = 0; i < V; ++i )
			{
				h[i] += distance[i];
			}

			int d = f;
			for ( int v = t; v != s; v = prevv[v] )
			{
				d = min( d, G[ prevv[v] ][ preve[v] ].cap );
			}
			f -= d;
			res += d * h[t];
			for ( int v = t; v != s; v = prevv[v] )
			{
				Edge &e = G[ prevv[v] ][ preve[v] ];
				e.cap -= d;
				G[v][ e.rev ].cap += d;
			}
		}

		return res;
	}

	int solve2( const int s, const int t, int f )
	{
		const int INF = INT_MAX / 2;
		int res = 0;

		while ( f > 0 )
		{
			vector<int> dist( V, INF );
			dist[s] = 0;
			vector<int> prevv( V ), preve( V );

			bool update = true;
			while ( update )
			{
				update = false;

				for ( int v = 0; v < V; v++ )
				{
					if ( dist[v] == INF )
					{
						continue;
					}
					for ( int i = 0; i < (int)G[v].size(); i++ )
					{
						Edge &e = G[v][i];
						if ( e.cap > 0 && dist[ e.to ] > dist[v] + e.cost )
						{
							dist[ e.to ] = dist[v] + e.cost;
							prevv[ e.to ] = v;
							preve[ e.to ] = i;
							update = true;
						}
					}
				}
			}

			if ( dist[t] == INF )
			{
				return -1;
			}

			int d = f;
			for ( int v = t; v != s; v = prevv[v] )
			{
				d = min( d, G[ prevv[v] ][ preve[v] ].cap );
			}
			f -= d;
			res += d * dist[t];
			for ( int v = t; v != s; v = prevv[v] )
			{
				Edge &e = G[ prevv[v] ][ preve[v] ];
				e.cap -= d;
				G[v][ e.rev ].cap += d;
			}
		}

		return res;
	}
};
// MinimumCostFlow( |V| )
// connect( from, to, cap, cost )
// solve( s, t, f ) :  Primal-Dual O( F |E| log |V| )
// solve2( s, t, f ) : Bellman-Ford O( F |E| |V| )

// END SNIPLATE
