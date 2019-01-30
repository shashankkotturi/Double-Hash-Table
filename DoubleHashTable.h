#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H


/****************************************
 * UW User ID:  vskottur
 * Submitted for ECE 250
 * Semester of Submission:  Fall 2017
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    - s72rahma
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    - s72rahma (debugging, test cases)
 ****************************************/
 
 
 /*
 * DoubleHashTable
 *
 * Shashank Kotturi
 * 2017-10-22
 *
 * The DoubleHashTable class implements a hash table structure using
 * two arrays. One array stores objects of type T (integer or double)
 * and the other stores objects of type state (EMPTY, OCCUPIED, DELETED).
 * Two hash functions are used to store values within the hashtable. The
 * primary function maps the value to a certain bin and if a collision
 * is detected, the secondary function is then used to resolve the collision.
 *
 * Member Variables:
 *      T *array                 An array of objects of type T. This array will 
 *                               contain the values placed in the hash table.
 *      
 *      state *array_state       An array of objects of type “state” – to store
 *                               the status of the bin. The state of a bin is 
 *                               one of three possible values: EMPTY, OCCUPIED,
 *                               or DELETED.
 *
 *		int count		         The number of elements currently in hashtable.
 *                                   Must be greater than or equal to 0.
 *
 *		int array_size           Current capacity of the hashtable.
 *                                   Must be greater than or equal to 1.
 *
 *		int power                 This is associated with the capacity of the
 *                                hash table (array_size = 2^power).
 *
 *
 * Member Functions (Accessors):
 *
 *      int size() const
 *          Return the number of elements currently stored in the hash table.(O(1))
 *
 *      int capacity() const
 *          Return the number of bins in the hash table. (O(1))
 *      
 *      bool empty() const
 *           Return true if the hash table is empty, false otherwise. (O(1))
 *
 *      bool member( T const & ) const
 *           Return true if object obj is in the hash table and false otherwise. (O(1))
 *
 *      T bin( int n ) const
 *           Return the entry in bin n. The behaviour of this function is
 *           undefined if the bin is not filled. It will only be used to test
 *           the class with the expected locations. (O(1))
 *
 *      void print() const 
 *           A function which you can use to print the class in the testing
 *           environment.
 *
 *
 * Member Functions (Mutators):
 *
 *      void insert ( T const & )
 *           Insert the new object into the hash table in the appropriate bin
 *           as determined by the two aforementioned hash functions and the rules
 *           of double hashing. If the table is full, throw an overflow exception. (O(1))
 *
 *		bool remove( T const & )
 *           Remove the object from the hash table if it is in the hash table 
 *           (returning false if it is not) by setting the corresponding flag of
 *            the bin to deleted. This may throw an underflow exception. (O(1))
 *
 *		void clear()
 *          Remove all elements in the hash table. (O(M))
 *
 *
 * Bugs:           None detected as of October 22, 2017
 * Todo:           Nothing left as of October 22, 2017
 * References:     For commenting code: https://ece.uwaterloo.ca/~dwharder/icsrts/C/02/
 */
 
 

#include "Exception.h"
#include "ece250.h"
#include <iostream>
using namespace std;

enum state { EMPTY, OCCUPIED, DELETED };

template<typename T>
class DoubleHashTable {
	private:
		int count; // Number of Elements in the HashTable
		int power;
		int array_size; // Size of HashTable
		T *array;
		state *array_state;

		int h1( T const & ) const; // first hash function
		int h2( T const & ) const; // second hash function

	public:
		DoubleHashTable( int = 5 );
		~DoubleHashTable();
		int size() const;
		int capacity() const;		
		bool empty() const;
		bool member( T const & ) const;
		T bin( int ) const;

		void print() const;

		void insert( T const & );
		bool remove( T const & );
		void clear();
};

/*
 * DoubleHashTable<T >::DoubleHashTable( int m )
 *
 * Constructor initialization for hash table implementation
 * 
 * Initializes size of hash table (count) to 0.
 * Initializes array_size to a value of m.
 * Default initialization value for array_size is 5 as called above.
 * A T *array and state *array_state of size array_size is initialized.
 * T *array is initialized with "0" and state *array_state is initialized with
 * "EMPTY".
 *
 * Parameter m is the specified size of the array by user.
 *           m must be greater than or equal to 1.
 *
 */

template<typename T >
DoubleHashTable<T >::DoubleHashTable( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
array( new T [array_size] ),
array_state( new state[array_size] ) {

	for ( int i = 0; i < array_size; ++i ) {
		array_state[i] = EMPTY;
		array[i] = 0;
	}
}

/*
 * DoubleHashTable<T >::~DoubleHashTable()
 *
 * Destructor implentation for class DoubleHashTable.
 *
 * Deletes all contents of array and array_state.
 *
 */

template<typename T >
DoubleHashTable<T >::~DoubleHashTable() {
	delete[] array;
	delete[] array_state;
}

/*
 * int DoubleHashTable<T >::size() const
 *
 * Returns number of elements currently stored in the hash table.
 * Does not return a negative value.
 *
 * Runs in O(1).
 *
 * Returns:
 *  The number of elements currently stored in the hash table.
 */

template<typename T >
int DoubleHashTable<T >::size() const {
	return count;
}

/*
 * int DoubleHashTable<T >::capacity() const
 *
 * Returns number of bins in the hash table.
 *
 * Runs in O(1).
 *
 * Returns:
 *  The number of bins in the hash table.
 */

template<typename T >
int DoubleHashTable<T >::capacity() const {
	return array_size;
}

/*
 * bool DoubleHashTable<T >::empty() const
 *
 * Returns true if size of hash table is 0, i.e., hash table is empty.
 * Returns false if size of hash table is not 0, i.e., hash table is not empty.
 *
 * Runs in O(1).
 * 
 * Returns:
 *  True if hash table is empty, otherwise false.
 */

template<typename T >
bool DoubleHashTable<T >::empty() const { 
	return !(count > 0);
}

/*
 * int DoubleHashTable<T >::h1( T const &obj ) const
 *
 * Returns array index of bin to insert obj in hash table.
 * Calculate index by converting obj to int i using static_cast<int>.
 * Array index = i % array_size. If index < 0, index += array_size.
 *
 * Runs in O(1).
 * 
 * Returns:
 *  Array index of bin to insert obj in hash table.
 */

template<typename T >
int DoubleHashTable<T >::h1( T const &obj ) const {
	
	int i = static_cast<int>(obj);
	i %= array_size;
	
	// Add array_size to ensure i > 0
	if (i < 0)
	    i += array_size;
	
    return i;
}

/*
 * int DoubleHashTable<T >::h2( T const &obj ) const
 *
 * Returns offset for array index of bin to insert obj in hash table.
 * Calculate index by converting obj to int i using static_cast<int>.
 * Array index = (i / array_size) % array_size.
 * If index < 0, index += array_size.
 * If index is even, add 1 to make it odd.
 *
 * Runs in O(1).
 * 
 * Returns:
 *  Offset for array index of bin to insert obj in hash table.
 */

template<typename T >
int DoubleHashTable<T >::h2( T const &obj ) const {
	
    int i = static_cast<int>(obj);
	i /= array_size;
	i %= array_size;
	
	// Add array_size to ensure i > 0
    if (i < 0)
	   i += array_size;
	
    // Ensure an odd jump is made if i becomes even   
	if (i %2 == 0)
	   i += 1;

	return i;
}

/*
 * bool DoubleHashTable<T >::member( T const &obj ) const
 *
 * Checks if object is in the hash table. Returns true if object is in the hash
 * table. Else, it returns false.
 *
 * Checks if the bin value from the first hash function (h1) contains the object.
 * If yes, i.e., the bin contains the object and the bin is not EMPTY, the 
 * function returns true. If the object is not in the bin, the function uses the
 * offset from the second hash function (h2) to go through the hash table a 
 * maximum of the capacity of the hash table to find the element.
 *
 * When iterating through the hash table, the function checks these conditions:
 * 1) Bin contains object value AND
 * 2) Bin is OCCUPIED
 *
 * Once the bin with the object is found, the function returns true.
 *
 * Runs in O(1).
 *
 * Returns:
 *  True if object is in the hash table. Else, false.
 */

template<typename T >
bool DoubleHashTable<T >::member( T const &obj ) const {
	int probe = h1(obj);
	int offset = h2(obj);
	int traversals = 0;
	
	// Returns true if the bin value from the first hash function (h1) contains
	// the object.
	if(array[probe] == obj && array_state[probe] != EMPTY)
		return true;
    
    // Uses offset from the second hash function (h2) to iterate.
    // Iterates through the hash table a maximum of the size of the hash table.
    // Iterates when object is not in the bin OR if the bin is not OCCUPIED AND
    //  bin is not empty AND traversals is less than the size of the hash table.
	if(array[probe] != obj) {
		while((array[probe] != obj || array_state[probe] != OCCUPIED) && array_state[probe] != EMPTY && traversals < array_size) {
			probe += offset;
			probe %= array_size;
            
            // Returns true if the bin in the hash table contains the object.
			if(array[probe] == obj && array_state[probe] != EMPTY)
				return true;

			traversals++;
		}
		return false;
	}
	
	
	return false;
	
}

/*
 * T DoubleHashTable<T >::bin( int n ) const 
 *
 * Return the entry in bin n. The behaviour of this function is undefined if the
 * bin is not filled. It will only be used to test the class with expected
 * locations.
 *
 * Runs in O(1).
 *
 * Returns:
 *  The entry in bin if the bin is empty.
 */
 
template<typename T >
T DoubleHashTable<T >::bin( int n ) const {
                    
    if((n < 0) || (n >= array_size))
       throw ArrayBoundsException();
    return (array[n]);
}


/*
 * void DoubleHashTable<T >::insert( T const &obj )
 *
 * May throw an overflow exception if hash table has reached maximum capacity.
 *
 * Inserts new object into the hash table using hash functions h1 and h2.
 * If bin returned from h1 is OCCUPIED, offset from h2 is added continuously
 * the bin value from h1 until an EMPTY bin is found. Once an EMPTY bin is found,
 * obj is inserted into the specific bin. The state of the specific bin is
 * changed to OCCUPIED and the number of elements (count) in the hash table 
 * is incremented.
 *
 * Runs in O(1).
 */
 
template<typename T >
void DoubleHashTable<T >::insert( T const &obj ) {
	 if (count >= array_size)
        throw overflow();
     
     int probe = h1(obj);
     int offset = h2(obj);
     
     // Collision resolved by creating new bin value from offset using h2
     while(array_state[probe] == OCCUPIED){
        probe += offset;
        probe %= array_size;
     }
     
     // Inserting element into hash table by inserting the value into the
     // specific bin, changing the state of the specific bin to OCCUPIED, and
     // incrementing the number of elements (count) in the hash table.
     array[probe] = obj;
     array_state[probe] = OCCUPIED;
     count++;
     	
	 return ; 
}

/*
 * bool DoubleHashTable<T >::remove( T const &obj ) const
 *
 * Checks if object is in the hash table. Returns true if object is in the hash
 * table and the specified object is removed, the state of the bin is changed to
 * DELETED and the number of elements in the hash table (count) is decremented.
 * Else, it returns false.
 *
 * Checks if the bin value from the first hash function (h1) contains the object.
 * If yes, i.e., the bin contains the object and the bin is not EMPTY, the 
 * function returns true after deleting the object, the state of the bin is changed to
 * DELETED and the number of elements in the hash table (count) is decremented.
 
 * If the object is not in the bin, the function uses the
 * offset from the second hash function (h2) to go through the hash table a 
 * maximum of the capacity of the hash table to find the element.
 *
 * When iterating through the hash table, the function checks these conditions:
 * 1) Bin contains object value AND
 * 2) Bin is OCCUPIED
 *
 * Once the bin with the object is found, the function returns true after 
 * deleting the object, the state of the bin is changed to DELETED and the 
 * number of elements in the hash table (count) is decremented.
 *
 * Runs in O(1).
 *
 * Returns:
 *  True if object is in the hash table and object is removed. Else, false.
 */

template<typename T >
bool DoubleHashTable<T >::remove( T const &obj ) {
	int probe = h1(obj);
	int offset = h2(obj);
	int traversals = 0;
	
	// Returns true if the bin value from the first hash function (h1) contains
	// the object and it is removed, the state of the bin is changed to DELETED,
	// and the number of elements in the hash table( count) is decremented.
	if(array[probe] == obj && array_state[probe] != EMPTY){
		array_state[probe] = DELETED;
		array[probe] = 0;
		count--;
		return true;
    }

    // Uses offset from the second hash function (h2) to iterate.
    // Iterates through the hash table a maximum of the size of the hash table.
    // Iterates when object is not in the bin OR if the bin is not OCCUPIED AND
    //  bin is not empty AND traversals is less than the size of the hash table.
	if(array[probe] != obj) {
		while((array[probe] != obj || array_state[probe] != OCCUPIED) && array_state[probe] != EMPTY && traversals < array_size) {
			probe += offset;
			probe %= array_size;

            // Returns true if the bin in the hash table contains the object.
            // The object is removed, the state of the bin is changed to DELETED,
            // and the number of elements in the hash table( count) is decremented.
			if(array[probe] == obj && array_state[probe] != EMPTY) {
				array_state[probe] = DELETED;
				array[probe] = 0;
				count--;
				return true;
			}
			traversals++;
		}
		return false;
	}
	
	return false;
	
}

/*
 * void DoubleHashTable<T >::clear()
 *
 * Removes all elements in the hash table.
 *
 * Runs in O(M).
 */

template<typename T >
void DoubleHashTable<T >::clear() {
	 for (int i = 0; i < array_size; i++){
         array[i] = 0;
         array_state[i] = EMPTY;
     }
     count = 0;	
	 return ; 
}

/*
 * void DoubleHashTable<T >::print() const
 *
 * Used to print the class in the testing environment.
 *
 */

template<typename T >
void DoubleHashTable<T >::print() const {
      for (int i = 0; i < array_size; i++){
          cout << array[i] << " - " << array_state[i] << endl;
      }	
	return;
}

#endif
