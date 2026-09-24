1. In LinkedList::deleteFront(), why does it take two separate delete calls instead of one?
   Name exactly what each one frees, and name the two new calls back in the program responsible
   for putting them on the heap in the first place.

It takes two different calls because two separate things were allocated on the heap and each delete 
frees one. delete doomed->data frees the T object, the one created by new int() or new Data() 
and what not back in main.cpp. The node only holds a pointer to that object, so deleting the node doesn't 
clean it up. delete doomed frees the Node<T> itself, the one created by new Node<T>() inside LinkedList::addFront. 
If I only called one of them, the other would leak.

2. ArrayList never had a destructor before today. Explain, in your own words, why switching
   from T data[CAPACITY] to T* data [CAPACITY] is what made a destructor necessary, and what
   would happen if you forgot to write one. Would you get a compiler error? Why or why not?

The original way let the values live inside the ArrayList object, so when the ArrayList died the compiler's default 
destructor cleaned up each T on the way out and I didn't need to write anything. When that changed, the array only 
holds pointers, and the T objects they point to live on the heap instead of inside the ArrayList. Destroying a pointer 
does nothing to what it points at, so unless I delete data_[i] for each slot, every T that was added with new in 
main.cpp would leak, which is why I wrote ~ArrayList() override. If I had forgotten it, there would be no compiler 
error, because leaks are a runtime problem and not something the compiler can catch from the types.

3. search() and addFront() both take a T*, but they treat that pointer completely differently.
   Explain the difference in terms of ownership: which one is allowed to delete what you hand it,
   and which one is never allowed to?

addFront(T* value) takes ownership, so once I hand it a pointer, the container is responsible for it. That's why the 
destructor deletes every stored pointer and why deleteFront() does delete doomed->data. I shouldn't delete that 
pointer afterward myself, since the list will do it, either when I call deleteFront or when the list is destroyed. On 
the other hand, search(T* value) is the opposite, it never takes ownership. It's const, so it won't change the list, 
and by design it only reads *value to compare against each element without storing or deleting it. That's why I can 
safely pass &key, a stack variable, from main.cpp. If search tried to delete &key, it would be deleting a stack 
address, which is undefined behavior and usually a crash. So the rule is that if the container stores the pointer, 
it owns it, and if it only reads through the pointer for comparison, then it's just borrowing.

4. You swapped LinkedList<T> for ArrayList<T> inside makeList() and reran main.cpp without
   changing a single line there. What two mechanisms, by name, made that possible?

The two mechanisms are runtime polymorphism (through virtual functions) and the factory function makeList<T>(). For 
the first mechanism, both LinkedList<T> and ArrayList<T> publicly inherit from List<T> and override every pure virtual
method. main.cpp only ever holds a std::unique_ptr<List<T>>, so it never knows or cares which concrete type is behind 
it. virtual in List.h is what makes the right function get called at runtime, and override in the subclasses just 
makes the compiler check that each signature really matches. As for the second one, it's the only place in the program 
that names a concrete type. main.cpp just calls makeList<int>() and makeList<Data>(), so swapping the commented line 
inside makeList changes the whole program's behavior without editing main.cpp at all.

5. Pick one keyword from the Key Terms glossary that you either had to add today or wouldn't have
   thought to add on your own (explicit, override, virtual, const, or any other). Describe, in
   your own words and without copying the guide's wording, the smallest example you can think
   of where leaving it out would cause a real problem.

The keyword I picked is explicit, which was added on Node's constructor. Without it, the constructor would act as a 
converting constructor, and the compiler would silently turn a T* into a Node<T> whenever it needed one which causes issues, so a line 
like Node<int> n = new int(8); would compile even though that's not what I meant. With explicit, that line is a 
compile error and forces me to write Node<int> n(new int(8));, which is what I want. The smallest real problem it 
prevents is the compiler quietly letting a caller pass a borrowed pointer like a stack address where a Node<T> is 
expected. If that node ends up in the list, the list will eventually delete that pointer, which is undefined behavior. 
The conversion is silent, so the caller never knows they just signed over ownership of something they don't own.