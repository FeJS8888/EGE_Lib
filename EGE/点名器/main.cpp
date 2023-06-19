#include "FeEGELib.h"
using FeEGE::getElementById;
using FeEGE::getkey;

bool BlockedBySettings = false;

int main() {
	initgraph(700,500);

	setbkcolor(EGERGB(0,50,200));

	//注册Element

	//choose_image定义
	PIMAGE choose_image = newimage();
	getimage(choose_image,".\\resources\\image\\choose.png");
	Element* choose_element = new Element("choose",choose_image,350,100);
	reg_Element(choose_element);
	
	//opensettings定义
	PIMAGE opensettings = newimage();
	getimage(opensettings,".\\resources\\image\\open_settings.png");
	Element* open_settings_element = new Element("open_settings",opensettings,350,400);
	reg_Element(open_settings_element);
	
	open_settings_element->listen("on_click",[](Element* e) { //openSettings
		cout<<BlockedBySettings<<endl;
		if(BlockedBySettings == true) return;
		BlockedBySettings = true;
		Element* settings_element = getElementById("settings");
		Element* close_settings_element = getElementById("close_settings");
		settings_element->show();
		close_settings_element->show();
	});
	
	//settings定义
	PIMAGE settings_image = newimage();
	getimage(settings_image,".\\resources\\image\\settings.png");
	Element* settings_element = new Element("settings",settings_image,350,300);
	reg_Element(settings_element);
	settings_element->hide();
	
	//close_settings定义
	PIMAGE close_settings_image = newimage();
	getimage(close_settings_image,".\\resources\\image\\close_settings.png");
	Element* close_settings_element = new Element("close_settings",close_settings_image,350,300);
	reg_Element(close_settings_element);
	close_settings_element->hide();
	close_settings_element->decrease_scale(92);
	close_settings_element->listen("on_click",[](Element* e) {
		BlockedBySettings = false;
		Element* settings_element = getElementById("settings");
		Element* close_settings_element = getElementById("close_settings");
		settings_element->hide();
		close_settings_element->hide();
	});
	start(120);
}
