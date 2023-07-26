#define MAXCLONESCOUNT 10000
#define MAXELEMENTCOUNT 500
#include "FeEGELib.h"
#include <iomanip>
using namespace std;
void e1goleft(Element* e) {
	e->move_left(5);
	e->turn_left(50);
	if(e->getposition().x <= 0) e->set_posx(600);
}
using FeEGE::getkey;
using FeEGE::getElementById;

void clones_move(Element* e) {
	e->move_up(10);
	if(e->getposition().y < -100) e->deletethis();
	if(e->is_touched_by(getElementById("enemy"))) {
		e->deletethis();
		getElementById("enemy")->set_image(1);
		cout<<"[Info] Touched enemy\n";
	}
}

void on_clone(Element* e) {
	e->show();
	e->decrease_scale(82);
	Position pos = getElementById("self")->getposition();
	e->move_to(pos.x + 2,pos.y - 11);
//	cout<<getElementById("0")->getposition().x<<" "<<getElementById("0")->getposition().y<<endl;
}

void e0move(Element* e) {
	int speed = 7;
	if(getkey('a') || getkey('A')) if(e->getposition().x - getwidth(e->get_image()) * e->getscale() / 100 / 2 >= speed) e->move_left(speed);
	if(getkey('d') || getkey('D')) if(e->getposition().x + getwidth(e->get_image()) * e->getscale() / 100 / 2 <= getwidth() - speed) e->move_right(speed);
	if(getkey('s') || getkey('S')) if(e->getposition().y + getheight(e->get_image()) * e->getscale() / 100 / 2 <= getheight() - speed) e->move_down(speed);
	if(getkey('w') || getkey('W')) if(e->getposition().y - getheight(e->get_image()) * e->getscale() / 100 / 2 >= speed) e->move_up(speed);
	if(getkey(' ')) {
		if(e->get_variable(1) + 20 < frame) {
			Element* clone = getElementById("2")->clone();
			clone->listen("frame",clones_move);
			e->set_variable(1,frame);
		}
	}
//	if(e->is_touched_by(getElementById("enemy"))){
//		e->cancel_move();
//	}
}
//
void enemy_frame(Element* e) {
	if(e->get_image_order() == 1) {
		int statu = e->get_variable(1);
		if(statu >= 2) {
			e->set_variable(1,0);
			e->set_image(0);
		} else e->set_variable(1,statu + 1);
	}
}

void a(Element* e) {
//	Element *s = &(e->clone());
//	e->add_clones(e->clone());
//	cout<<&s<<endl;
	Element* clone = e->clone();
	clone->listen("frame",clones_move);
}



int main() {
	setinitmode(INIT_RENDERMANUAL);
	initgraph(400,650); //ª≠≤º¥Û–°
/*
	PIMAGE e = newimage();
	getimage(e,".\\test.png");
	cout<<getwidth(e)<<" "<<getheight(e)<<endl;;
	for(int i = 0;i < getwidth(e);++ i){
		for(int j = 0;j < getheight(e);++ j) cout<<setw(4)<<EGEGET_A(getpixel(i,j,e))<<" ";
		cout<<"\n";
	} 
*/
	PIMAGE e_0 = newimage();
	PIMAGE enemy = newimage();
	PIMAGE enemy_hurt = newimage();
	PIMAGE e_2 = newimage();
	getimage(e_0,".//resources//image//plane.png");
	getimage(enemy,".//resources//image//enemy.png");
	getimage(enemy_hurt,".//resources//image//enemy_hurt.png");
	getimage(e_2,".//photo.png");
	Element e0 = *(new Element("self",e_0,50,550));
	Element e1 = *(new Element("enemy",enemy,50,50));
	Element e2 = *(new Element("2",e_2,100,100));

	e1.add_image(enemy_hurt);
	e2.hide();
	e1.hide();
	e0.increase_order(1);
	e1.increase_order(1);
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
//	e1.listen("frame",enemy_frame);
	e2.listen("clones:on_clone",on_clone);
	
	Any *a;
	a = new Any;
	a->CHAR = 'a';
	
//	e0.deletethis();
	start(60);
}
