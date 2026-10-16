// BEGIN SNIPLATE split 
// {{ abbr: Split stirng by delimiter }}
// {{ class: String }}
// {{ pattern: vector<string>\ssplit }}
// 文字列 split / デリミタに含まれるいずれかの文字で分割
vector<string> split( const string &str, const string &delim = " " )
{
	int start = 0;
	vector<string> result;
	
	for ( int pos; ( pos = str.find_first_of( delim, start ) ) != (int)string::npos; start = pos + 1 )
	{
		result.push_back( str.substr( start, pos - start ) );
	}
	result.push_back( str.substr( start ) );

	result.erase(
			remove_if( result.begin(), result.end(), not1( mem_fun_ref( &string::size ) ) ),
			result.end() );

	return result;
}

// END SNIPLATE

// BEGIN SNIPLATE rollinghash 
// {{ abbr: Rolling Hash for a string }}
// {{ class: String }}
// ローリングハッシュ
// S に含まれる T の始点を列挙
template < typename Type >
std::vector< int > rollingHash( const std::vector< Type > &S, const std::vector< Type > &T, const int param = 0 )
{
// 	using ULL = unsigned long long;

	const ULL B = vector<ULL>( { 1000000007, 999999937, 100000007 } )[ param ];

	const int len_s = S.size(), len_t = T.size();
	if ( len_s < len_t )
	{
		return {};
	}

	ULL t = 1;
	for ( int i = 0; i < len_t; ++i )
	{
		t *= B;
	}

	ULL hash_t = 0, hash_s = 0;
	for ( int i = 0; i < len_t; ++i )
	{
		hash_t = hash_t * B + T[i];
		hash_s = hash_s * B + S[i];
	}

	std::vector< int > result;
	for ( int i = 0; i + len_t <= len_s; ++i )
	{
		if ( hash_t == hash_s )
		{
			result.push_back( i );
		}

		if ( i + len_t < len_s )
		{
			hash_s = hash_s * B + S[ i + len_t ] - S[i] * t;
		}
	}

	return result;
}

// END SNIPLATE

// VI construct_SA( const string &S )
// {
// 	const int L = SZ( S );
// 
// 	VI sa( L + 1 ), rank( L + 1 );
// 	REP( i, L + 1 )
// 	{
// 		sa[i] = i;
// 		rank[i] = i < L ? S[i] : -1;
// 	}
// 
// 	int k = 1;
// 	VI next( rank );
// 	const auto comp = [&]( const int i, const int j )
// 	{
// 		if ( rank[i] != rank[j] )
// 		{
// 			return rank[i] < rank[j];
// 		}
// 
// 		const int ri = i + k <= L ? rank[ i + k ] : -1;
// 		const int rj = j + k <= L ? rank[ j + k ] : -1;
// 		return ri < rj;
// 	};
// 
// 	for ( k = 1; k <= L; k *= 2 )
// 	{
// 		sort( ALL( sa ), comp );
// 
// 		next[ sa[0] ] = 0;
// 		REP( i, 1, L + 1 )
// 		{
// 			next[ sa[i] ] = next[ sa[ i - 1 ] ] + comp( sa[ i - 1 ], sa[i] );
// 		}
// 
// 		copy( ALL( next ), begin( rank ) );
// 	}
// 
// 	return move( sa );
// }

// BEGIN SNIPLATE manacher
// {{ abbr: Manacher }}
// {{ class: String }}
// reference: https://snuke.hatenablog.com/entry/2014/12/02/235837
// This function is published at https://github.com/torus711/cplib-cpp/string.cpp
std::vector< int > manacher( const string &S )
{
	const int N = S.size();
	std::vector< int > res( N );
	for ( int i = 0, j = 0; i < N; )
	{
		while ( 0 <= i - j  && i + j < N && S[ i - j ] == S[ i + j ] ) ++j;
		res[i] = j;
		int k = 1;
		while ( 0 <= i - k && k + res[ i - k ] < j )
		{
			res[ i + k ] = res[ i - k ];
			++k;
		}
		i += k;
		j -= k;
	}
	return res;
}

// END SNIPLATE
