#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#define DEFAULT_VALUE "1"

class CNode
{
public:
	CNode();
	CNode(std::string);
	CNode(CNode&);
	~CNode();

	void operator = (CNode& other);
	friend std::ostream& operator << (std::ostream& output, CNode& node);
	enum Identity { NUMERIC, OPERATOR, TRIGONOMETRIC, VARIABLE };

	void setIdentity(Identity); //ustawia tozsamosc
	void setValue(std::string);
	std::string fixIncorrectVariable(std::string); //usuwa niedozwolone znaki ze zmiennej
	bool isNumeric(const std::string&); //sprawdza czy argument jest liczb¹
	bool isOperator(const std::string&); //sprawdza czy argument jest operatorem
	bool isTrigonometric(const std::string&); //sprawdza czy argument jest wartoscia trygonometryczn¹
	bool isCorrectVariable(const std::string&); //sprawdza czy argument jest zmienn¹
	void createNode(std::vector<std::string>&, std::vector<std::string>::iterator&, int&); //metoda zaczyna parsowac wyrazenie w miejscu aktualnej pozycji iteratora
	std::string getValue(); //zwraca wartosc wez³a
	std::vector<CNode*>& getChildren(); //zwraca wektor wskaznikow do dzieci
	Identity getIdentity(); //zwraca typ wez³a

private:
	std::string value;; //wartosc wêz³a
	std::vector<CNode*> children; //wektor wskaŸników do dzieci
	Identity identity; //czym jest wezel
};
