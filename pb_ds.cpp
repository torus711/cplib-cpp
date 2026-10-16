// BEGIN SNIPLATE orderedsetPBDS 
// {{ abbr: define pb_ds's ordered set }}
// {{ class: pb_ds }}
// {{ pattern: using ordered_set = __gnu_pbds::tree< }}
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

template < typename T >
using ordered_set = __gnu_pbds::tree<
	T,
	__gnu_pbds::null_type,
	std::less<T>,
	__gnu_pbds::rb_tree_tag,
	__gnu_pbds::tree_order_statistics_node_update
>;

// ordered_set::find_by_order
// ordered_set::order_of_key

// END SNIPLATE
