/* this file implemant linklist without using using stl */
/* linear collection of data elements */
#include <algorithm>

template<typename T>
class node
{
private:
	T *data;
	class node *next;
public:
	node(T *Userdata)
	{
		data = Userdata;
	}
};

template<typename T>
class linklist
{
private:
	node<T> *head;
public:	
	// constructor
	linklist()
	{
		head=NULL;
	}
	// distructor
	~linklist()
	{
		node<T> *temp,*prev;
		temp = head;
		while(temp)
		{
			prev = temp;
			temp = temp->next;
			delete prev;
		}
	}
	// push to the lats
	bool push_back(T *data)
	{
		node<T> *temp,*curr;
		temp = new node<T>;
		if(temp == nullptr)
		{
			return false;
		}
		temp->data=data;
		temp->next=NULL;
		if(head == NULL)
		{
			head=temp;
		}
		else
		{
			curr = head;
			while(curr->next)
			{
				curr = curr->next;
			}
			curr->next = temp;
		}
		return true;
	}
	// push to the front
	bool push_front(T *data)
	{
		node<T> *temp,*curr;
		temp = new node<T>;
		if(temp == nullptr)
		{
			return false;
		}
		temp->data=data;
		temp->next=NULL;
		temp->next = head;
		head = temp;
		return true;
	}
	// pop the front member of linklist
	T *pop_front(void)
	{
		node<T> *temp;
		if(head == NULL)
		{
			return NULL;
		}
		temp = head;
		head = head->next;
		T *data = head->data;
		delete temp;
		return data;
	}
	// pop the last member of linklist
	T *pop_back(void)
	{
		node<T> *temp,*prev;
		if(head == NULL)
		{
			return NULL;
		}
		if(head->next == NULL)
		{
			return pop_front();
		}
		temp = head;
		while(temp->next)
		{
			prev = temp;
			temp = temp->next;
		}
		prev->next = NULL;
		T *data = temp->data;
		delete temp;
		return data;
		
	}
	// pop by index
	T *at(int index)
	{
		node<T> *temp,*prev;
		if(head == NULL)
		{
			return NULL;
		}	
		if(index == 0)
		{
			temp = head;
			head = head->next;
			T *data = head->data;
			delete temp;
		}
		temp = head;
		
		while(index--)
		{
			prev = temp;
			temp = temp->next;
			if(temp == NULL)
			{
				return NULL;
			}
		}
		prev->next = temp->next;
		T *data = temp->data;
		delete temp;
		return data;
	}
};
