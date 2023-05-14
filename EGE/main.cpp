#include "FeEGELib.h"
using namespace std;

void s1(Element* e) {
	e->increase_scale(50);
}

void s2(Element* e) {
	e->decrease_scale(50);
}
void fun2(Element* e) {
	e->hide();
}
void e0goright(Element* e){
	e->move_right(5);
	e->turn_right(5);
	if(e->getposition().x >= 600) e->set_posx(-100);
}
void e1goleft(Element* e){
	e->move_left(5);
	e->turn_left(5);
	if(e->getposition().x <= 0) e->set_posx(600);
}

int main() {
	setinitmode(INIT_RENDERMANUAL);
	initgraph(500,500); //ª≠≤º¥Û–°
	PIMAGE e_0 = newimage();
	PIMAGE e_1 = newimage();
	getimage(e_0,".//resources//image//0.png");
	getimage(e_1,".//resources//image//1.png");
	Element e0 = *(new Element(e_0,50,50));
	Element e1 = *(new Element(e_1,450,50));
	e0.decrease_scale(50);
	reg_Element(&e0);
	reg_Element(&e1);
	e0.show();
	e1.show();
	e0.listen("on_mouse_put_on",s1);
	e0.listen("on_mouse_move_away",s2);
	e0.listen("on_click",fun2);
	e0.listen("frame",e0goright);
	e1.listen("frame",e1goleft);
	while(true) {
		reflush();
		cout<<e0.is_touched_by(&e1);
		delay_fps(120);
	}
}

