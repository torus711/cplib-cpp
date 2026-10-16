// Å’·‹¤’Ê•”•ª—ñ‚ğ‹‚ß‚é
// O( NM )
// This function is published at https://github.com/torus711/cplib-cpp/dynamic_programming.cpp
template < typename RandomAccessIterator, typename OutputIterator >
OutputIterator longest_common_subsequence(
		RandomAccessIterator first1, RandomAccessIterator last1, 
		RandomAccessIterator first2, RandomAccessIterator last2,
		OutputIterator result )
{
	using T = typename std::iterator_traits< RandomAccessIterator >::value_type;
	const int N = std::distance( first1, last1 ), M = std::distance( first2, last2 );

	std::vector< std::vector<int> > dp( N + 1, std::vector<int>( M + 1, 0 ) );
	std::vector< std::vector< std::pair<int,int> > > prev( N + 1, std::vector< std::pair<int,int> >( M + 1, std::make_pair( -1, -1 ) ) );

	for ( int i = 0; i < N; ++i )
	{
		for ( int j = 0; j < M; ++j )
		{
			if ( *( first1 + i ) == *( first2 + j ) )
			{
				dp[ i + 1 ][ j + 1 ] = dp[i][j] + 1;
				prev[ i + 1 ][ j + 1 ] = std::make_pair( i, j );
			}
			else
			{
				if ( dp[ i + 1 ][j] < dp[i][ j + 1 ] )
				{
					dp[ i + 1 ][ j + 1 ] = dp[i][ j + 1 ];
					prev[ i + 1 ][ j + 1 ] = std::make_pair( i, j + 1 );
				}
				else
				{
					dp[ i + 1 ][ j + 1 ] = dp[ i + 1 ][j];
					prev[ i + 1 ][ j + 1 ] = std::make_pair( i + 1, j );
				}
			}
		}
	}

	std::vector<T> lcs;
	lcs.reserve( dp[N][M] );
	for ( std::pair<int,int> current( N, M ); prev[ current.first ][ current.second ].first != -1; current = prev[ current.first ][ current.second ] )
	{
		if ( dp[ prev[ current.first ][ current.second ].first ][ prev[ current.first ][ current.second ].second ] < dp[ current.first ][ current.second ] )
		{
			lcs.push_back( *( first1 + prev[ current.first ][ current.second ].first ) );
		}
	}

	std::reverse_copy( lcs.begin(), lcs.end(), result );
	return result;
}

// Å’·’P’²‘‰Á•”•ª—ñ‚ğ‹‚ß‚é
// O( n log n )
// This function is published at https://github.com/torus711/cplib-cpp/dynamic_programming.cpp
template < typename ForwardIterator,
		 typename T = typename std::iterator_traits< ForwardIterator >::value_type,
		 typename Compare = std::less< T > >
int longest_increasing_subsequence(
		ForwardIterator first, ForwardIterator last,
		const bool ac_eq = false,
		const T identify = numeric_limits< T >::max() )
{
	const int N = std::distance( first, last );

	std::vector<int> dp( N, identify );
	for ( ; first != last; ++first )
	{
		if ( ac_eq )
		{
			*std::upper_bound( dp.begin(), dp.end(), *first ) = *first;
		}
		else
		{
			*std::lower_bound( dp.begin(), dp.end(), *first ) = *first;
		}
	}
	
	return std::distance( dp.begin(), std::lower_bound( dp.begin(), dp.end(), identify ) );
}
