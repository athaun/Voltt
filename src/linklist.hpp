#pragma once

#include <fstream>
#include <iostream>
#include "mem.hpp"

template<typename Elem, typename Precision>
struct Node {
	RelPtr<Node<Elem, Precision>, Precision> next;
	Elem data;

	explicit Node(const Elem& _data)
	: data(_data), next()
	{}
};


template<typename Elem, typename Precision = int32_t>
struct LinkList {
	using Node_t = Node<Elem, Precision>;
	
	Node_t* root;
	Node_t* head;

	auto push(const Elem& _data) -> void
	{
		Node_t* new_head = new Node_t(_data);
		if (root == nullptr) {
			root = new_head;
			return;
		}

		switch (root->next->base) {
			case 0:
				root->next = new_head;
				head = new_head;
				return;
			default:
				head->next = new_head;
				head = new_head;
				return;
		}
	}

	auto pop() -> Elem 
	{
		Elem data = head->data;
		head = nullptr;
		return data;
	}

	auto peek() -> Elem&
	{
		return head->data;	
	}

	auto dump(std::ostream& _os) -> void
	{
		if (root == nullptr) return;
		Node_t* curr = root;
		while ( curr != nullptr ) {
			_os << curr->data << '\n';
			switch (curr->next->base) {
				case 0: return; 
				default: curr = curr->next->get();
			}
		}
	}

	explicit LinkList()
	: root(nullptr), head(nullptr)
	{}

	explicit LinkList(const Elem& _data)
	: root(new Node_t(_data)), head(nullptr)
	{}

	~LinkList()
	{
		Node_t* curr = root;
		Node_t* next;
		
		while ( curr != nullptr ) {
			next = curr->next->get();
			delete curr;
			curr = next;
		}

		root = nullptr;
		head = nullptr;
	}
};
