// PQ1: Follows STL implementation of erase()
template<class Object>
Vector<Object>::iterator Vector<Object>::erase(Vector<Object>::iterator vItr) {

	// Check if vItr is valid
	if (vItr < begin() || vItr >= end()) { return end(); }

	// Shift elements after vItr over by one
	for (Vector<Object>::iterator itr = vItr; itr < (end() - 1); itr++)
		*itr = move(*(itr + 1));

	// Update the size, since we removed one element
	theSize--;

	// Returns the position the iterator pointed to
	// Note that this isn't invalidated, since all we did was shift over the values
	return vItr;
}