#include "./../_forLib.cpp"

/******************/
/* ユーティリティ */
/******************/

// BEGIN SNIPLATE log2floor 
// {{ abbr: flooring log_2( x )
// {{ class: math }}
// {{ pattern: int\slog2floor }}
int log2floor( const LL n )
{
	return 63 - __builtin_clzll( n );
}

// END SNIPLATE

// BEGIN SNIPLATE log2ceil 
// {{ abbr: ceiling log_2( x )
// {{ class: math }}
// {{ pattern: int\slog2ceil }}
int log2ceil( const LL n )
{
	return 63 - __builtin_clzll( n ) + ( 1 < __builtin_popcountll( n ) );
}

// END SNIPLATE

// BEGIN SNIPLATE sign 
// {{ abbr: return the sign of integer
// {{ class: math }}
// {{ pattern: constexpr\sint\ssign }}
constexpr int sign( const int a )
{
	return ( 0 < a ) - ( a < 0 );
}

// END SNIPLATE

/********/
/* 数論 */
/********/

// BEGIN SNIPLATE gcd 
// {{ abbr: Greatest Common Divisor by Euclidean Algorithm }}
// {{ class: math, numtheory }}
// {{ pattern: long\slong\sgcd }}
long long gcd( const long long a, const long long b )
{
	return b == 0 ? a : gcd( b, a % b );
}

// END SNIPLATE

// BEGIN SNIPLATE lcm 
// {{ abbr: Least Common Multiply }}
// {{ require: gcd }}
// {{ class: math, numtheory }}
// {{ pattern: long\slong\sgcd }}
// Least Common Multiple
long long lcm( const long long a, const long long b )
{
	return a / gcd( a, b ) * b;
}

// END SNIPLATE

// BEGIN SNIPLATE extgcd 
// {{ abbr: Extended Euclidean Algorithm }}
// {{ class: math, numtheory }}
// {{ pattern: int\sextgcd }}
// ax + by = gcd( a, b ) となる x, y を求める
// 拡張 Euclid の互除法
LL extgcd( LL a, LL b, LL &x, LL &y )
{
	LL d = a;
	if ( b != 0 )
	{
		d = extgcd( b, a % b, y, x );
		y -= ( a / b ) * x;
	}
	else
	{
		x = 1;
		y = 0;
	}
	return d;
}

// END SNIPLATE

// BEGIN SNIPLATE factorization 
// {{ abbr: Prime Factorization }}
// {{ class: math, numtheory }}
// {{ pattern: vector< long long > primeFactorization( long long N ) }}
// 素因数分解 O( √N )
// Thi function is published at https://github.com/torus711/cplib-cpp/mathematics.cpp
vector< long long > primeFactorization( long long N )
{
	if ( N == 1 )
	{
		return vector< long long >();
	}

	vector< long long > result;
	for ( long long p = 2; p * p <= N; p++ )
	{
		while ( !( N % p ) )
		{
			result.push_back( p );
			N /= p;
		}
	}
	if ( N != 1 )
	{
		result.push_back( N );
	}

	return result;
}

// END SNIPLATE

// BEGIN SNIPLATE eratosthenes 
// {{ abbr: Sieve of Eratosthenes }}
// {{ class: math, numtheory }}
// {{ pattern: vector<bool> eratosthenes( const int N ) }}
// 素数かどうかの表を計算
// Eratosthenes の篩 : O( N log log N )
// This function is published at https://github.com/torus711/cplib-cpp/mathematics.cpp
vector<bool> eratosthenes( const int N )
{
	vector<bool> isPrime( N + 1, true );
	isPrime[0] = isPrime[1] = false;

	for ( int i = 0; i <= N; i++ )
	{
		if ( !isPrime[i] )
		{
			continue;
		}

		for ( int j = 2; i * j <= N; j++ )
		{
			isPrime[ i * j ] = false;
		}
	}

	return isPrime;
}

// END SNIPLATE

// BEGIN SNIPLATE getprimes 
// {{ abbr: generate list of primes less or equal to N }}
// {{ require: eratosthenes }}
// {{ class: math, numtheory }}
// {{ pattern: vector<int> getPrimes( const int N ) }}
// N 以下の素数のリストを生成する
// include : eratosthenes
// This function is published at https://github.com/torus711/cplib-cpp/mathematics.cpp
vector<int> getPrimes( const int N )
{
	vector<bool> isPrime = eratosthenes( N );
	vector<int> result;
	result.reserve( count( isPrime.begin(), isPrime.end(), true ) );
	for ( int i = 0; i <= N; i++ )
	{
		if ( isPrime[i] )
		{
			result.push_back( i );
		}
	}
	return result;
}

// END SNIPLATE

// BEGIN SNIPLATE isprime 
// {{ abbr: evaluate whether n is prime }}
// {{ class: math, numtheory }}
// {{ pattern: bool\sisPrime }}
// 素数判定 O(√N)
// This function is published at https://github.com/torus711/cplib-cpp/mathematics.cpp
bool isPrime( const long long N )
{
	if ( N < 2 )
	{
		return false;
	}

	for ( long long i = 2; i * i <= N; ++i )
	{
		if ( !( N % i ) )
		{
			return false;
		}
	}

	return true;
}

// END SNIPLATE

// BEGIN SNIPLATE modpow 
// {{ abbr: calc a^x in O( logx ) time }}
// {{ class: math, numtheory }}
// {{ pattern: long long mod_pow }}
// a^x を mod で求める
// 反復二乗法
// O( log x )
// This function is published at https://github.com/torus711/cplib-cpp/mathematics.cpp
long long mod_pow( long long a, long long x, long long mod )
{
	a %= mod;

	long long res = 1;
	for ( ; x; x >>= 1, ( a *= a ) %= mod )
	{
		if ( x & 1 )
		{
			( res *= a ) %= mod;
		}
	}
	return res;
}

// END SNIPLATE

// BEGIN SNIPLATE modinv 
// {{ abbr: calc a^{-1} mod p ( p is prime ) }}
// {{ require: modpow }}
// {{ class: math, numtheory }}
// {{ pattern: int\smod_inverse }}
// p が素数のとき、p を法とする剰余環での逆元を求める
// Fermat の小定理を利用
// a^{ p - 1 } \equiv 1 ( mod p )
// a^{ p - 2 } \equiv a^{-1} ( mod p )
// incluide : mod_pow
int mod_inverse( long long a, long long p )
{
	return mod_pow( a, p - 2, p );
}

// END SNIPLATE

// BEGIN SNIPLATE modfact 
// {{ abbr: calc factorization for each element in Z/pZ }}
// {{ class: math, numtheory }}
// {{ pattern: class\smodFact }}
// 素数を法とする剰余環での n! を求める
// This class is published at https://github.com/torus711/cplib-cpp/mathematics.cpp
class modFact
{
private:
	const int MAX_N, MOD;
	vector<int> fact;

public:
	modFact( const int n, const int mod ) : MAX_N( n ), MOD( mod ), fact( min( MAX_N + 1, MOD ) )
	{
		fact[0] = 1;
		for ( int i = 1; i < (int)fact.size(); ++i )
		{
			fact[i] = 1LL * fact[ i - 1 ] * i % MOD;
		}
		return;
	}

	int operator()( const int n )
	{
		int e;
		return operator()( n, e );
	}

	int operator()( const int n, int &e )
	{
		e = 0;
		if ( n == 0 )
		{
			return 1;
		}

		const long long res = operator()( n / MOD, e );
		e += n / MOD;

		if ( n / MOD % 2 != 0 )
		{
			return res * ( MOD - fact[ n % MOD ] ) % MOD;
		}
		return res * fact[ n % MOD ] % MOD;
	}
};
// modfact( max_n, mod )
// ()( n )
// ()( n, e )

// END SNIPLATE

// BEGIN SNIPLATE modcomb 
// {{ abbr: calc nCr on Z/pZ }}
// {{ require: modinv, modfact }}
// {{ class: math, numtheory }}
// {{ pattern: class\smodConb }}
// nCr
// include : modFact, mod_inverse
// This class is published at https://github.com/torus711/cplib-cpp/mathematics.cpp
class modComb
{
private:
	const int MOD;
	modFact mod_fact;

public:
	modComb( const int n, const int mod ) : MOD( mod ), mod_fact( n, mod ) {};

	int operator()( const int n, const int r )
	{
		if ( n < 0 || r < 0 || n < r )
		{
			return 0;
		}

		int e1, e2, e3;
		long long a1 = mod_fact( n, e1 ), a2 = mod_fact( r, e2 ), a3 = mod_fact( n - r, e3 );
		if ( e1 > e2 + e3 )
		{
			return 0;
		}
		return a1 * mod_inverse( a2 * a3 % MOD, MOD ) % MOD;
	}
};
// modComb( n, mod )
// ()( n, r )

modComb nCr( {{ cursor }}, MOD );

// END SNIPLATE

// BEGIN SNIPLATE pascaltri 
// {{ abbr: calc nCr on Z/pZ }}
// {{ class: math, numtheory }}
// {{ pattern: class\sPascalTriangle }}
// パスカルの三角形による nCr on Z/mZ の計算
// 前処理 \Theta( N^2 ), クエリ O( 1 )
// This class is published at https://github.com/torus711/cplib-cpp/mathematics.cpp
class PascalTriangle
{
private:
	const int N, MOD;
	vector< vector<int> > dp;

public:
	PascalTriangle( const int n, const int mod ) : N( n ), MOD( mod )
	{
		dp.resize( N + 1 );
		for ( int i = 0; i <= N; ++i )
		{
			dp[i].resize( i + 1 );
		}

		dp[0][0] = 1;
		for ( int i = 0; i < N; ++i )
		{
			for ( int j = 0; j <= i; ++j )
			{
				( dp[ i + 1 ][j] += dp[i][j] ) %= MOD;
				( dp[ i + 1 ][ j + 1 ] += dp[i][j] ) %= MOD;
			}
		}

		return;
	}

	int operator()( const int n, const int r ) const
	{
		return n < 0 || r < 0 || n < r ? 0 : dp[n][r];
	}
};
// PascalTriangle( N )
// ()( n, r )

PascalTriangle nCr( {{ cursor }}, MOD );

// END SNIPLATE

// BEGIN SNIPLATE totient 
// {{ abbr: Euler's Totient function }}
// {{ class: math, numtheory }}
// {{ pattern: long\slong\seuler_totient }}
// euler の φ関数 (euler totient)
// n 以下の、n と互いに素な整数の数を求める
// x^φ(m) ≡ 1 ( mod m )
long long euler_totient( long long n )
{
	long long res = n;
	for ( long long i = 2; i * i <= n; i++ )
	{
		if ( !( n % i ) )
		{
			res = res / i * ( i - 1 );
			for ( ; !( n % i ); n /= i );
		}
	}
	if ( 1 < n )
	{
		res = res / n * ( n - 1 );
	}

	return res;
}

// END SNIPLATE

// BEGIN SNIPLATE mobius 
// {{ abbr: Mobius function }}
// {{ class: math, numtheory }}
// {{ pattern: map<int,int>\smobius }}
// メビウス関数
// n が 1 でない平方数で割り切れるとき、0
// そうでないとき、n の約数の個数を k として ( -1 ) ^ k
map<int,int> mobius( int n )
{
	map<int,int> res;
	vector<int> primes;

	for ( int i = 2; i * i <= n; i++ )
	{
		if ( !( n % i ) )
		{
			primes.push_back( i );
			while ( !( n % i ) ) n /= i;
		}
	}
	if ( n != 1 ) primes.push_back( n );

	const int m = primes.size();
	for ( int s = 0; s < ( 1 << m ); s++ )
	{
		int mu = 1, d = 1;
		for ( int i = 0; i < m; i++ )
		{
			if ( s & 1 << i )
			{
				mu *= -1;
				d *= primes[i];
			}
		}
		res[d] = mu;
	}

	return res;
}

// END SNIPLATE

// BEGIN SNIPLATE stirling2nc 
// {{ abbr: Stirling number of the 2nd kind }}
// {{ class: math, numtheory }}
// {{ require; modcomb, modpow, modinv }}
// {{ pattern: int\sstirling2nd }}
int stirling2nd( const int N, const int K, const int MOD )
{
	long long res = 0;
	for ( int j = 0; j <= K; ++j )
	{
		( res += long long( j % 2 ? MOD - 1 : 1 ) * nCr( M, j ) % MOD * mod_pow( M - j, N, MOD ) % MOD ) %= MOD;
	}
	long long factk = 1;
	for ( int i = 1; i <= K; ++k )
	{
		( factk *= i ) %= MOD;
	}
	return res * mod_inverse( factk, MOD );
}

// END SNIPLATE

// BEGIN SNIPLATE quotientring
// {{ abbr: QuotientRing }}
// {{ require: modinv }}
// {{ class: DataStructure }}
// {{ pattern: class\QuotientRing$ }}
template < int M >
class QuotientRing
{
	long long value_;

public:
	QuotientRing( const int v = 0 ) : value_( ( ( v % M ) + M ) % M )

	QuotientRing& operator+=( const QuotientRing &rhs )
	{
		value_ += rhs.value_;
		if ( value_ > M )
		{
			value_ %= M;
		}
		return *this;
	}

	QuotientRing& operator-=( const QuotientRing &rhs )
	{
		value_ -= rhs.value_;
		if ( 0 < value_ )
		{
			value_ = ( value_ % M + M ) % M;
		}
		return *this;
	}

	QuotientRing& operator*=( const QuotientRing &rhs )
	{
		( value_ *= rhs.value_ ) %= M;
		return *this;
	}

	QuotientRing& operator/=( const QuotientRing $rhs )
	{
		( value_ *= mod_inverse( rhs.value, M ) ) %= M;
		return *this;
	}
}

QuotientRing operator+( const QuotientRing &lhs, const QuotientRing &rhs )
{
	return QuotientRing( lhs.value_ + rhs.value_ );
}

ostream& operator<<( const QuotientRing rhs )
{
	out << rhs.value_;
	return out;
}

// END SNIPLATE
