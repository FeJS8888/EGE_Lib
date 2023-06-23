#ifndef _FEEGELIB_

#define _FEEGELIB_
#include<graphics.h>
#include<vector>
#include<string>
#include<windows.h>
#include<algorithm>
#include<iostream>
#include<queue>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>

#ifndef MAXCLONESCOUNT
#define MAXCLONESCOUNT 0
#endif

#define LeftButton VK_LBUTTON
#define log printf
#define PIE 3.141592653589793238462643383279502f

using namespace std;

class Position;
class Element;
void reg_Element(Element* element);

bool iskey = false;
key_msg keymsg;
bool mousehit = false;
bool needsort = true;
mouse_msg mouseinfo;
bool keystatus[360];
vector<Element*>Element_queue;
int __SIZE__ = 0;
int removesize = 0;

PIMAGE pen_image;


//Classes

//union TYPE_UNION{
//	int type_int;
//	bool type_bool;
//	string type_string;
//	void* type_pointer;
//	unsigned int type_uint;
//};


#include <iostream>
#include<ctime>
#include<cstdio>
#include<time.h>
using namespace std;

class Position {
	public:
		//Variables
		double x;
		double y;

		// Functions
		Position(double x = 0,double y = 0) {
			this->x = x;
			this->y = y;
		}
		~Position() {}
};

class Element {
	private:
		//Variables
		string id;
		unsigned short scale;
		int angle;
		int order;
		unsigned char alpha;
		int reg_order;
		Position pos;
		Position backup_pos;
		bool is_cancel_x;
		bool is_cancel_y;
		bool is_show;
		PIMAGE __visible_image;
		vector<PIMAGE> image_vector;
		vector<Element*> clones;
		vector<void(*)(Element*)> frame_function_vector;
		vector<void(*)(Element*)> on_mouse_put_on_function_vector;
		vector<void(*)(Element*)> on_mouse_hitting_function_vector;
		vector<void(*)(Element*)> on_mouse_move_away_function_vector;
		vector<void(*)(Element*)> on_click_function_vector;
		vector<void(*)(Element*)> on_clone_function_vector;
		vector<void(*)(Element*)> on_clone_clones_function_vector;
		unsigned int current_image = 0;
		int private_variables[10];
		int clonecount;
		bool deleted = false;
		inline void reflush_mouse_statu() {
			/*
				Test click
			*/
			if(this->ismousein()) {
				int statu = this->get_variable(0);
				if(this->ishit()) {
					if(statu == 1) {
						this->set_variable(0,2);
						for(int i = 0; i < this->on_mouse_hitting_function_vector.size(); ++ i) on_mouse_hitting_function_vector[i](this);
					}
				} else {
					if(statu == 0) {
						this->set_variable(0,1);
						for(int i = 0; i < this->on_mouse_put_on_function_vector.size(); ++ i) on_mouse_put_on_function_vector[i](this);
					} else if(statu == 2) {
						this->set_variable(0,0);
						for(int i = 0; i < this->on_click_function_vector.size(); ++ i) on_click_function_vector[i](this);
					}
				}
			} else {
				if(this->get_variable(0) == 1)	{
					for(int i = 0; i < this->on_mouse_move_away_function_vector.size(); ++ i) on_mouse_move_away_function_vector[i](this);
					this->set_variable(0,0);
				}
			}
		}
	public:
		//Functions
		Element(string id,PIMAGE image,Position pos) {
			this->id = id;
			log("[CONSTRUCTURE]ptr: 0x%x id: %s\n",this,this->id.c_str());
			this->__visible_image = newimage(getwidth(),getheight());
			setbkcolor(EGERGBA(1,1,4,0),this->__visible_image);
			this->pos = pos;
			this->image_vector.push_back(image);
			this->is_show = true;
			this->scale = 100;
			this->angle = 0;
			this->order = 0;
			this->clonecount = 0;
			this->alpha = 0xFF;
			for(int i = 0; i < 10; ++ i) this->private_variables[i] = 0;
		}
		Element(string id,PIMAGE image,double x = 0,double y = 0) {
			this->id = id;
			log("[CONSTRUCTURE]ptr: 0x%x id: %s\n",this,this->id.c_str());
			this->__visible_image = newimage(getwidth(),getheight());
			setbkcolor(EGERGBA(1,1,4,0),this->__visible_image);
			this->pos = *(new Position(x,y));
			this->image_vector.push_back(image);
			this->is_show = true;
			this->scale = 100;
			this->angle = 0;
			this->order = 0;
			this->clonecount = 0;
			this->alpha = 0xFF;
			for(int i = 0; i < 10; ++ i) this->private_variables[i] = 0;
		}
//		Element(Element* that) {
//			this->__visible_image = newimage(getwidth(),getheight());
//			setbkcolor(EGERGBA(1,1,4,0),this->__visible_image);
//			this->image_vector = that->image_vector;
//			this->current_image = that->current_image;
//			this->pos = *(new Position(that->pos.x,that->pos.y));
//			this->is_show = true;
//			this->scale = 100;
//			this->angle = 0;
//			this->order = 0;
//			this->clonecount = 0;
//		}
		void call() {
			this->backup_pos = pos;
			this->reflush_mouse_statu();
			for(int i = 0; i < this->frame_function_vector.size(); ++ i) this->frame_function_vector[i](this);
			if(this->deleted) return;
			if(!this->is_show) return;

			//backup
			if(this->is_cancel_x) {
				this->is_cancel_x = false;
				this->pos.x = this->backup_pos.x;
			}
			if(this->is_cancel_y) {
				this->is_cancel_y = false;
				this->pos.y = this->backup_pos.y;
			}
			//

			PIMAGE image = this->image_vector[this->current_image];
//			PIMAGE alpha_image = newimage(getwidth(image),getheight(image));
//			putimage_transparent(alpha_image,image,0,0,EGERGBA(11,))
			putimage_rotatezoom(nullptr,image,this->pos.x,this->pos.y,0.5,0.5,this->angle / 180.00f * PIE,this->scale / 100.00f,1,this->alpha);

			delimage(this->__visible_image);
			this->__visible_image = newimage(getwidth(),getheight());
			setbkcolor(EGERGBA(1,1,4,0),this->__visible_image);
			putimage_rotatezoom(this->__visible_image,image,this->pos.x,this->pos.y,0.5,0.5,this->angle / 180.00f * PIE,this->scale / 100.00f,1);
			this->reflush_mouse_statu();
		}
		inline void move_left(double pixels = 0) {
			this->pos.x -= pixels;
		}
		inline void move_right(double pixels = 0) {
			this->pos.x += pixels;
		}
		inline void move_up(double pixels = 0) {
			this->pos.y -= pixels;
		}
		inline void move_down(double pixels = 0) {
			this->pos.y += pixels;
		}
		inline void move_forward(double pixels = 0){
			this->pos.x -= sin(this->angle / 180.00f * PIE) * pixels; 
			this->pos.y -= cos(this->angle / 180.00f * PIE) * pixels; 
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
			this->angle = (this->angle - angle) % 360;
		}
		inline void turn_left(int angle) {
			this->angle = (this->angle + angle) % 360;
		}
		inline void turn_to(int angle) {
			this->angle = angle % 360;
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
		inline void move_to(double x,double y) {
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
			if(GetForegroundWindow() != getHWnd()) return false;
			if(!this->is_show) return false;
			int x,y;
			mousepos(&x,&y);
			if(x < 0 || y < 0 || x > getwidth() || y > getheight()) {
//				cout<<"BECAUSE OUT"<<endl;
				return false;
			}
//			cout<<x<<" "<<y<<"<><>"<<EGEGET_A(getpixel(x,y,this->__visible_image))<<"<><>"<<getpixel(x,y,this->__visible_image)<<endl;
			//getch();
//			cout<<"======DEBUG=========\nPRINT __visible_image\n";
//			putimage(0,0,this->__visible_image);
//putimage_rotatezoom(nullptr,this->__visible_image,this->pos.x,this->pos.y,0.5,0.5,this->angle / 180.00f,this->scale / 100.00f,1);
			//getch();
//			if(EGEGET_A(getpixel(x,y,this->__visible_image)) == 255) cout<<"!= alpha"<<endl;
//			else //getch();
//			cout<<"GETA"<<EGEGET_A(getpixel(x,y,this->__visible_image)<<endl;
			return ((EGEGET_A(getpixel(x,y,this->__visible_image)) != 0) || (getpixel(x,y,this->__visible_image) != 65796)); //EGERGBA(1,1,4,0) = 65796
//			int d_width = getwidth(this->image_vector[this->current_image]) / 2;
//			int d_height = getheight(this->image_vector[this->current_image]);
//			return (x >= this->pos.x - d_width && x <= this->pos.x + d_width && y >= this->pos.y - d_height && y <= this->pos.y + d_height);
		}
		inline bool ishit() {
			if(!GetAsyncKeyState(LeftButton)) return false;
			return this->ismousein();
		}
		inline void set_variable(unsigned int which,int value) {
			this->private_variables[which] = value;
		}
		inline int get_variable(unsigned int which) {
			return this->private_variables[which];
		}
		inline void add_image(PIMAGE image) {
			this->image_vector.push_back(image);
		}
		inline void set_image(int order) {
			this->current_image = order;
		}
		inline int get_image_order() {
			return this->current_image;
		}
		inline PIMAGE get_image() {
			return this->image_vector[this->current_image];
		}
		inline bool is_touched_by(Element* that) {
			if(!this->is_show || !that->is_show) return false;
			for(int x = this->pos.x - getwidth(this->image_vector[this->current_image]); x <= this->pos.x + getwidth(this->image_vector[this->current_image]); ++ x) {
				for(int y = this->pos.y - getheight(this->image_vector[this->current_image]); y <= this->pos.y + getheight(this->image_vector[this->current_image]); ++ y) {
					if(x < 0 || y < 0 || x >= getwidth() || y >= getheight()) continue;
					if(getpixel(x,y,this->__visible_image) == 65796) continue;
					if(getpixel(x,y,that->__visible_image) == 65796) continue;
//					cout<<"=====DEBUG=====\ntouched at :("<<x<<" "<<y<<")\n==============="<<endl;
					return true;
				}
			}
		}
		inline Element* clone() {
			static Element* e[MAXCLONESCOUNT];
			e[clonecount] = new Element(this->id + "_" + to_string(clonecount),this->get_image(),this->pos);
			e[clonecount]->angle = this->angle;
			e[clonecount]->scale = this->scale;
			e[clonecount]->is_show = this->is_show;
			reg_Element(e[clonecount]);
			for(int i = 0; i < this->on_clone_function_vector.size(); ++ i) this->on_clone_function_vector[i](this);
			for(int i = 0; i < this->on_clone_clones_function_vector.size(); ++ i) {
				this->on_clone_clones_function_vector[i](e[clonecount]);
			}
			return e[clonecount ++];
		}
		inline string getId() {
			return this->id;
		}
		inline void listen(string listen_mode,auto function) {
			if(listen_mode == "frame") this->frame_function_vector.push_back(function) ;
			if(listen_mode == "on_mouse_put_on") this->on_mouse_put_on_function_vector.push_back(function) ;
			if(listen_mode == "on_mouse_hitting") this->on_mouse_hitting_function_vector.push_back(function) ;
			if(listen_mode == "on_mouse_move_away") this->on_mouse_move_away_function_vector.push_back(function) ;
			if(listen_mode == "on_click") this->on_click_function_vector.push_back(function) ;
			if(listen_mode == "on_clone") this->on_clone_function_vector.push_back(function) ;
			if(listen_mode == "clones:on_clone") this->on_clone_clones_function_vector.push_back(function) ;
			log("[Info] ptr: 0x%x listen: %s\n",this,listen_mode.c_str());
		}
		inline Element* deletethis() {
			int i = 0;
			for(i = 0; i < Element_queue.size(); ++ i) {
				if(Element_queue[i] == this) {
					Element_queue[i] = nullptr;
					removesize ++;
					needsort = true;
					this->deleted = true;
					return this;
				}
			}
		}
		inline vector<void (*)(Element*)> geton_clone_clones_function_vector() {
			return this->frame_function_vector;
		}
		inline void cancel_x() {
			this->is_cancel_x = true;
		}
		inline void cancel_y() {
			this->is_cancel_y = true;
		}
		inline void cancel_move() {
			this->is_cancel_x = true;
			this->is_cancel_y = true;
		}
		inline unsigned char getalpha() {
			return this->alpha;
		}
		inline void setalpha(unsigned char alpha) {
			this->alpha = alpha;
		}
		inline void decrease_alpha(unsigned char alpha) {
			this->alpha -= alpha;
		}
		~Element() {
			log("[DESTRUCTUCT] Delete 0x%x\n",this);
		}
};
int current_reg_order = 0;
unsigned long long frame = 0;


namespace pen {
	int order = 0;
	void print(int x,int y,string str) {
		if(pen_image == nullptr) return;
		outtextxy(x,y,str.c_str(),pen_image);
	}
	void font(int scale,string fontname) {
		if(pen_image == nullptr) return;
		setfont(scale,0,fontname.c_str(),pen_image);
	}
	void color(color_t color) {
		if(pen_image == nullptr) return;
		setcolor(color,pen_image);
	}
	void clear(int x,int y,int ex,int ey) {
		if(pen_image == nullptr) return;
		bar(x,y,ex,ey,pen_image);
	}
	void clear_all() {
		if(pen_image == nullptr) return;
		bar(0,0,getwidth(pen_image),getheight(pen_image),pen_image);
	}
	void setorder(int value) {
		order = value;
	}
}

//Functions

void reg_Element(Element* element) {
	needsort = true;
	element->set_reg_order(current_reg_order ++);
	if(Element_queue.size() <= __SIZE__) Element_queue.push_back(element);
	else Element_queue[__SIZE__] = element;
	__SIZE__ ++;
}

bool cmp(Element* _A,Element* _B) {
	if(_A == nullptr) return false;
	if(_B == nullptr) return true;
	if(_A->getorder() > _B->getorder()) return false;
	else if (_A->getorder() < _B->getorder()) return true;
	return _A->getreg_order() < _B->getreg_order();
}

void reflush() {
	++ frame;
	cleardevice();
	if(needsort) {
		sort(Element_queue.begin(),Element_queue.begin() + __SIZE__,cmp);
		needsort = false;
	}
	__SIZE__ -= removesize;
	removesize = 0;
	bool pen_nprinted = true;
	for(int i = 0; i < __SIZE__; ++ i) {
		if(pen_nprinted && Element_queue[i]->getorder() >= pen::order) {
			pen_nprinted = false;
			putimage_transparent(nullptr,pen_image,0,0,EGERGBA(1,1,4,0));
		}
		if(Element_queue[i] != nullptr) Element_queue[i]->call();
	}
	flushmouse();
	char fps[100];
	sprintf(fps,"FPS : %0.2f",getfps());
	setcaption(fps);

	delay_ms(1);
}

void start(int fps) {
	randomize();
	while(is_run()) {
		reflush();
	}
}

void defineElement() {
}



namespace FeEGE {
	short getkey(int VB) {
		return GetAsyncKeyState(VB);
	}
	Element* getElementById(string ElementId) {
		for(int i = 0; i < Element_queue.size(); ++ i) {
			if(Element_queue[i] == nullptr) continue;
			if(Element_queue[i]->getId().length() != ElementId.length()) continue;
			if(Element_queue[i]->getId() == ElementId) {
				return Element_queue[i];
			}
		}
		return nullptr;
	}
	void initpen() {
		int Width = getwidth();
		int Height = getheight();

		//≥ı ºªØª≠± 

		if(pen_image != nullptr) delimage(pen_image);
		pen_image = newimage(Width,Height);
		setbkcolor(EGERGBA(1,1,4,0),pen_image);
		setcolor(EGERGB(255,0,0),pen_image);
		setfillcolor(EGERGBA(1,1,4,0),pen_image);

//		static Element* pen_element = new Element("$pen",pen_image,Width / 2,Height / 2);
//		reg_Element(pen_element);
	}
}
#endif
