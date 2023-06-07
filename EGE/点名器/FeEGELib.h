#ifndef _FEEGELIB_

#define _FEEGELIB_
#include<graphics.h>
#include<vector>
#include<string>
#include<windows.h>
#include<algorithm>
#include<iostream>
#include<queue>

#ifndef MAXCLONESCOUNT
#define MAXCLONESCOUNT 0
#endif

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

//Classes

//union TYPE_UNION{
//	int type_int;
//	bool type_bool;
//	string type_string;
//	void* type_pointer;
//	unsigned int type_uint;
//};

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
			cout<<"[CONSTRUCTURE]ptr: "<<this<<" Id: "<<this->id<<endl;
			this->__visible_image = newimage(getwidth(),getheight());
			setbkcolor(EGERGBA(1,1,4,0),this->__visible_image);
			this->pos = pos;
			this->image_vector.push_back(image);
			this->is_show = true;
			this->scale = 100;
			this->angle = 0;
			this->order = 0;
			this->clonecount = 0;
		}
		Element(string id,PIMAGE image,double x = 0,double y = 0) {
			this->id = id;
			cout<<"[CONSTRUCTURE]ptr: "<<this<<" Id: "<<this->id<<endl;
			this->__visible_image = newimage(getwidth(),getheight());
			setbkcolor(EGERGBA(1,1,4,0),this->__visible_image);
			this->pos = *(new Position(x,y));
			this->image_vector.push_back(image);
			this->is_show = true;
			this->scale = 100;
			this->angle = 0;
			this->order = 0;
			this->clonecount = 0;
//			for(int i = 0; i < 10; ++ i) this->private_variables[i] = 0;
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
			putimage_rotatezoom(nullptr,image,this->pos.x,this->pos.y,0.5,0.5,this->angle / 180.00f,this->scale / 100.00f,1);
			delimage(this->__visible_image);
			this->__visible_image = newimage(getwidth(),getheight());
			setbkcolor(EGERGBA(1,1,4,0),this->__visible_image);
			putimage_rotatezoom(this->__visible_image,image,this->pos.x,this->pos.y,0.5,0.5,this->angle / 180.00f,this->scale / 100.00f,1);
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
			if(!mousehit) return false;
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
//			cout<<clonecount<<" ID: ";
			e[clonecount] = new Element(this->id + "_" + to_string(clonecount),this->get_image(),this->pos);
//			cout<<e[clonecount]->getId()<<" ptr: "<<e[clonecount]<<endl;
			reg_Element(e[clonecount]);
			for(int i = 0; i < this->on_clone_function_vector.size(); ++ i) this->on_clone_function_vector[i](this);
//			cout<<"SIZe:"<<this->on_clone_clones_function_vector.size()<<endl;
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
			if(listen_mode == "clones:on_clone") {
				this->on_clone_clones_function_vector.push_back(function) ;
				cout<<"ptr: "<<this<<" Id:"<<this->getId()<<" length: "<<on_clone_clones_function_vector.size()<<endl;
			}
			cout<<"[INFO]ptr: "<<this<<" listen:"<<listen_mode<<endl;
		}
		inline void deletethis() {
			int i = 0;
			for(i = 0; i < Element_queue.size(); ++ i) {
				if(Element_queue[i] == this) {
					Element_queue[i] = nullptr;
//					cout<<"[DE]É¾³ýElement_queue";
//					cout<<"=========\n";
//					for(int i = 0; i < Element_queue.size(); ++ i) {
//						cout<<Element_queue[i]<<endl;
//					}
//					cout<<"=========\n";
					removesize ++;
					needsort = true;
					this->deleted = true;
					return;
				}
			}
			removesize ++;
//			cout<<"removesize: "<<removesize<<endl;
//			delete this;
		}
		inline int geton_clone_clones_function_vector() {
			return this->frame_function_vector.size();
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
		~Element() {
			cout<<"DESTRUCTUCT : "<<this<<endl;
		}
};
int current_reg_order = 0;
unsigned long long frame = 0;

//Functions

void reg_Element(Element* element) {
	needsort = true;
	element->set_reg_order(current_reg_order ++);
//	cout<<"Element_queue.size(): "<<Element_queue.size()<<"  __SIZE__: "<<__SIZE__<<endl;
	if(Element_queue.size() <= __SIZE__) Element_queue.push_back(element);
//	Element_queue.push_back(element);
	else Element_queue[__SIZE__] = element;
//	cout<<"reg: (ptr)"<<element<<"  Pos: "<<__SIZE__<<endl;
	__SIZE__ ++;
}

bool cmp(Element* _A,Element* _B) {
//	cout<<"_A: "<<_A<<"  _B: "<<_B<<endl;
	if(_A == nullptr) return false;
	if(_B == nullptr) return true;
	if(_A->getorder() > _B->getorder()) return false;
	else if (_A->getorder() < _B->getorder()) return true;
	return _A->getreg_order() < _B->getreg_order();
}

void reflush() {
//	cout<<"[Info] New Frame"<<endl;
	++ frame;
	cleardevice();
//	vector<Element*>compared ;
//	for(int i = 0; i < Element_queue.size(); ++ i) {
//		compared.push_back(*Element_queue[i]);
//	}
//	sort(co0mpared.begin(),compared.end());
////	for(int j = 0; j < Element_queue.size(); ++ j) {
//		for(int i = 0; i < Element_queue.size(); ++ i) {
//			Element_queue[i]->call();
//		}
////	}
//	for(int i = 0; i < Element_queue.size(); ++ i) {
//		compared.push_back(Element_queue[i]);
//	}
	if(needsort) {
		sort(Element_queue.begin(),Element_queue.begin() + __SIZE__,cmp);
		needsort = false;
	}
//	cout<<__SIZE__<<" -> ";
	__SIZE__ -= removesize;
//	cout<<__SIZE__<<endl;
	removesize = 0;
//	if(need_sort){
//		for()
//		need_sort = false;
//	}
//	cout<<"=========\n";
//	for(int i = 0; i < __SIZE__; ++ i) {
//		cout<<Element_queue[i]<<endl;
//	}
//	cout<<"=========\n";
	for(int i = 0; i < __SIZE__; ++ i) {
		if(Element_queue[i] != nullptr) Element_queue[i]->call();
	}

//	cout<<"========================\n";
//	for(int i = 0;i < Element_queue.size();++ i){
//		cout<<"ptr: "<<Element_queue[i]<<" Id: "<<Element_queue[i]->getId()<<" length: "<<Element_queue[i]->geton_clone_clones_function_vector()<<endl;
//	}
//	cout<<"========================\n";

	delay_ms(0);
	iskey = false;
//	if(kbhit()) keymsg = getch();
	if(mousemsg()) {
		mouseinfo = getmouse();
		if(mouseinfo.is_left()) {
			mousehit = (mousehit ? false : true);
		}
	}
//	flushkey();
	flushmouse();
//	xyprintf(20,20,"FPS : %0.2f",getfps());
	char fps[100];
	sprintf(fps,"FPS : %0.2f",getfps());
	setcaption(fps);
}

void start(int fps) {
	while(is_run()) {
		reflush();
//		delay_fps(fps);
		delay_ms(1);
	}
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
//				cout<<Element_queue[i]->getId()<<" == "<<ElementId<<" ptr: "<<Element_queue[i]<<endl;
				return Element_queue[i];
			}
		}
		return nullptr;
	}
}
#endif
