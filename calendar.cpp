#include "./../_.cpp"

bool isLeap( int y )
{
	if ( !( y % 400 ) )
	{
		return true;
	}
	if ( !( y % 100 ) )
	{
		return false;
	}
	return !( y % 4 );
}

constexpr int DAYS[][13] = {
	0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
	0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
};

const ull secOfDay = 60 * 60 * 24;

struct Date
{
	int Y, M, D, h, m;
	ull s;

	Date( int Y, int M, int D, int h, int m, int s ) : Y( Y ), M( M ), D( D ), h( h ), m( m ), s( s ) 
	{
		return;
	}

	Date( ull sec ) : Y( 0 ), M( 0 ), D( 0 ), h( 0 ), m( 0 )
	{
		addDay( sec / secOfDay );
		addSec( sec % secOfDay );

		return;
	}

	void addYear( int year )
	{
		Y += year;

		return;
	}

	void addMonth( int month )
	{
		M += month;

		if ( 12 <= M - 1 )
		{
			addYear( ( M - 1 ) / 12 );
			M = ( M - 1 ) % 12 + 1;
		}

		return;
	}

	void addDay( int day )
	{
		D += day;

		while ( 366 <= D )
		{
			if ( M <= 2 && isLeap( Y ) || 3 <= M && isLeap( Y + 1 ) )
			{
				D -= 366;
			}
			else
			{
				D -= 365;
			}

			addYear( 1 );
		}

		while ( days[M-1] < D )
		{
			if ( M == 2 && isLeap( Y ) )
			{
				if ( 29 < D )
				{
					D -= 29;
					addMonth( 1 );
				}
			}
			else
			{
				D -= days[M-1];
				addMonth( 1 );
			}
		}

		return;
	}

	void addHour( int hour )
	{
		h += hour;

		if ( 24 <= h )
		{
			addDay( h / 24 );
			h %= 24;
		}

		return;
	}

	void addMin( int min )
	{
		m += min;

		if ( 60 <= m )
		{
			addHour( m / 60 );
			m %= 60;
		}

		return;
	}

	void addSec( ull sec )
	{
		s += sec;

		if ( 60 <= s )
		{
			addMin( s / 60 );
			s %= 60;
		}

		return;
	}

	bool operator < ( const Date &target ) const
	{
		if ( Y < target.Y )
		{
			return true;
		}
		else if ( Y > target.Y )
		{
			return false;
		}

		if ( M < target.M )
		{
			return true;
		}
		else if ( M > target.M )
		{
			return false;
		}

		if ( D < target.D )
		{
			return true;
		}
		else if ( D > target.D )
		{
			return false;
		}

		if ( h < target.h )
		{
			return true;
		}
		else if ( h > target.h )
		{
			return false;
		}

		if ( m < target.m )
		{
			return true;
		}
		else if ( m > target.m )
		{
			return false;
		}

		if ( s < target.s )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

bool sameDate( const Date &a, const Date &b )
{
	return a.Y == b.Y && a.M == b.M && a.D == b.D;
}

bool sameTime( const Date &a, const Date &b )
{
	return a.h == b.h && a.m == b.m && a.s == b.s;
}

int diffDay( Date a, Date b )
{
	if ( b < a )
	{
		swap( a, b );
	}

	int diff = 0;

	while ( !sameDate( a, b ) )
	{
		b.addDay( 1 );
		diff++;
	}

	return diff;
}

ull diffSec( Date a, Date b )
{
	int sec = 0, day = diffDay( a, b );

	while ( !sameTime( a, b ) )
	{
		b.addSec( 1 );
		sec++;
	}

	if ( 60 * 60 * 24 < sec )
	{
		sec = secOfDay - sec;
	}

	return day * secOfDay + sec;
}
