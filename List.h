/*074090 Œ‚±¶¡÷*/
#ifndef _MY_CLIST_H
#define _MY_CLIST_H
#define OUT_OF_RANGE 0
template<class T>
struct MyEntry
{
	T item;
	MyEntry<T>* next;

};

template<class T>
class CMyList
{
public:
	CMyList();
	void Add(const T& item);
	T& GetItem(int position);
	T& operator[](int position)
	{
		return GetItem(position);
	}
	int  size()
	{return count;}
private:
	MyEntry<T>* entries;
	int count;
};


template<class T>
CMyList<T>::CMyList() 
{
	count = 0;
	entries = NULL;
}

template<class T>
void CMyList<T>::Add(const T &item)
{
	MyEntry<T>* newnode = new MyEntry<T>;
	newnode->item = item;
	newnode->next = entries;
	entries = newnode;
	count ++;
}

template<class T>
T& CMyList<T>::GetItem(int position)
{
	if(position >= count)
		throw OUT_OF_RANGE;
	MyEntry<T>* head = entries;
	for(int i = 0;i<position; i++)
	{
		head = head->next;
	}
	return head->item;
}

#endif