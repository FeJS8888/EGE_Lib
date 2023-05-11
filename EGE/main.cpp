#include<graphics.h>
#include<vector>
#include<string>
#include<algorithm>
#include<iostream>
#include<queue>
using namespace std;

int keymsg;
bool mousehit = false;
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
		PIMAGE __visible_image;
		vector<void(*)(Element*)> frame_function_vector;
		vector<void(*)(Element*)> on_mouse_put_on_function_vector;
		vector<void(*)(Element*)> on_mouse_move_away_function_vector;
		vector<void(*)(Element*)> on_click_function_vector;
		unsigned int current_image = 0;
		int private_variables[10];
		inline void reflush_mouse_statu() {

		}
	public:
		//Functions
		Element(PIMAGE image,Position pos) {
			this->__visible_image = newimage(getwidth(),getheight());
			setbkcolor(EGERGBA(0,0,0,0),this->__visible_image);
			this->pos = pos;
			this->image_vector.push_back(image);
			this->is_show = true;
			this->scale = 100;
			this->angle = 0;
			this->order = 0;
		}
		Element(PIMAGE image,int x = 0,int y = 0) {
			this->__visible_image = newimage(getwidth(),getheight());
			setbkcolor(EGERGBA(0,0,0,0),this->__visible_image);
			this->pos = *(new Position(x,y));
			this->image_vector.push_back(image);
			this->is_show = true;
			this->scale = 100;
			this->angle = 0;
			this->order = 0;
			for(int i = 0; i < 10; ++ i) this->private_variables[i] = 0;
		}
		void call() {
			this->reflush_mouse_statu();
			for(int i = 0; i < this->frame_function_vector.size(); ++ i) {
				this->frame_function_vector[i](this);
			}
			if(!this->is_show) return;
			
			//
			cout<<this->current_image<<endl;
			PIMAGE image = this->image_vector[this->current_image];
//			putimage_rotatezoom(NULL,image,this->pos.x,this->pos.y,0.5,0.5,this->angle / 180.00f,this->scale / 100.00f,1);
//			cout<<"<><>"<<EGEGET_A(getpixel(0,0))<<endl;
			delimage(this->__visible_image);
			this->__visible_image = newimage(getwidth(),getheight());
			setbkcolor(EGERGBA(0,0,0,0),this->__visible_image);
			putimage_rotatezoom(this->__visible_image,image,this->pos.x,this->pos.y,0.5,0.5,this->angle / 180.00f,this->scale / 100.00f,1);
			putimage(0,0,this->__visible_image);
			
			/*
				Test click
			*/
			auto e = this; 
			cout<<"<<<IS_IN = "<<this->ismousein()<<endl;
			if(e->get_variable(0) == 3) e->set_variable(0,0);
			if(e->ismousein()) {
				int statu = e->get_variable(0);
				if(e->ishit()) {
					if(statu == 1){
						e->set_variable(0,2);
//						for(int i = 0;i < this->on_mouse_move_away_function_vector.size();++ i) on_mouse_move_away_function_vector[i](this);
					}
				} else {
					if(statu == 0){
						e->set_variable(0,1);
						cout<<"ON";
						for(int i = 0;i < this->on_mouse_put_on_function_vector.size();++ i) on_mouse_put_on_function_vector[i](this);
					}
					else if(statu == 2) {
						e->set_variable(0,3);
						for(int i = 0;i < this->on_click_function_vector.size();++ i) on_click_function_vector[i](this);
					}
				}
			} else{
				if(e->get_variable(0) == 1)	{
					for(int i = 0;i < this->on_mouse_move_away_function_vector.size();++ i) on_mouse_move_away_function_vector[i](this);
					e->set_variable(0,0);
				}
				cout<<"AWAY"<<endl;
			}

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
		inline void set_posx(int x) {
			this->pos.x = x;
		}
		inline void set_posy(int y) {
			this->pos.y = y;
		}
		inline void move_to(int x,int y) {
			this->pos.x = x;
			this->pos.y = y;
		}
		inline void move_to(Position pos) {
			this->pos = pos;
		}
		inline void push_function(auto function) {
			this->frame_function_vector.push_back(function) ;
		}
		inline bool ismousein() {
			int x,y;
			mousepos(&x,&y);
			if(x < 0 || y < 0 || x > getwidth() || y > getheight()){
				cout<<"BECAUSE OUT"<<endl;
				return false;
			}
//			cout<<x<<" "<<y<<"<><>"<<EGEGET_A(getpixel(x,y,this->__visible_image))<<endl;
//			if(EGEGET_A(getpixel(x,y,this->__visible_image)) == 255) cout<<"!= alpha"<<endl;
//			else getch();
			return (EGEGET_A(getpixel(x,y,this->__visible_image)) != 0);
//			int d_width = getwidth(this->image_vector[this->current_image]) / 2;
//			int d_height = getheight(this->image_vector[this->current_image]);
//			return (x >= this->pos.x - d_width && x <= this->pos.x + d_width && y >= this->pos.y - d_height && y <= this->pos.y + d_height);
		}
		inline bool ishit() {
			if(!mousehit) return false;
			return this->ismousein();
		}
		inline void set_variable(unsigned int which,int value) {
			this->private_variables[which] = value;
		}
		inline int get_variable(unsigned int which) {
			return this->private_variables[which];
		}
		inline void add_image(PIMAGE image){
			this->image_vector.push_back(image);
		}
		inline void set_image(int order){
			this->current_image = order;
		}
		inline int get_image_order(){
			return this->current_image; 
		}
		inline PIMAGE get_image(){
			return this->image_vector[this->current_image];
		}
		inline void listen(string listen_mode,auto function){
			if(listen_mode == "frame") this->frame_function_vector.push_back(function) ;
			if(listen_mode == "on_mouse_put_on") this->on_mouse_put_on_function_vector.push_back(function) ;
			if(listen_mode == "on_mouse_move_away") this->on_mouse_move_away_function_vector.push_back(function) ;
			if(listen_mode == "on_click") this->on_click_function_vector.push_back(function) ;
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
	if(kbhit()) keymsg = getch();
	if(mousemsg()) {
		mouseinfo = getmouse();
		if(mouseinfo.is_left()) {
			mousehit = (mousehit ? false : true);
		}
	}
	flushkey();
	flushmouse();
//	xyprintf(20,20,"FPS : %0.2f",getfps());
}

void s1(Element* e) {
	e->set_image(1);
}

void s2(Element* e) {
	e->set_image(0);
}
//void fun1(Element* e) {
//	e->move_down(1);
//}
void fun2(Element* e) {
	e->hide();
//	e->decrease_scale(1);
//	if(e->getscale() == 0) e->set_scale(100);
}

int main() {
	setinitmode(INIT_RENDERMANUAL);
	initgraph(500,500); //ª≠≤º¥Û–°
	setbkcolor(EGERGB(0,0,255));
	PIMAGE e_0 = newimage();
	PIMAGE e_1 = newimage();
	getimage(e_0,".//resources//image//0.png");
	getimage(e_1,".//resources//image//1.png");
	Element e0 = *(new Element(e_0,50,50));
	e0.add_image(e_1);
	reg_Element(&e0);
	e0.show();
	e0.listen("on_mouse_put_on",s1);
	e0.listen("on_mouse_move_away",s2);
	e0.listen("on_click",fun2);
	while(true) {
//		cout<<e0.ismousein()<<"<<<"<<endl; 
		reflush();
		cout<<"==============="<<endl; 
//		delay_fps(100);
	}
}

