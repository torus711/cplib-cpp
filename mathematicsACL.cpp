// #include "template.cpp"

// BEGIN SNIPLATE modfactACL 
// {{ abbr: calc factorization for each element in Z/pZ }}
// {{ class: math, numtheory }}
// {{ require: modint }}
// {{ pattern: class\smodFact }}
// 素数を法とする剰余環での n! を求める
// This class is published at https://github.com/torus711/cplib-cpp/mathematicsACL.cpp
class modFact
{
private:
	const int MAX_N;
	std::vector< MINT > fact;

public:
	modFact( const int n ) : MAX_N( n ), fact( min( MAX_N + 1, MINT::mod() ) )
	{
		fact[0] = 1;
		for ( int i = 1; i < (int)fact.size(); ++i )
		{
			fact[i] = fact[ i - 1 ] * i;
		}
		return;
	}

	MINT operator()( const int n )
	{
		int e;
		return operator()( n, e );
	}

	MINT operator()( const int n, int &e )
	{
		e = 0;
		if ( n == 0 )
		{
			return 1;
		}

		const MINT res = operator()( n / MINT::mod(), e );
		e += n / MINT::mod();

		if ( n / MINT::mod() % 2 != 0 )
		{
			return res * ( MINT::mod() - fact[ n % MINT::mod() ] );
		}
		return res * fact[ n % MINT::mod() ];
	}
};
// modfact( max_n )
// ()( n )
// ()( n, e )

// END SNIPLATE

// BEGIN SNIPLATE modcombACL 
// {{ abbr: calc nCr on Z/pZ }}
// {{ require: modfactACL }}
// {{ class: math, numtheory }}
// {{ pattern: class\smodConb }}
// nCr
// include : modFact
// This class is published at https://github.com/torus711/cplib-cpp/mathematicsACL.cpp
class modComb
{
private:
	modFact mod_fact;

public:
	modComb( const int n ) : mod_fact( n ) {};

	MINT operator()( const int n, const int r )
	{
		if ( n < 0 || r < 0 || n < r )
		{
			return 0;
		}

		int e1, e2, e3;
		MINT a1 = mod_fact( n, e1 ), a2 = mod_fact( r, e2 ), a3 = mod_fact( n - r, e3 );
		if ( e1 > e2 + e3 )
		{
			return 0;
		}
		return a1 * MINT( a2 * a3 ).inv();
	}
};
// modComb( max_n )
// ()( n, r )

modComb nCr( {{ cursor }} );

MINT nHr( const int n, const int r )
{
	return nCr( n + r - 1, r );
}

// END SNIPLATE
