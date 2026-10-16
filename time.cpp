#include "./../_forLib.cpp"

#include <chrono>

class StopWatch
{
private:
	chrono::system_clock::time_point start_;

public:
	StopWatch() : start_( chrono::system_clock::now() ) {}
	virtual ~StopWatch() = default;

	void reset()
	{
		start_ = chrono::system_clock::now();
		return;
	}

	chrono::system_clock::duration elapsedTime() const
	{
		return chrono::system_clock::now() - start_;
	}
	
	long long elapsedTimeMs() const
	{
		return chrono::duration_cast< chrono::milliseconds >( elapsedTime() ).count();
	}
};

class TimeKeeper
{
private:
	const long long measurement_;
	StopWatch stopwatch_;

public:
	TimeKeeper( long long m ) : measurement_( m ), stopwatch_() {};

	operator bool() const
	{
		return stopwatch_.elapsedTimeMs() <= measurement_;
	}
};

class TimeTotalizator
{
private:
	chrono::system_clock::duration elapsed_ = chrono::system_clock::duration();

	class TTSubunit
	{
	private:
		TimeTotalizator &parent;
		chrono::system_clock::time_point start_;
	
	public:
		TTSubunit(  TimeTotalizator &p ) : parent( p ), start_( chrono::system_clock::now() ) {};
		~TTSubunit()
		{
			parent.elapsed_ += chrono::system_clock::now() - start_;
		}
	};
	
public:
	TTSubunit getSubunit()
	{
		return move( TTSubunit( *this ) );
	}

	long long elapsedTimeMs() const
	{
		return chrono::duration_cast< chrono::milliseconds >( elapsed_ ).count();
	}

	void printElapsedMs( ostream &s = cout ) const
	{
		s << elapsedTimeMs() << endl;
	}
};

int main()
{
	

	return 0;
}
