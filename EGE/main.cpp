#include<graphics.h>
#include<vector>
#include<algorithm>
#include<iostream>
#include<queue>
using namespace std;

int keymsg;
bool mousehit;
mouse_msg mouseinfo;

//Classes
class Position {
	public:
		//Variables
		int x;
		int y;

		// Functions
		Position(int x = 0,int y = 0) {
			this->x = x;
			this->y = y;
		}
		~Position() {}
};

class Element {
	private:
		//Variables
		unsigned short scale;
		int angle;
		int order;
		int reg_order;
		Position pos;
		bool is_show;
		vector<PIMAGE> image_vector;
		vector<void(*)(Element*)> function_vector;
		unsigned int current_image = 0;
		int private_variables[10];
		inline void reflush_mouse_statu(){
			
		}
	public:
		//Functions
		Element(PIMAGE image,Position pos) {
			this->pos = pos;
			this->image_vector.push_back(image);
			this->is_show = true;
			this->scale = 100;
			this->angle = 0;
			this->order = 0;
		}
		Element(PIMAGE image,int x = 0,int y = 0) {
			this->pos = *(new Position(x,y));
			this->image_vector.push_back(image);
			this->is_show = true;
			this->scale = 100;
			this->angle = 0;
			this->order = 0;
		}
		void call() {
			this->reflush_mouse_statu();
			for(int i = 0;i < this->function_vector.size();++ i){
				this->function_vector[i](this);
			}
			if(!this->is_show) return;
			PIMAGE image = this->image_vector[this->current_image];
			putimage_rotatezoom(NULL,image,this->pos.x,this->pos.y,0.5,0.5,this->angle / 180.00f,this->scale / 100.00f,1);
		}
		inline void move_left(int pixels = 0) {
			this->pos.x -= pixels;
		}
		inline void move_right(int pixels = 0) {
			this->pos.x += pixels;
		}
		inline void move_up(int pixels = 0) {
			this->pos.y -= pixels;
		}
		inline void move_down(int pixels = 0) {
			this->pos.y += pixels;
		}
		inline void set_scale(unsigned short scale) {
			this->scale = scale;
		}
		inline void increase_scale(unsigned short scale) {
			if(this->scale + scale <= 100) this->scale += scale;
			else this->scale = 100;
		}
		inline void decrease_scale(unsigned short scale) {
			if(this->scale > scale) this->scale -= scale;
			else this->scale = 0;
		}
		inline void hide() {
			this->is_show = false;
		}
		inline void show() {
			this->is_show = true;
		}
		inline void turn_right(int angle) {
			this->angle = (this->angle - (int)(angle / 360.00f * 1130)) % 1130;
		}
		inline void turn_left(int angle) {
			this->angle = (this->angle + int(angle / 360.00f * 1130)) % 1130;
		}
		inline void turn_to(int angle) {
			this->angle = (int)(angle / 360.00f * 1130) % 1130;
		}
		inline unsigned short getscale() {
			return this->scale;
		}
		inline int getangle() {
			return this->angle;
		}
		inline Position getposition() {
			return this->pos;
		}
		inline bool getisshow() {
			return this->is_show;
		}
		inline void increase_order(int count) {
			this->order += count;
		}
		inline void decrease_order(int count) {
			this->order -= count;
		}
		inline void set_order(int count) {
			this->order = count;
		}
		inline void set_reg_order(int count) {
			this->reg_order = count;
		}
		inline int getreg_order() {
			return this->reg_order;
		}
		inline int getorder() {
			return this->order;
		}
		bool operator<(Element _B) {
			if(this->getorder() > _B.getorder()) return false;
			else if (this->getorder() < _B.getorder()) return true;
			return this->getreg_order() < _B.getreg_order();
		}
		bool operator>(Element _B) {
			if(this->getorder() < _B.getorder()) return false;
			else if (this->getorder() > _B.getorder()) return true;
			return this->getreg_order() > _B.getreg_order();
		}
		inline void set_posx(int x){
			this->pos.x = x;
		}
		inline void set_posy(int y){
			this->pos.y = y;
		}
		inline void move_to(int x,int y){
			this->pos.x = x; 
			this->pos.y = y;
		}
		inline void move_to(Position pos){
			this->pos = pos;
		}
		inline void push_function(auto function){
			this->function_vector.push_back(function) ;
		}
		inline bool ismousein(int x,int y){
			int d_width = getwidth(this->image_vector[this->current_image]) / 2;
			int d_height = getheight(this->image_vector[this->current_image]);
			return (x >= this->pos.x - d_width && x <= this->pos.x + d_width && y >= this->pos.y - d_height && y <= this->pos.y + d_height);
		}
		inline bool ishit(){
			if(!mousehit) return false;
			int x,y;
			mousepos(&x,&y);
			return this->ismousein(x,y);
		}
		inline void set_variable(unsigned int which,int value){
			this->private_variables[which] = value;
		}
		inline int get_variable(unsigned int which){
			return this->private_variables[which];
		}
		~Element() {}
};
vector<Element*> Element_queue;
int current_reg_order = 0;


//Functions

void reg_Element(Element* element) {
	element->set_reg_order(current_reg_order ++);
	Element_queue.push_back(element);
}

void reflush() {
	cleardevice();
	vector<Element>compared ;
	for(int i = 0; i < Element_queue.size(); ++ i) {
		compared.push_back(*Element_queue[i]);
	}
	sort(compared.begin(),compared.end());
	for(int i = 0; i < compared.size(); ++ i) {
		Element_queue[compared[i].getreg_order()]->call();
	}
	delay_ms(0);
	keymsg = -1;
	mousehit = false;
	if(kbhit()) keymsg = getch();
	if(mousemsg()){
		mousehit = true;
		mouseinfo = getmouse();
	}
	flushkey();
}

void fun(Element* e){
//	e->move_right(1);
	if(e->ishit()) e->set_variable(0,1);
	else {
		if(!e->get_variable(0)) return;
		int x,y;
		mousepos(&x,&y);
		if(e->ismousein(x,y)){
			e->set_variable(0,0);
			e->hide();
		}
	}
}
void fun1(Element* e){
	e->move_down(1);
}
void fun2(Element* e){
	e->decrease_scale(1);
	if(e->getscale() == 0) e->set_scale(100);
}

int main() {
	initgraph(500,500); //ª≠≤º¥Û–° 
	
	PIMAGE e0_image = newimage();
	getimage(e0_image,".//photo.png");
	Element e0 = *(new Element(e0_image,50,50));
	reg_Element(&e0);
	e0.show(); 
	e0.push_function(fun);
//	e0.push_function(fun1);
//	e0.push_function(fun2); 
	while(true){
//		if(keymsg == (int)('a' + ('w' - 'a'))) e0.move_up(5);
//		if(keymsg == (int)('a' + ('a' - 'a'))) e0.move_left(5);
//		if(keymsg == (int)('a' + ('s' - 'a'))) e0.move_down(5);
//		if(keymsg == (int)('a' + ('d' - 'a'))) e0.move_right(5);


		cout<<mousehit<<" "<<e0.ishit()<<endl;
		reflush();
	}
}

