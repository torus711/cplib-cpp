// BEGIN SNIPLATE include_segtree 
// {{ abbr: includes <atcoder/segtree> }}
// {{ class: ACL }}
// {{ pattern: #include <atcoder/segtree> }}
#include <atcoder/segtree>
using namespace atcoder;

// END SNIPLATE

// BEGIN SNIPLATE segtreeACL 
// {{ abbr: template for ACL's segtree }}
// {{ class: ACL }}
// {{ require: include_segtree }}
namespace SegTree
{
	using S = int;

	S op( const S a, const S b )
	{
		return a + b;
	}

	S e()
	{
		return 0;
	}

	using segtree = segtree< S, op, e >;
}

// END SNIPLATE

// BEGIN SNIPLATE LazySegtreeACL 
// {{ abbr: my template }}
// {{ class: util }}
// {{ pattern: #include <atcoder/lazysegtree> }}
#include <atcoder/lazysegtree>
using namespace atcoder;

namespace LazySegTree
{
	using S = int;

	// F: S -> S
	struct F
	{
		S v;
	};

	// op: S \times S -> S
	S op( const S a, const S b )
	{
		return a + b;
	}

	S e()
	{
		return 0;
	}

	// return f( a )
	S mapping( const F f, const S a )
	{
		return a + f.v;
	}

	// f \circ g
	F composition( const F f, const F g )
	{
		return F{ f.v + g.v };
	}

	F id()
	{
		return F{ 0 };
	}

	using SegTree = lazy_segtree< S, op, e, F, mapping, composition, id >;
}

// END SNIPLATE
