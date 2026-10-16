#include <complex>
using Point =  complex< double >;
constexpr double EPS = 1e-8;
constexpr double PI = acos( -1 );
constexpr Point O( 0, 0 );

// 入力ストリームから実数二つをとって Point へ
istream& operator>> ( istream &s, Point &a )
{
	double r, i;
	s >> r >> i;
	a = Point( r, i );
	return s;
}

/**********************/
/* 浮動小数の基本演算 */
/**********************/

// a == b
bool eq( const double a, const double b )
{
	return abs( a - b ) <= EPS;
}


/*********+************/
/* ベクトルの基本演算 */
/**********************/

// 二点間の距離
double distance( const Point &a, const Point &b )
{
	return abs( a - b );
}

// 内積（ドット積）
double dot( const Point &a, const Point &b )
{
	return a.real() * b.real() + a.imag() * b.imag();
}

// 外積（クロス積）
double cross( const Point &a, const Point &b )
{
	return a.real() * b.imag() - a.imag() * b.real();
}

// 単位ベクトル
Point unitVector( const Point &a )
{
	return a / abs( a );
}

// 法線ベクトル
Point normalVector1( const Point &a )
{
	return a * Point( 0, 1 );
}
Point normalVector2( const Point &a )
{
	return a * Point( 0, -1 );
}
Point ( *normalVectors[] )( const Point& ) = { normalVector1, normalVector2 };

// 原点を中心に、点 p を theta [rad] 回転
Point rotate( const Point &p, const double theta )
{
	return Point(
		p.real() * cos( theta ) - p.imag() * sin( theta ),
		p.real() * sin( theta ) + p.imag() * cos( theta ) );
}

// 点 c を中心に、点 p を theta [rad] 回転
Point rotate( const Point &p, const Point &c, const double theta )
{
	const Point cp = p - c;
	const Point q = rotate( cp, theta );
	return c + q;
}


/**************/
/* ファンクタ */
/**************/

// x 座標優先
// include : eq
struct CompX : public binary_function<Point,Point,bool>
{
	bool operator ()( const Point &a, const Point &b ) const
	{
		return eq( a.real(), b.real() ) ? a.imag() < b.imag() : a.real() < b.real();
	}
};

// y 座標優先
// include : eq
struct CompY : public binary_function<Point,Point,bool>
{
	bool operator ()( const Point &a, const Point &b ) const
	{
		return eq( a.imag(), b.imag() ) ? a.real() < b.real() : a.imag() < b.imag();
	}
};

// 偏角
struct CompArg : public binary_function<Point,Point,bool>
{
	bool operator ()( const Point &a, const Point &b ) const
	{
		return arg( a ) < arg( b );
	}
};

// Point 二つを pair に
struct Zip : public binary_function< Point, Point, pair<Point,Point> >
{
	pair<Point,Point> operator ()( const Point &a, const Point &b ) const
	{
		return make_pair( a, b );
	}
};

// pair<Point,int> の比較
// include : CompX
struct CompPPI : public binary_function< pair<Point,int>, pair<Point,int>, bool >
{
	bool operator()( const pair<Point,int> &a, const pair<Point,int> &b ) const
	{
		if ( a.first == b.first )
		{
			return a.second < b.second;
		}
		return CompX()( a.first, b.first );
	}
};

/********************************************/
/* 線分と点・線分と線分・直線と直線の関係等 */
/********************************************/

// CCW 関数　a -> b -> c と進むとき、反時計回りなら 1, 直進なら 0, 時計回りなら -1
// include : dot, cross
// -1, 1 := 外積の符号
//  2 := c-a-b で一直線
// -2 := a-b-c で一直線
//  0 := a-c-b で一直線
int ccw( const Point &a, const Point &b, const Point &c )
{
	if ( cross( b - a, c - a ) < -EPS )
	{
		return -1;
	}
	if ( EPS < cross( b - a, c - a ) )
	{
		return 1;
	}
	if ( dot( b - a, c - a ) < -EPS )
	{
		return 2;
	}
	if ( dot( b - a, b - a ) < dot( c - a, c - a ) )
	{
		return -2;
	}	
	return 0;
}

// 点 p と直線 ( q1, q2 ) の距離
// include : cross
double distancePL( const Point &p, const Point &q1, const Point &q2 )
{
	return abs( cross( q2 - q1, p - q1 ) ) / abs( ( q2 - q1 ) );
}

// 点 p と線分 ( q1, q2 ) の距離
// include : dot, distancePL
double distancePS( const Point p, const Point &q1, const Point &q2 )
{
	if ( EPS < dot( q2 - q1, p - q1 ) && EPS < dot( q1 - q2, p - q2 ) ) // 垂線が交わる
	{
		return distancePL( p, q1, q2 );
	}
	else
	{
		return min( abs( q1 - p ), abs( q2 - p ) );
	}
}

// 点 p が線分 ( q1, q2 ) に乗っているか
// include : dot, cross
bool intersectPS( const Point &p, const Point &q1, const Point &q2 )
{
	// 端点からの p へのベクトルの外積が 0 ⇔ 端点からのベクトルが並行 ⇔ 直線 ( q1, q2 ) に点 p が乗る
	// p から端点へのベクトルの内積が 0 以下 ⇔ 点 p が q1, q2 の内側にある
	return abs( cross( q1 - p, q2 - p ) ) <= EPS && dot( q1 - p, q2 - p ) <= EPS;
}

// 直線 ( p1, p2 ) と直線 ( q1, q2 ) の交点
// include : cross
Point intersectionLL( const Point &p1, const Point &p2, const Point &q1, const Point &q2 )
{
	return p1 + ( p2 - p1 ) * ( cross( q2 - q1, q1 - p1 ) / cross( q2 - q1, p2 - p1 ) );
}

// 線分 ( p1, p2 ) と線分 ( q1, q2 ) が交差するか
// include : cross, intersectPS, intersectionLL
bool intersectSS( const Point &p1, const Point &p2, const Point &q1, const Point &q2 )
{		
	if ( abs( cross( p1 - p2, q1 - q2 ) ) <= EPS ) // 二直線が並行な場合
	{
		// 片方の線分に他方の端点が乗るか
		return intersectPS( q1, p1, p2 ) ||
			   intersectPS( q2, p1, p2 ) ||
			   intersectPS( p1, q1, q2 ) ||
			   intersectPS( p2, q1, q2 );
	}
	else
	{
		// 二直線の交点が両方の線分に乗るか
		Point r( intersectionLL( p1, p2, q1, q2 ) );
		return intersectPS( r, p1, p2 ) && intersectPS( r, q1, q2 );
	}
}

/**************/
/* 三角形とか */
/**************/

// 三角形 ( a, b, c ) の面積（ヘロンの公式）
double heron( const double a, const double b, const double c )
{
	const double s = ( a + b + c ) / 2;
	return sqrt( s * ( s - a ) * ( s - b ) * ( s - c ) );
}
double heron( const Point &a, const Point &b, const Point &c )
{
	return heron( abs( a - b ), abs( b - c ), abs( c - a ) );
}

// 三角形 ( a, b, c ) の中に点 p があるか
// include : cross, intersectPS
bool point_in_triangle( const Point &a, const Point &b, const Point &c, const Point &p )
{
	int res = 0;
	res += EPS < cross( b - a, p - a ) ? 1 : -1;
	res += EPS < cross( c - b, p - b ) ? 1 : -1;
	res += EPS < cross( a - c, p - c ) ? 1 : -1;

	if ( abs( res ) == 3 ) // 外積の符号が一致すると真
	{
		return true;
	}
	// いずれかの辺に乗るか
	return intersectPS( p, a, b ) || intersectPS( p, b, c ) || intersectPS( p, c, a );
}

// 三角形 ( a, b, c ) の最小包含円を求める
// include : dot
double triangle_enclosing( const Point &A, const Point &B, const Point &C )
{
	double a = abs( B - C ), b = abs( C - A ), c = abs( A - B );

	if ( dot( B - A, C - A ) <= EPS ||
		 dot( A - B, C - B ) <= EPS ||
		 dot( A - C, B - C ) <= EPS ) // 内積が 0 以下 ⇔ 直角または鈍角の角がある
	{
		// 鈍角三角形の場合、包含円は最長の辺を直径にもつ
		return max( a, max( b, c ) ) / 2;
	}
	else
	{
		// 余弦定理より cosA を求め、逆余弦関数を介して sinA を得る
		double cosA = ( pow( b, 2. ) + pow( c, 2. ) - pow( a, 2. ) ) / ( 2 * b * c );
		double sinA = sin( acos( cosA ) );

		// 正弦定理より、外接円の半径を得る
		return abs( a / sinA ) / 2;
	}
}


/******/
/* 円 */
/******/

// 二つの円 ( p1, r1 ) と ( p2, r2 ) の位置関係
// include : eq
// 接点を持たない : 0
// 外接する : 1
// 二点で交差する : 2
// 一致する : 3
// 1 が 2 を内部にもち、接点が無い : 10
// 1 が 2 を内部にもち、接する : 11
// 2 が 1 を内部にもち、接点がない : 20
// 2 が 1 を内部にもち、接する : 21
int intersectCC( const Point &p1, const double r1, const Point &p2, const double r2 )
{
	if ( eq( p1.real(), p2.real() ) && eq( p1.imag(), p2.imag() ) && eq( r1, r2 ) )
	{
		return 3;
	}

	const double dist = abs( p1 - p2 );
	if ( r1 + r2 + EPS < dist )
	{
		return 0;
	}
	else if ( eq( r1 + r2, dist ) )
	{
		return 1;
	}
	else if ( abs( r1 - r2 ) + EPS < dist && dist + EPS < r1 + r2 )
	{
		return 2;
	}
	else if ( dist + EPS <  abs( r1 - r2 ) )
	{
		return r1 < r2 ? 20 : 10;
	}
	else if ( eq( abs( r1 - r2 ), dist ) )
	{
		return r1 < r2 ? 21 : 11;
	}
	return -1;
}

// 線分 ( p1, p2 ) と円 ( q, r ) が交差するか
// include : distancePS
bool intersect_SC( const Point &p1, const Point &p2, const Point &q, const double r )
{
	if ( abs( p1 - q ) + EPS < r && abs( p2 - q ) + EPS < r ) // 線分は内部にある
	{
		return false;
	}
	else if ( r + EPS < abs( p1 - q ) && r + EPS < abs( p1 - q ) )
	{
		return distancePS( q, p1, p2 ) <= r + EPS;
	}
	else
	{
		return true;
	}
};

// 円 ( p, r ) と直線 ( q1, q2 ) の交点 :TODO
vector<Point> circle_line_intersection( const Point &p, const double r, const Point &q1, const Point q2 )
{
}

// 円と線分の交点 :TODO
vector<Point> circle_segment_intersection( const Point &p, const double r, const Point &q1, const Point &q2 )
{
}


// 円 ( p1, r1 ) と円 ( p2, r2 ) の交点
vector<Point> intersectionSS( const Point &p1, const double r1, const Point &p2, const double r2 )
{
	if ( abs( p1 - p2 ) <= EPS ) // 中心点が等しい ⇔ 交点が無いか、無限にある
	{
		return vector<Point>();
	}

	vector<Point> res;
	const double dist = abs( p1 - p2 );
	const double rc = ( dist * dist + r1 * r1 - r2 * r2 ) / ( 2 * dist );
	const double rs = sqrt( r1 * r1 - rc * rc );
	const Point diff = ( p2 - p1 ) / Point( dist, 0 );

	res.push_back( Point( p1 + diff * Point( rc, rs ) ) );
	res.push_back( Point( p1 + diff * Point( rc, -rs ) ) );
	return res;
}

// 点 p を通る直線と円 ( q, r ) の交点
// include : dot
vector<Point> tangentPC( const Point &p, const Point &q, const double r )
{
	const double x = dot( p - q, p - q );
	double d = x - r * r;

	if ( d < -EPS )
	{
		return vector<Point>();
	}

	d = max( d, 0. );
	const Point s1 = ( p - q ) * ( r * r / x );
	const Point s2 = ( ( p - q ) * ( -r * sqrt( d ) / x ) ) * Point( 0, 1 );

	vector<Point> res;
	res.push_back( q + s1 - s2 );
	res.push_back( q + s1 + s2 );
	return res;
}

// 二円 ( p1, r1 ), ( p2, r2 ) の共通接線
vector< pair<Point,Point> > tangentCC( const Point &p1, const double r1, const Point &p2, const double r2 )
{
	vector< pair<Point,Point> > res;

	// 外接線
	if ( abs( r1 - r2 ) < EPS )
	{
		Point dir = p2 - p1;
		dir = ( dir * ( r1 / abs( dir ) ) ) * Point( 0, 1 );
		res.push_back( make_pair( p1 + dir, p2 + dir ) );
		res.push_back( make_pair( p1 - dir, p2 - dir ) );
	}
	else
	{
		Point p = ( p1 * ( -r2 ) ) + ( p2 * r1 );
		p = p * ( 1. / ( r1 - r2 ) );
		vector<Point> ps = tangentPC( p, p1, r1 );
		vector<Point> qs = tangentPC( p, p2, r2 );
		for ( int i = 0; i < min( ps.size(), qs.size() ); ++i )
		{
			res.push_back( make_pair( ps[i], qs[i] ) );
		}
	}

	// 内接円
	Point p = ( p1 * r2 ) + ( p2 * r1 );
	p = p * ( 1. / ( r1 + r2 ) );
	vector<Point> ps = tangentPC( p, p1, r1 );
	vector<Point> qs = tangentPC( p, p2, r2 );
	for ( int i = 0; i < min( ps.size(), qs.size() ); ++i )
	{
		res.push_back( make_pair( ps[i], qs[i] ) );
	}
	
	return res;
}

/**************/
/* 多角形とか */
/**************/

// 四角形 ( a, b, c, d ) の中に点 p があるか
// 対角線は ( a, c ) と ( b, d )
// include : point_in_triangle
bool point_in_square( const Point &a, const Point &b, const Point &c, const Point &d, const Point &p )
{
	return point_in_triangle( a, b, c, p ) || point_in_triangle( a, c, d, p );
}

// 多角形 ps の面積
// verified : AOJ 0079
double polygon_area( vector<Point> ps )
{
	const int N = ps.size();
	ps.push_back( ps[0] );

	double res = 0;
	for ( int i = 0; i < N; ++i )
	{
		res += ps[i].real() * ps[ i + 1 ].imag() - ps[ i + 1 ].real() * ps[i].imag();
	}
	return abs( res / 2 );
}

// 点 p が多角形 qs の内部にあるか 
// Winding Number Algorithm
// 自己交差を含む場合でも内部と判定される
// include : ccw
bool point_in_polygon( const Point &p, const vector<Point> &qs )
{
	const int N = qs.size();
	
	int wn = 0;
	for ( int i = 0; i < N; ++i )
	{
		const Point &s = qs[i], &t = qs[ ( i + 1 ) % N ];
		if ( s.imag() - EPS <= t.imag() && s.imag() - EPS < p.imag() && p.imag() + EPS < t.imag() ) // 上向きの辺
		{
			wn += ccw( s, p, t ) == -1;
		}
		else if ( t.imag() - EPS <= s.imag() && t.imag() - EPS < p.imag() && p.imag() + EPS < s.imag() )
		{
			wn -= ccw( t, p, s ) == -1;
		}
	}
	return wn;
};

// 点集合 ps の凸包（ Graham Scan ）
// include : cross, CompX
vector<Point> convex_hull( vector<Point> ps )
{
	const int N = ps.size();

	sort( ps.begin(), ps.end(), CompX() );

	int k = 0;
	vector<Point> convex( N * 2 );

	for ( int i = 0; i < N; i++ )
	{
		while ( 2 <= k && cross( convex[ k - 1 ] - convex[ k - 2 ], ps[i] - convex[ k - 1 ] ) <= EPS )
		{
			k--;
		}
		convex[ k++ ] = ps[i];
	}

	for ( int i = N - 2, t = k; 0 <= i; i-- )
	{
		while ( t < k && cross( convex[ k - 1 ] - convex[ k - 2 ], ps[i] - convex[ k - 1 ] ) <= EPS )
		{
			k--;
		}
		convex[ k++ ] = ps[i];
	}

	convex.resize( k - 1 );
	return convex;
}

// 双対グラフ
// includes : CompArg, dot, cross
class DualGraph
{
	using Edge = std::tuple< int, int, int, Point >;
	// ( to, rev-edge, edge id, angle )

	const int N;
	int M;
	const std::vector< Point > ps_;
	std::vector< std::vector< Edge > > G;
	std::vector< std::vector< int > > edge_indices_, edge_poss_;
	std::vector< std::vector< bool > > used_;

	int outer_ = -1;
	std::vector< std::vector< Point > > areas_;

	std::vector< std::vector< int > > intersects_;

public:
	DualGraph( const std::vector< Point > &ps ) :
		N( ps.size() ), M( 0 ), ps_( ps ), G( N ), edge_indices_( N ), edge_poss_( N ), used_( N )
	{
		return;
	}

	void connect( const int a, const int b )
	{
		G[a].emplace_back( b, G[b].size(), M, ps_[b] - ps_[a] );
		G[b].emplace_back( a, G[a].size() - 1, M, ps_[a] - ps_[b] );
		++M;

		return;
	}

	void solve()
	{
		sort_edges();

		for ( int v = 0; v < N; ++v )
		{
			for ( int i = 0; i < int( G[v].size() ); ++i )
			{
				if ( used_[v][i] )
				{
					continue;
				}

				dfs( v, i );
			}
		}

		return;
	}

private:
	void sort_edges()
	{
		CompArg comp;
		for ( int v = 0; v < N; ++v )
		{
			edge_indices_[v].resize( G[v].size() );
			std::iota( begin( edge_indices_[v] ), end( edge_indices_[v] ), 0 );

			std::sort( begin( edge_indices_[v] ), end( edge_indices_[v] ), 
					[&]( const int i, const int j ){ return comp( get< 3 >( G[v][i] ), get< 3 >( G[v][j] ) ); } );

			edge_poss_[v].resize( G[v].size() );
			for ( int i = 0; i < int( G[v].size() ); ++i )
			{
				edge_poss_[v][ edge_indices_[v][i] ] = i;
			}

			used_[v].resize( G[v].size() );
		}
		intersects_.resize( M );

		return;
	}

	void dfs( const int sv, const int se )
	{
		std::vector< Point > area;

		for ( int v = sv, e = se; area.empty() || v != sv; )
		{
			used_[v][e] = true;
			area.push_back( ps_[v] );
			intersects_[ get< 2 >( G[v][e] ) ].push_back( areas_.size() );

			const int r = get< 1 >( G[v][e] );
			v = get< 0 >( G[v][e] );
			e = edge_indices_[v][ ( edge_poss_[v][r] + G[v].size() - 1 ) % G[v].size() ];
		}

		{
			double theta = 0;
			for ( int i = 0; i < int( area.size() ); ++i )
			{
				const Point p1 = area[ ( i + 1 ) % area.size() ] - area[i];
				const Point p2 = area[ ( i + 2 ) % area.size() ] - area[i];
				theta += atan2( cross( p1, p2 ), dot( p1, p2 ) );
			}
			if ( theta < 0 )
			{
				outer_ = areas_.size();
			}
		}

		areas_.push_back( area );
		
		return;
	}

	// results
public:
	int area_number() const
	{
		return areas_.size();
	}

	std::vector< std::vector< Point > > areas() const
	{
		return areas_;
	}

	std::vector< std::pair< int, int > > edges() const
	{
		std::vector< std::pair< int, int > > res;
		for ( auto &row : intersects_ )
		{
			res.emplace_back( row[0], row[1] );
		}
		return res;
	}

	int outer() const
	{
		return outer_;
	}
};
// DualGraph( vector< Point > > )
// connect( u, v )
// solve()
// area_number()
// areas()
// edges()
// outer()

namespace delaunay
{
	struct Circle
	{
		const Point c;
		const double r;

		Circle( const Point &c_, const double r_ ) : c( c_ ), r( r_ )
		{
			return;
		}
	};

	struct Triangle
	{
		VPII ps;

		Triangle( const PII &a, const PII &b, const PII &c )
		{
			ps.PB( a );
			ps.PB( b );
			ps.PB( c );
			sort( ALL( ps ) );

			return;
		}

		bool has_common_point( const Triangle &rhs )
		{
			for ( int i = 0; i < 3; ++i )
			{
				for ( int j = 0; j < 3; ++j )
				{
					if ( ps[i] == rhs.ps[j] )
					{
						return true;
					}
				}
			}
			return false;
		}

		Circle circum_circle() const
		{
			const double x1 = ps[0].fst;
			const double y1 = ps[0].snd;
			const double x2 = ps[1].fst;
			const double y2 = ps[1].snd;
			const double x3 = ps[2].fst;
			const double y3 = ps[2].snd;

			const double c = 2 * ( ( x2 - x1 ) * ( y3 - y1 ) - ( y2 - y1 ) * ( x3 - x1 ) );
			const double x = ( ( y3 - y1 ) * ( x2 * x2 - x1 * x1 + y2 * y2 - y1 * y1 ) + ( y1 - y2 ) * ( x3 * x3 - x1 * x1 + y3 * y3 - y1 * y1 ) ) / c;  
			const double y = ( ( x1 - x3 ) * ( x2 * x2 - x1 * x1 + y2 * y2 - y1 * y1 ) + ( x2 - x1 ) * ( x3 * x3 - x1 * x1 + y3 * y3 - y1 * y1 ) ) / c;  

			const Point p( x, y );
			return Circle( p, abs( p - Point( x1, y1 ) ) );
		}

		double area() const
		{
			return heron( Point( ps[0].fst, ps[0].snd ), Point( ps[1].fst, ps[1].snd ), Point( ps[2].fst, ps[2].snd ) );
		}

		bool operator<( const Triangle &rhs ) const
		{
			return ps < rhs.ps;
		}

		bool operator==( const Triangle &rhs ) const
		{
			return ps == rhs.ps;
		}
	};


	class DelaunayTriangulation
	{
		private:
			VPII ps;

			VT< Triangle > triangles;

		public:
			void add_point( const PII &p )
			{
				ps.PB( p );
				return;
			};

			VT< Triangle > solve()
			{
				const Triangle outer( PII( -1, -1 ), PII( -1, 5000 ), PII( 5000, -1 ) );
				triangles.PB( outer );

				random_shuffle( ALL( ps ) );
				for_each( ALL( ps ), [&]( const PII &p ){ eval_point( p ); } );

				VT< Triangle > res;
				FOR( tri, triangles )
				{
					if ( tri.has_common_point( outer ) )
					{
						continue;
					}
					res.PB( tri );
				}
				return res;
			}

			void eval_point( const PII &p )
			{
				DUMP( SZ( triangles ) );
				Point pp( p.fst, p.snd );
				map< Triangle, int > tri_counts;
				VT< Triangle > next_tris;

				FOR( tri, triangles )
				{
					const Circle cir = tri.circum_circle();

					if ( abs( pp - cir.c ) <= cir.r + EPS )
					{
						++tri_counts[ Triangle( p, tri.ps[0], tri.ps[1] ) ];
						++tri_counts[ Triangle( p, tri.ps[0], tri.ps[2] ) ];
						++tri_counts[ Triangle( p, tri.ps[1], tri.ps[2] ) ];
					}
					else
					{
						next_tris.PB( tri );
					}
				}

				FOR( tcnt, tri_counts )
				{
					if ( 2 <= tcnt.snd )
					{
						continue;
					}
					next_tris.PB( tcnt.fst );
				}

				triangles = next_tris;

				return;
			}
	};
}	
