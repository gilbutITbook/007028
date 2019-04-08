#pragma once
#include <string>

struct Paragraph;
struct BoldParagraph;
struct ListItem;
struct List;
struct Div;

struct Visitor
{
	virtual ~Visitor() = default;

	virtual void visit(const Paragraph& p) = 0;
	virtual void visit(const BoldParagraph& p) = 0;
	virtual void visit(const ListItem& li) = 0;
	// 여기가 문제이다.
	virtual void visit(const List& l) = 0;
	virtual void visit(const Div& div) = 0;

	// 새로운 타입이 추가될때 마다 방문자 함수를 추가해야 한다.

	virtual std::string str() const = 0;
};
