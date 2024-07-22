#include <iostream>
#include <string>
using namespace std;
class Score
{
public:
	void Input();
	void Evalauate();
	void Output();
private:
	string name;
	double s[4];
	double total;
	char grade;
};
void Score::Input()
{
	cin>>name;
	cin>>s[0];
	cin>>s[1];
	cin>>s[2];
	cin>>s[3];
}
void Score::Evalauate()
{
	total=(s[0]+s[1])/4+s[2]/4+s[3]/2;
	if(total>=90&&total<=100)
		grade='A';
	else if(total>=80&&total<=89)
		grade='B';
	else if(total>=70&&total<=79)
		grade='C';
	else if(total>=60&&total<=69)
		grade='D';
	else
		grade='E';
}
void Score::Output()
{
	if(s[0]<0||s[0]>50||s[1]<0||s[1]>50||s[2]<0||s[2]>100||s[3]<0||s[3]>100)
		cout<<"error"<<endl;
	else
		cout<<"name: "<<name<<", "<<"total: "<<total<<", "<<"grade: "<<grade<<endl;
}
int main()
{
    Score *s1=new Score;
    s1->Input();
    s1->Evalauate();
    s1->Output();
    return 0;
}
