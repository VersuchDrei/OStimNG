import com.greensock.*;

class SpeedButton extends MovieClip
{

	var shape:MovieClip;
	var _w;
	var _h;
	
	public function SpeedButton(){
		_w = _width;
		_h = _height;
		_alpha = 70;
	}

	public function Show(showButton:Boolean)
	{

		if (!showButton)
		{
			shape.gotoAndStop(31);
		}
		else
		{
			shape.gotoAndStop(1);
		}
	}
	
	public function OnHighlight(){
		this._width = _w;
		this._height = _h;
		TweenLite.killTweensOf(this, false, {_alpha:true});
		this._alpha = 100;
		TweenLite.to(this,0.5,{_width:_w + 3, _height:_h + 3});
	}
	
	public function OnUnhighlight(){
		this._alpha = 70;
		TweenLite.to(this,0.2,{_width:_w, _height:_h});
	}
}