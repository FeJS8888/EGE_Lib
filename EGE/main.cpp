#define MAXCLONESCOUNT 10000
#include "FeEGELib.h"
using namespace std;
void e1goleft(Element* e) {
	e->move_left(5);
	e->turn_left(50);
	if(e->getposition().x <= 0) e->set_posx(600);
}
using FeEGE::getkey;
using FeEGE::getElementById;

void clones_move(Element* e){
//	if(e == nullptr) return;
	e->move_up(5);
//	if(e->getposition().y < -100) e->deletethis();
}

void e0move(Element* e) {
	int speed = 7;
	if(getkey('a') || getkey('A')) if(e->getposition().x - getwidth(e->get_image()) * e->getscale() / 100 / 2 >= speed) e->move_left(speed);
	if(getkey('d') || getkey('D')) if(e->getposition().x + getwidth(e->get_image()) * e->getscale() / 100 / 2 <= getwidth() - speed) e->move_right(speed);
	if(getkey('s') || getkey('S')) if(e->getposition().y + getheight(e->get_image()) * e->getscale() / 100 / 2 <= getheight() - speed) e->move_down(speed);
	if(getkey('w') || getkey('W')) if(e->getposition().y - getheight(e->get_image()) * e->getscale() / 100 / 2 >= speed) e->move_up(speed);
	if(getkey(' ')) {
		cout<<getElementById("2")<<endl;
		Element* clone = getElementById("2")->clone();
		cout<<clone<<endl;
		clone->listen("frame",clones_move);
	}
}



void a(Element* e){
//	Element *s = &(e->clone());
//	e->add_clones(e->clone());
//	cout<<&s<<endl;
	Element* clone = e->clone();
	clone->listen("frame",clones_move);
}

void on_clone(Element* e){
	e->show();
	e->move_to(getElementById("0")->getposition());
	cout<<"OK";
}

int main() {
	setinitmode(INIT_RENDERMANUAL);
	initgraph(400,650); //»­²¼´óÐ¡
	
	PIMAGE e_0 = newimage();
	PIMAGE e_1 = newimage();
	PIMAGE e_2 = newimage();
	getimage(e_0,".//resources//image//plane.png");
	getimage(e_1,".//resources//image//enemy.png");
	getimage(e_2,".//photo.png");
	Element e0 = *(new Element("0",e_0,50,50));
	Element e1 = *(new Element("1",e_1,450,50));
	Element e2 = *(new Element("2",e_2,100,100));
	
	e2.hide();
	e0.decrease_scale(85);
	e1.decrease_scale(87);
	e0.turn_left(180);
	
	reg_Element(&e0);
	reg_Element(&e1);
	reg_Element(&e2);
	
	e0.show();
	e1.show();
	e0.listen("frame",e0move);
	e0.listen("on_click",a);
	e2.listen("on_clone",on_clone);
	e2.listen("clones:on_clone",on_clone);
	start(120);
}
