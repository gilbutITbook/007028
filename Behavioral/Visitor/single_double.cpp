#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <memory>
using namespace std;

#include "visitor.hpp"
#include "model.hpp"

struct HtmlVisitor : Visitor
{
	void visit(const Paragraph& p) override
	{
		oss << "<p>" << p.text << "</p>\n";
	}

	void visit(const ListItem& li) override
	{
		oss << "<li>" << li.text << "</li>\n";
	}

	void visit(const List& l) override
	{
		oss << "<ul>\n";
		for_each(l.begin(), l.end(), [&](const ListItem& li)
		{
			// li가 ListItem이라는 것을 알 때만 수용할 수 있다.
			this->visit(li);

			// 만약 li가 Element이면 아래를 수행한다.
			//li.accept(*this);
		});
		oss << "</ul>\n";
	}

	string str() const override
	{
		return oss.str();
	}

private:
	ostringstream oss;
};

struct MarkdownVisitor : Visitor
{
	// dynamic_cast로 if 구문 분기
	void visit(const Element& e)
	{
		if (const Paragraph* p = dynamic_cast<const Paragraph*>(&e))
		{
			visit(*p);
		} else
		{
			// other cases
		}

	}

	void visit(const Paragraph& p) override
	{
		oss << p.text << "\n";
	}
	
	void visit(const BoldParagraph& p) override
	{
		oss << "*" << p.text << "*\n";
	}

	void visit(const ListItem& li) override
	{
		oss << " * " << li.text << "\n";
	}

	void visit(const List& l) override
	{
		oss << "\n";
		for_each(l.begin(), l.end(), [&](const ListItem& li) { this->visit(li); });
		oss << "\n";
	}


	void visit(const Div& div) override
	{
		oss << "<div>\n";
		for (auto e : div)
		{
			//visit(*e);
			e->accept(*this);
		}
		oss << "</div>\n";
	}

	string str() const override
	{
		return oss.str();
	}

private:
	ostringstream oss;
};

// 새로운 클래스가 추가되면 전체적으로 컴파일을 다시해야 한다.

int main_test2(int argc, char* argv[])
{
	BoldParagraph p{ "Here are some colors" };
	ListItem red{ "Red" };
	ListItem green{ "Green" };
	ListItem blue{ "Blue" };
	List colors{ red,green,blue };

	// 단일 디스패치는 한 가지 정보, 어떤 항목을 텍스트로 렌더링 할 것이가? 만 있으면 된다.
	// 이중 디프패치는 두 가지 정보, 어떤 항목을, 어떤 렌더러로 랜더링할 것인가? 가 필요하다.

	vector<Element*> document{ &p, &colors };
	cout << "Textual representation: " << endl;

	MarkdownVisitor v;
	ostringstream oss;
	//for_each(document.begin(), document.end(), [&](const Element* e)
	//{
	//	// v.visit(*e) won't work :), but...
	//	e->accept(v);
	//});
	Div div{ &p, &colors };
	div.print_html(oss);
	cout << oss.str() << endl;

	// 다른 표현방식이 필요하기 전까지는 수정이 필요없다.

	getchar();
	return 0;
}
