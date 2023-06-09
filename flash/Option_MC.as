﻿// by osmosis-wrench 2022
import com.greensock.*;

class Option_MC extends MovieClip
{
	var _w:Number;
	var _h:Number;
	var myIdx:Number;

	var NodeID:String;
	var Title:String;
	var ImagePath:String;
	var Description:String;
	var NodeInfoBox:MovieClip;
	var optionText:TextField;
	var TextureLoader;
	var textureContainer:MovieClip;

	public function Option_MC()
	{
		super();
		_w = this._width;
		_h = this._height;
		this._alpha = 80;
		NodeInfoBox = _parent._parent.NodeInfoBox;
		HideOption();
	}

	// Public Functions
	public function OnHighlight()
	{
		NodeInfoBox.AssignData({Title:Title, Description:Description});
		this._width = _w;
		this._height = _h;
		TweenLite.killTweensOf(this, false, {_alpha:true});
		this._alpha = 100;
		TweenLite.to(this,0.5,{_width:_w + 3, _height:_h + 3});
	}

	public function OnUnHighlight()
	{
		this._alpha = 70;
		TweenLite.to(this,0.2,{_width:_w, _height:_h});
	}

	public function OnSelect()
	{
		TweenLite.to(this,0.5,{_width:_w + 7, _height:_h + 7});
		_parent.SendTransitionRequest(NodeID);
	}

	public function SetData(Node:Object)
	{
		NodeID = Node["NodeID"];
		Title = Node["Title"];
		ImagePath = Node["ImagePath"];
		Description = Node["Description"];
		this.TextureLoader.loadClip(ImagePath,this.textureContainer);
		NodeID != null ? this.ShowOption() : this.HideOption();
		DrawBorder(Node["Border"]);
	}

	function DrawBorder(colorStr:String)
	{
		clear();
		this.lineStyle(2,parseInt(colorStr, 16),100);
		this.moveTo(-42,-42);
		this.lineTo(42,-42);
		this.lineTo(42,42);
		this.lineTo(-42,42);
		this.lineTo(-42,-42);
	}

	public function ShowOption()
	{
		this.enabled = true;
		TweenLite.to(this,0.5,{_alpha:70});
	}

	public function HideOption()
	{
		if (this._width != _w)
		{
			_width = _w;
			_height = _h;
		}
		this.enabled = false;
		this._alpha = 0;

	}
	// Private Functions

}