#ifndef __MOUSELISTENER_H__
#define __MOUSELISTENER_H_

#include "cocos2d.h"

class MouseListener
{
public:
	void GetWindowPos(HWND hWnd, POINT *p)
{
    HWND hWndParent = GetParent(hWnd);
    POINT pos = {0};

    MapWindowPoints(hWnd, hWndParent, &pos, 1);

	*p = pos;
}

cocos2d::CCPoint getMousePos(){
	POINT p;
	if(GetCursorPos(&p)){
		POINT wp;
		GetWindowPos(cocos2d::CCEGLView::sharedOpenGLView()->getHWnd(), &wp);
		p.x-= wp.x;
		p.y-= wp.y;
			
		if(p.y<=cocos2d::CCDirector::sharedDirector()->getWinSize().height){
			p.y =abs(p.y-cocos2d::CCDirector::sharedDirector()->getWinSize().height);
		}else{
			p.y =-(p.y-cocos2d::CCDirector::sharedDirector()->getWinSize().height);
		}
		return cocos2d::Point(p.x,p.y);
	}else{
		cocos2d::CCLog("Something wrong");
		return cocos2d::Point(0,0);
	}
}
};

#endif