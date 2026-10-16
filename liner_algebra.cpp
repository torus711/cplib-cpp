#include <limits>

// BEGIN SNIPLATE matrix_types 
// {{ abbr: type aliases for matrix calculation }}
// {{ class: linearalgebra }}
// {{ pattern: using\sVector\s=\svector }}
// {{ invisible }}
template < typename Number > using Vector = vector< Number >;
template < typename Number > using Matrix = vector< vector< Number > >;

// END SNIPLATE

// BEGIN SNIPLATE unitvector 
// {{ abbr: Unit Vector }}
// {{ class: linearalgebra }}
// {{ pattern: Matrix<\sNumber\s>\sidentify_matrix }}
// 単位行列
// O( N )
template < typename Number >
Matrix< Number > identify_matrix( const int N )
{
	Matrix< Number > res( N, Vector< Number >( N ) );
	for ( int i = 0; i < N; ++i )
	{
		res[i][i] = 1;
	}
	return res;
}

// END SNIPLATE

// トレース
// O( N )
template < typename Number >
Number trace( const Matrix< Number > &a )
{
	Number res;
	for ( int i = 0; i < static_cast<int>( a.size() ); ++i )
	{
		res = a[i][i];
	}
	return res;
}

// ベクトルの和
// O( N )
template < typename Number >
const Vector< Number > operator+( const Vector< Number > &a, const Vector< Number > &b )
{
	assert( a.size() == b.size() );

	Vector< Number > res;
	res.reserve( a.size() );
	transform( a.begin(), a.end(), b.begin(), back_inserter( res ), plus< Number >() );
	return res;
}

// 行列の和
// O( NM )
template < typename Number >
const Matrix< Number > operator+( const Matrix< Number > &a, const Matrix< Number > &b )
{
	if ( a.size() != b.size() || a[0].size() != b[0].size() )
	{
		throw "Matrix + Matrix : size error";
	}

	Matrix< Number > res( a.size(), Vector< Number >( a[0].size() ) );
	for ( int i = 0; i < static_cast<int>( a.size() ); ++i )
	{
		for ( int j = 0; j < static_cast<int>( a[0].size() ); ++j )
		{
			res[i][j] = a[i][j] + b[i][j];
		}
	}
	return res;
}

// ベクトルの定数倍
// O( N )
template < typename Number >
inline const Vector< Number > operator*( Vector< Number > v, const Number c )
{
	transform( v.begin(), v.end(), v.begin(), bind( multiplies< Number >(), placeholders::_1, c ) );
	return v;
}
template < typename Number >
inline const Vector< Number > operator*( const Number c, Vector< Number > v )
{
	return v * c;
}

// 行列の定数倍
// O( NM )
template < typename Number >
const Matrix< Number > operator*( Matrix< Number > a, const Number c )
{
	transform( a.begin(), a.end(), a.begin(),
		bind( function< const Vector< Number >( Vector< Number >, const Number ) >( static_cast< const Vector< Number >(*)( Vector< Number >, const Number ) >( &operator* ) ), placeholders::_1, c ) );
	return a;
}
template < typename Number >
const Matrix< Number > operator*( const Number c, Matrix< Number > a )
{
	return a * c;
}

// ベクトル×行列
// O( NM )
template < typename Number >
Vector< Number > operator*( const Vector< Number > &v, const Matrix< Number > &m )
{
	if ( v.size() != m.size() )
	{
		throw "Vector * Matrix : size error";
	}

	Vector< Number > res( v.size() );
	for ( int i = 0; i < static_cast<int>( v.size() ); ++i )
	{
		for ( int j = 0; j < static_cast<int>( m.size() ); ++j )
		{
			res[i] += v[j] * m[j][i];
		}
	}
	return res;
}

// 行列×ベクトル
template < typename Number >
Vector< Number > operator*( const Matrix< Number > &m, const Vector< Number > &v )
{
	if ( m[0].size() != v.size() )
	{
		throw "Matrix * Vector : size error";
	}

	Vector< Number > res( m.size() );
	for ( int i = 0; i < static_cast<int>( m.size() ); ++i )
	{
		for ( int k = 0; k < static_cast<int>( v.size() ); ++k )
		{
			res[i] += m[i][k] * v[k];
		}
	}
	return res;
}

// 行列積のためのヘルパ関数
double modulo( const double a, const long long mod )
{
	return a;
}
int modulo( const int a, const long long mod )
{
	return a % mod;
}

// BEGIN SNIPLATE matmul 
// {{ abbr: Multiplies of two matrixes }}
// {{ class: linearalgebra }}
// {{ pattern: Matrix<\sNumber\s>\smatrix_multiply }}
// 行列同士の積
// O( N^3 )
template < typename Number >
Matrix< Number > matrix_multiply( const Matrix< Number > &a, const Matrix< Number > &b, unsigned int mod = 0 )
{
	assert( a[0].size() == b.size() );

	Matrix< Number > res( a.size(), Vector< Number >( b[0].size() ) );
	for ( int i = 0; i < static_cast<int>( a.size() ); ++i )
	{
		for ( int j = 0; j < static_cast<int>( b[0].size() ); ++j )
		{
			for ( int k = 0; k < static_cast<int>( b.size() ); ++k )
			{
				res[i][j] += a[i][k] * b[k][j];
				res[i][j] = modulo( res[i][j], mod );
			}
		}
	}
	return res;
}
template < typename Number >
inline const Matrix< Number > operator*( const Matrix< Number > &a, const Matrix< Number > &b )
{
	return matrix_multiply( a, b );
}

// END SNIPLATE

// 行列累乗
// O( N^3 log n )
template < typename Number >
Matrix< Number > power( Matrix< Number > a, long long n, long long mod = numeric_limits< long long >::max() )
{
	Matrix< Number > res = identify_matrix< Number >( a.size() );

	while ( n )
	{
		if ( n & 1 )
		{
			res = matrix_multiply( res, a, mod 	);
		}
		a = matrix_multiply( a, a, mod );
		n >>= 1;
	}

	return res;
}

// n 元一次連立方程式 Ax = b を解く
// Gauss Jordan の消去法 : O( n^3 )
// 戻り値はベクトル x
vector<double> gauss_jordan( const vector< vector<double> > &A, const vector<double> &b )
{
	const int n = A.size();
	const double EPS = 1e-8;

	vector< vector<double> > B( n, vector<double>( n + 1 ) );
	for ( int i = 0; i < n; i++ )
	{
		for ( int j = 0; j < n; j++ )
		{
			B[i][j] = A[i][j];
		}
	}
	for ( int i = 0; i < n; i++ )
	{
		B[i][n] = b[i];
	}

	for ( int i = 0; i < n; i++ )
	{
		{
			int p = i;
			for ( int j = i; j < n; j++ )
			{
				if ( abs( B[j][i] ) > abs( B[p][i] ) )
				{
					p = j;
				}
			}
			swap( B[i], B[p] );
		}

		if ( abs( B[i][i] ) < EPS )
		{
			return vector<double>();
		}

		for ( int j = i; j <= n; j++ )
		{
			B[i][j] /= B[i][i];
		}
		for ( int j = 0; j < n; j++ )
		{
			if ( i != j )
			{
				for ( int k = i + 1; k <= n; k++ )
				{
					B[j][k] -= B[j][i] * B[i][k];
				}
			}
		}
	}

	vector<double> res( n );
	for ( int i = 0; i < n; i++ )
	{
		res[i] = B[i][n];
	}
	return res;
}

// バイナリ行列の階数を求める
// O( N^3 )
int binary_matrix_rank( vector< vector<bool> > matrix )
{
	const int N = matrix.size(), M = matrix[0].size();

	int r = 0;
	for ( int x = 0; r < N && x < M; ++x )
	{
		int p = r;
		for ( int i = r + 1; i < N; i++ )
		{
			if ( matrix[i][x] )
			{
				p = i;
			}
		}
		swap( matrix[r], matrix[p] );
		if ( !matrix[r][x] )
		{
			continue;
		}

		for ( int i = r + 1; i < N; ++i )
		{
			if ( !matrix[i][x] )
			{
				continue;
			}

			for ( int j = x; j < M; ++j )
			{
				matrix[i][j] = matrix[i][j] ^ matrix[r][j];
			}
		}

		r++;
	}

	return r;
}

// 係数行列 A ，解ベクトル b の連立合同式を解く
// O( N^3 )
// include mathematics::mod_inverse
std::vector< int > mod_gauss_jordan( const std::vector< std::vector< int > > &A, const std::vector< int > &b, const int MOD = 1000000007 )
{
	const int N = A.size();

	// B は拡大係数行列
	std::vector< std::vector< int > > B( A );
	for ( int i = 0; i < N; ++i )
	{
		B[i].push_back( b[i] );
	}

	for ( int i = 0; i < N; ++i )
	{
		{
			int p = i;
			while ( p < N && !B[p][i] )
			{
				++p;
			}
			if ( p == N )
			{
				return std::vector< int >();
			}
			swap( B[i], B[p] );
		}

		{
			const int inv = mod_inverse( B[i][i], MOD );
			for ( int j = i; j <= N; ++j )
			{
				B[i][j] = LL( B[i][j] ) * inv % MOD;
			}
		}

		for ( int j = 0; j < N; ++j )
		{
			if ( i == j )
			{
				continue;
			}

			const int x = B[j][i];
			for ( int k = i; k <= N; ++k )
			{
				( B[j][k] -= LL( x ) * B[i][k] % MOD ) %= MOD;
				( B[j][k] += MOD ) %= MOD;
			}

		}
	}

	std::vector< int > result;
	transform( B.begin(), B.end(), std::back_inserter( result ), []( const std::vector< int > &v ){ return v.back(); } );
	return result;
}

//--------------------------

int main()
{


	return 0;
}
