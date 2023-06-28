#define MAXCLONESCOUNT 10000
#define MAXELEMENTCOUNT 1000
#include "FeEGELib.h"
using namespace FeEGE;

bool BlockedBySettings = false;

vector<string> names;
string name;

int main() {
//	freopen(".\\logs\\output.log","w",stdout);

	initgraph(700,500);

	initpen();
	setbkcolor(EGERGB(0,50,200));

	pen::setorder(3);
	pen::font(40,"幼圆");
	pen::print(230,120,"点我随机点名");
//	reflush();
//	cout<<getbkcolor();
//	getch();
//	putimage_transparent(nullptr,pen_image,0,0,EGERGBA(1,1,4,0));
//
//	delay_ms(0);
//	getch();

	//读取信息
	freopen(".\\config\\students.txt","r",stdin);
	while(cin>>name) names.push_back(name);
	if(names.size() == 0) MessageBox(getHWnd(),"无学生信息！\n请在根目录下的config\\students.txt中配置","提示",MB_OK | MB_ICONWARNING);

	//注册Element

	//choose_image定义
	PIMAGE choose_image = newimage();
	getimage(choose_image,".\\resources\\image\\choose.png");
	Element* choose_element = newElement("choose",choose_image,350,150);
	reg_Element(choose_element);
	choose_element->set_order(2);
//	choose_element->listen("frame",[](Element* e){
//		if(gettime() - e->get_variable(1) == 5){
////			pen::clear_all();
//			pen::print(230,120,"点我随机点名");
//		}
//	});

	choose_element->listen("on_click",[](Element* e) {
		if(BlockedBySettings) return;
		if(names.size() == 0) {
			MessageBox(getHWnd(),"无学生信息！\n请在根目录下的config\\students.txt中配置","提示",MB_OK | MB_ICONWARNING);
			return;
		}
//		int statu = 4;
//		while(statu == 4) statu = MessageBox(getHWnd(),names[random(names.size())].c_str(),"选取结果(点击重试再次选取)",MB_RETRYCANCEL);
		name = names[random(names.size())];
		int len = name.length() * 20;
		pen::clear_all();
		pen::print((getwidth() - len) >> 1,125,name.c_str());

		for(int i = 0; i < 8; ++ i) {
			getElementById("colorful" + to_string(i))->clone();
		}
//		e->set_variable(1,gettime());
	});

	//opensettings定义
	PIMAGE opensettings = newimage();
	getimage(opensettings,".\\resources\\image\\open_settings.png");
	Element* open_settings_element = newElement("open_settings",opensettings,350,350);
	reg_Element(open_settings_element);
	open_settings_element->set_order(2);

	open_settings_element->listen("on_click",[](Element* e) { //openSettings
		if(BlockedBySettings == true) return;
		log("[Event] Open Settings\n");
		BlockedBySettings = true;
		Element* settings_element = getElementById("settings");
		settings_element->set_variable(1,1);
	});

	//settings定义
	PIMAGE settings_image = newimage();
	getimage(settings_image,".\\resources\\image\\settings.png");
	Element* settings_element = newElement("settings",settings_image,350,250);
	reg_Element(settings_element);
	settings_element->set_scale(0);
	settings_element->set_order(4);

	settings_element->listen("frame",[](Element* e) {
		if(e->get_variable(1) == 1) {
			if(e->getscale() != 100) e->increase_scale(10);
			else {
				Element* close_settings_element = getElementById("close_settings");
				Element* open_config_element = getElementById("open_config");
				Element* reload_config_element = getElementById("reload_config");
				close_settings_element->show();
				open_config_element->show();
				reload_config_element->show();
				e->set_variable(1,0);
			}
		}
		if(e->get_variable(1) == 2) {
			if(e->getscale() != 0) e->decrease_scale(10);
			else {
				e->set_variable(1,0);
			}
		}
	});

	//close_settings定义
	PIMAGE close_settings_image = newimage();
	getimage(close_settings_image,".\\resources\\image\\close_settings.png");
	Element* close_settings_element = newElement("close_settings",close_settings_image,550,30);
	reg_Element(close_settings_element);
	close_settings_element->hide();
	close_settings_element->decrease_scale(92);
	close_settings_element->set_order(5);

	close_settings_element->listen("on_click",[](Element* e) {
		log("[Event] Close Settings\n");
		BlockedBySettings = false;
		Element* settings_element = getElementById("settings");
		Element* close_settings_element = getElementById("close_settings");
		Element* open_config_element = getElementById("open_config");
		Element* reload_config_element = getElementById("reload_config");
		settings_element->set_variable(1,2);
		close_settings_element->hide();
		open_config_element->hide();
		reload_config_element->hide();
	});

	//snow_particle定义
	PIMAGE snow_image = newimage();
	getimage(snow_image,".\\resources\\image\\snow.png");
	Element* snow_element = newElement("snow",snow_image,550,-10);
	reg_Element(snow_element);
	snow_element->hide();

	snow_element->listen("clones:on_clone",[](Element* e) {
		e->show();
		e->set_order(100);
		e->setalpha(random(0xFF + 1 - 50) + 50);
		e->decrease_scale(random(21) + 30);
		e->set_posx(random(getwidth()));
	});

	snow_element->listen("frame",[](Element* e) {
		if(random(46) == 0) e->clone()->listen("frame",[](Element* e) {
			e->move_down(1);
			e->turn_right(3);
			if(e->getposition().y >= getheight() + 20){
				e->deletethis();
			}
		});
	});

	//彩色溅射定义
	PIMAGE colorful_image[8];
	Element* colorful_element[8];
	for(int i = 0; i < 8; ++ i) {
		colorful_image[i] = newimage();
		getimage(colorful_image[i],(".\\resources\\image\\colorful" + to_string(i) + ".png").c_str(),0,0);
		colorful_element[i] = newElement("colorful" + to_string(i),colorful_image[i],getwidth() / 2,getheight() / 2);
		colorful_element[i]->hide();
//		colorful_element[i]->increase_order(20);
		colorful_element[i]->turn_to(i * 45);
		colorful_element[i]->listen("clones:on_clone",[](Element* e) {
			e->set_order(1);
			e->listen("frame",[](Element* e) {
				e->move_forward(40);
				Position pos = e->getposition();
				if(pos.x < 0 || pos.x > getwidth() || pos.y < 0 || pos.y > getheight()) {
					e->deletethis();
					return;
				}
				if(e->get_variable(2) != 1) e->set_variable(2,1);
				else e->show();
			});
		});
	
		reg_Element(colorful_element[i]);
	}
	
	//打开配置文件定义
	PIMAGE open_config_image = newimage();
	getimage(open_config_image,".\\resources\\image\\open_config.png");
	Element* open_config_element = newElement("open_config",open_config_image,350,400);
	open_config_element->hide();
	open_config_element->set_order(5);
	reg_Element(open_config_element);
	
	open_config_element->listen("on_click",[](Element* e) {
		log("[Event] Open config\n");
		system(".\\config\\students.txt");
	});
	
	//重载配置文件定义
	PIMAGE reload_config_image = newimage();
	getimage(reload_config_image,".\\resources\\image\\reload_config.png");
	Element* reload_config_element = newElement("reload_config",reload_config_image,350,441);
	reload_config_element->hide();
	reload_config_element->set_order(5);
	reg_Element(reload_config_element);
	
	reload_config_element->listen("on_click",[](Element* e) {
		names.clear();
		ifstream in;
		in.open(".\\config\\students.txt",ios::in);
		while(in>>name) names.push_back(name);
		MessageBox(getHWnd(),("重载成功，共有" + to_string(names.size()) + "个学生信息").c_str(),"提示",MB_OK);
		pen::clear_all();
		pen::print(230,120,"点我随机点名");
	});
	
	
	start(120);
}
