// C++ program without declaring the
// move constructor
#include <iostream>
#include <vector>
using namespace std;

// Move Class
class Move {
private:
	// Declaring the raw pointer as
	// the data member of the class
	int* data;

public:

    Move()
	 : Move{0}
	 {

	 }
	// Constructor
	Move(int d)
	{
		// Declare object in the heap
		data = new int;
		*data = d;

		cout << "Constructor is called for "
			<< d << endl;
	};

	// Copy Constructor to delegated
	// Copy constructor
	Move(const Move& source)
		: Move{ *source.data }
	{

		// Copying constructor copying
		// the data by making deep copy
		cout << "Copy Constructor is called - "
			<< "Deep copy for "
			<< *source.data
			<< endl;
	}

	 // Move Constructor
    Move(Move&& source)
        : data{ source.data }
    {
 
        cout << "Move Constructor for "
             << *source.data << endl;
        source.data = nullptr;
    }

	Move& operator=(const Move& source)
	{
           cout << "Move operator= for "
             << *source.data << endl;
           *data = *source.data;
		return *this;
	}

	// Destructor
	~Move()
	{
		if (data != nullptr)

			// If the pointer is not
			// pointing to nullptr
			cout << "Destructor is called for "
				<< *data << endl;
		else

			// If the pointer is
			// pointing to nullptr
			cout << "Destructor is called"
				<< " for nullptr"
				<< endl;

		// Free the memory assigned to
		// data member of the object
		delete data;
	}


    ostream & Print( ostream & os)
    {
        os << *data ;
        return os;
    }
};



// Driver Code
int main()
{
	

	// Inserting object of Move class
	Move a{ 10 };
	
    Move c{std::move(a)};
    Move d;
    d=23;
	
    
    
	return 0;
}
